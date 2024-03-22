#include "floppy.h"
#include "io.h"
#include "irq.h"
#include <stdio.h>
#include <string.h>

#define FLOPPY_IRQ 6
#define FLOPPY_MOTOR_OFF  0x00
#define FLOPPY_MOTOR_ON   0x01
#define FLOPPY_MOTOR_WAIT 0x02
#define FLOPPY_BUFFER_LEN 0x4800

static volatile int floppy_motor_ticks = 0;
static volatile int floppy_motor_state = FLOPPY_MOTOR_OFF;
static uint8_t floppy_buffer[FLOPPY_BUFFER_LEN] __attribute__((aligned(0x8000)));

enum FLOPPY_REG {
    FLOPPY_DOR  = 2, //digital output
    FLOPPY_MSR  = 4, //master status, read only
    FLOPPY_FIFO = 5, //data FIFO
    FLOPPY_CCR  = 7 //configuration control
};

enum FLOPPY_CMD
{
    FLOPPY_CMD_SPECIFY          = 3,
    FLOPPY_CMD_WRITE_DATA       = 5,
    FLOPPY_CMD_READ_DATA        = 6,
    FLOPPY_CMD_RECALIBRATE      = 7,
    FLOPPY_CMD_SENSE_INTERRUPT  = 8,
    FLOPPY_CMD_SEEK             = 15,
};

static const char* drive_types_str[8] = {
    "None",
    "360KB 5.25\"",
    "360MB 5.25\"",
    "360KB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "Unknown Type",
    "Unknown Type"
};

void i686_detect_drives()
{
    i686_outb(0x70, 0x10);
    unsigned drives = i686_inb(0x71);

    printf("[LOG]Detected Floppy Drive 0: %s", drive_types_str[drives >> 4]);
    printf("[LOG]Detected Floppy Drive 1: %s", drive_types_str[drives & 0xF]);
}

int floppy_cmd(int base, uint8_t cmd)
{
    for (int i = 0;i < 200;i++)
    { //time out in 20 seconds
        i686_sleep(100); //100ms
        if ((i686_inb(base + FLOPPY_MSR) & 0xC0) == 0x80)
        {
            i686_outb(base + FLOPPY_FIFO, cmd);
            return 1;
        }
    }
    printf("[ERROR]Floppy command timed out.");
    return 0;
}

uint32_t floppy_read_port(int base)
{
    for (int i = 0;i < 600;i++)
    { //time out in 60 seconds
        i686_sleep(100); //100ms
        if (i686_inb(base + FLOPPY_MSR) & 0x80)
        {
            return i686_inb(base + FLOPPY_FIFO);
        }
    }
    printf("[ERROR]Floppy disk read timed out.");
    return 0;
}

void floppy_motor(int base, int state)
{
    if (state == FLOPPY_MOTOR_ON)
    {
        if (floppy_motor_state == FLOPPY_MOTOR_OFF)
        {
            i686_outb(base + FLOPPY_DOR, 0x1C); //on
            i686_sleep(500); //wait for 500ms
        }
        floppy_motor_state = FLOPPY_MOTOR_ON;
    }
    else if (state == FLOPPY_MOTOR_OFF)
    {
        i686_outb(base + FLOPPY_DOR, 0x1C); //off
        floppy_motor_state = FLOPPY_MOTOR_OFF;
    }
    else
    {
        if (floppy_motor_state == FLOPPY_MOTOR_WAIT)
        {
            printf("[LOG]Floppy: Attempt to wait while already waiting.");
        }

        floppy_motor_ticks = 300; //3 seconds
        floppy_motor_state = FLOPPY_MOTOR_WAIT;
    }
}

void floppy_check_interrupt(int base, int* st0, int* cyl)
{
    floppy_cmd(base, FLOPPY_CMD_SENSE_INTERRUPT);

    *st0 = floppy_read_port(base);
    *cyl = floppy_read_port(base);
}

static const char* floppy_err_status[] = { "normal", "error", "invalid", "drive" };

int floppy_calibrate(int base)
{
    floppy_motor(base, FLOPPY_MOTOR_ON);

    for (int i = 0; i < 5; i++)
    {
        i686_wait_ready(FLOPPY_IRQ);
        floppy_cmd(base, FLOPPY_CMD_RECALIBRATE);
        floppy_cmd(base, 0); //support drive 0
        i686_wait_max_time(FLOPPY_IRQ, 3000);
        
        int st0, cyl;
        floppy_check_interrupt(base, &st0, &cyl);

        if (st0 & 0xC0)
        {
            printf("[ERROR]Floppy calibration status \'%s\'.", floppy_err_status[st0 >> 6]);
            continue;
        }

        floppy_motor(base, FLOPPY_MOTOR_OFF);
        return 0;
    }

    printf("[ERROR]Floppy calibration: failed 5 retries.");
    floppy_motor(base, FLOPPY_MOTOR_OFF);
    return 1;
}

int i686_floppy_reset(int base)
{
    i686_wait_ready(FLOPPY_IRQ);
        
    //reset controller
    i686_outb(base + FLOPPY_DOR, 0x00); //disable
    i686_outb(base + FLOPPY_DOR, 0x0C); //enable

    //sleep until interrupt occurs
    i686_wait(FLOPPY_IRQ);
    
    //set transfer speed to 500kb/s
    i686_outb(base + FLOPPY_CCR, 0x00);

    floppy_cmd(base, FLOPPY_CMD_SPECIFY);
    floppy_cmd(base, 0xDF); // steprate = 3ms, unload times = 240ms
    floppy_cmd(base, 0x02); // load time = 16ms, no-DMA = 0

    if (floppy_calibrate(base))
        return 1; //fail
    return 0;
}

int floppy_seek(int base, int cyl, int head)
{
    floppy_motor(base, FLOPPY_MOTOR_ON);

    for (int i = 0; i < 5; i++)
    {
        i686_wait_ready(FLOPPY_IRQ);
        floppy_cmd(base, FLOPPY_CMD_SEEK);
        floppy_cmd(base, head << 2 | 0); //head | drive
        floppy_cmd(base, cyl);
        i686_wait_max_time(FLOPPY_IRQ, 3000);

        int st0, cyl;
        floppy_check_interrupt(base, &st0, &cyl);

        if (st0 & 0xC0)
        {
            printf("[ERROR]Floppy seek status \'%s\'.", floppy_err_status[st0 >> 6]);
            continue;
        }
        
        floppy_motor(base, FLOPPY_MOTOR_OFF);
        return 0;
    }

    printf("[ERROR]Floppy seek: failed 5 retries.");
    floppy_motor(base, FLOPPY_MOTOR_OFF);
    return 1;
}

int floppy_dma_init(int dir)
{
    union {
        uint8_t b[4];
        uint32_t l;
    } addr, count;

    addr.l = (uint32_t) &floppy_buffer;
    count.l = (uint32_t) FLOPPY_BUFFER_LEN;

    if ((addr.l >> 24) || (count.l >> 16) || ( ( (addr.l & 0xFFFF) + count.l ) >> 16 ))
    {
        printf("[ERROR] Floppy DMA initialisation failed: static buffer problem.");
        return 1;
    }

    unsigned char mode;
    if (dir == FLOPPY_DIR_READ)
        mode = 0x46; // 01 0 0 01 10 = single, inc, no auto, to mem, channel 2
    else if (dir == FLOPPY_DIR_WRITE)
        mode = 0x4a; // 01 0 0 10 10 = single, inc, no auto, from mem, channel 2
    else
    {
        printf("[ERROR] Floppy DMA initialisation failed: invalid direction.");
        return 1;
    }

    i686_outb(0x0A, 0x06); //mask channel 2

    i686_outb(0x0C, 0xFF); //reset flip flop
    i686_outb(0x04, addr.b[0]); //address low byte
    i686_outb(0x04, addr.b[1]); //address high byte

    i686_outb(0x81, addr.b[2]); //external page reg

    i686_outb(0x0C, 0xFF); //reset flip flop
    i686_outb(0x05, count.b[0]); //count low byte
    i686_outb(0x05, count.b[1]); //count high byte

    i686_outb(0x0B, mode); //set mode

    i686_outb(0x0A, 0x02); //unmask channel 2

    return 0;
}

int floppy_execute(DISK* disk, int base, int lba, int dir)
{
    //write = multitrack(1):MFM(1):skipdeleted(0):0:0:1:0:1
    //read = multitrack(1):MFM(1):skipdeleted(0):0:0:1:1:0
    uint8_t cmd;
    if (dir == FLOPPY_DIR_READ)
        cmd = FLOPPY_CMD_READ_DATA | 0xC0;
    else if (dir == FLOPPY_DIR_WRITE)
        cmd = FLOPPY_CMD_WRITE_DATA | 0xC0;
    else
    {
        printf("[ERROR] Floppy execution failed: invalid direction.");
        return 1;
    }

    uint32_t lba_sectors = lba / disk->sectors;
    uint16_t sec = lba % disk->sectors + 1;
    uint16_t cyl = lba_sectors / disk->heads;
    uint16_t head = lba_sectors % disk->heads;

    //seek both heads
    if (floppy_seek(base, cyl, head)) return 1;

    for (int i = 0;i < 5;i++)
    {
        floppy_motor(base, FLOPPY_MOTOR_ON);

        //initialise dma
        if (floppy_dma_init(dir)) {
            printf("[ERROR]Floppy execution: DMA initialisation failed.");
            continue;
        }

        i686_sleep(100); //give 100ms to settle after seeks

        i686_wait_ready(FLOPPY_IRQ);
        floppy_cmd(base, cmd);  //direction
        floppy_cmd(base, head << 2 | 0); //head | drive
        floppy_cmd(base, cyl);  //cylinder
        floppy_cmd(base, head); //head
        floppy_cmd(base, sec);  //sector
        floppy_cmd(base, 2);    //bytes / sector (128 * x^2, x = 2 => 512)
        floppy_cmd(base, 18);   //end of track
        floppy_cmd(base, 27);   //GAP3 length, 27 = 3.5"
        floppy_cmd(base, 0xFF); //data length (0xff if B/S is not 0)
        i686_wait_max_time(FLOPPY_IRQ, 3000);

        uint8_t st0 = floppy_read_port(base);
        uint8_t st1 = floppy_read_port(base);
        uint8_t st2 = floppy_read_port(base);

        uint8_t rcyl = floppy_read_port(base);
        uint8_t rhead = floppy_read_port(base);
        uint8_t rsec = floppy_read_port(base);

        uint8_t bps = floppy_read_port(base); //bytes per sector

        printf("[LOG]Floppy Disk Driver: (success) read head %i, cylinder %i, sector %i.", head, cyl, sec);

        int error = 0;
        if (st0 & 0xC0)
        {
            printf("[ERROR]Floppy execution status \'%s\'.", floppy_err_status[st0 >> 6]);
            error = 1;
        }
        if (st0 & 0x08)
        {
            printf("[ERROR]Floppy execution: drive not ready.");
            error = 1;
        }
        if (st1 & 0x80)
        {
            printf("[ERROR]Floppy execution: end of cylinder.");
            error = 1;
        }
        if (st1 & 0x20)
        {
            printf("[ERROR]Floppy execution: CRC error.");
            error = 1;
        }
        if (st1 & 0x10)
        {
            printf("[ERROR]Floppy execution: controller timed out.");
            error = 1;
        }
        if (st1 & 0x04)
        {
            printf("[ERROR]Floppy execution: no data found.");
            error = 1;
        }
        if (st1 & 0x02)
        {
            printf("[ERROR]Floppy execution: not writable.");
            error = 1;
        }
        if ((st1 | st2) & 0x01)
        {
            printf("[ERROR]Floppy execution: no address mark found.");
            error = 1;
        }
        if (st2 & 0x40)
        {
            printf("[ERROR]Floppy execution: deleted address mark.");
            error = 1;
        }
        if (st2 & 0x20)
        {
            printf("[ERROR]Floppy execution: CRC error in data.");
            error = 1;
        }
        if (st2 & 0x10)
        {
            printf("[ERROR]Floppy execution: wrong cylinder.");
            error = 1;
        }
        if (st2 & 0x04)
        {
            printf("[ERROR]Floppy execution: uPD765 sector not found.");
            error = 1;
        }
        if (st2 & 0x02)
        {
            printf("[ERROR]Floppy execution: bad cylinder.");
            error = 1;
        }
        if (bps != 0x02)
        {
            printf("[ERROR]Floppy execution: expected 512B/sector, got %uB/s", (1 << (bps + 7)));
            error = 1;
        }

        if (error == 0)
        {
            floppy_motor(base, FLOPPY_MOTOR_OFF);
            return 0;
        }
        else if (error == 2)
        {
            printf("[ERROR]Floppy execution: fatal error.");
            floppy_motor(base, FLOPPY_MOTOR_OFF);
            return 2;
        }
        printf("[ERROR]Floppy execution: retrying.");
    }

    printf("[ERROR]Floppy execution: failed 5 retries.");
    floppy_motor(base, FLOPPY_MOTOR_OFF);
    return 1;
}

int i686_floppy_read_sector(DISK* disk, int base, int lba, void* data)
{
    int status;
    if (status = floppy_execute(disk, base, lba, FLOPPY_DIR_READ))
        return status;
    memcpy(data, floppy_buffer, 512);
    return status;
}

int i686_floppy_writer_sector(DISK* disk, int base, int lba, void* data)
{
    memcpy(floppy_buffer, data, 512);
    return floppy_execute(disk, base, lba, FLOPPY_DIR_WRITE);
}