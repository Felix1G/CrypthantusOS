#include "pic.h"
#include "io.h"

#define PIC1_CMD_PORT 0x20
#define PIC1_DAT_PORT 0x21
#define PIC2_CMD_PORT 0xA0
#define PIC2_DAT_PORT 0xA1

enum {
    PIC_ICW1_ICW4           = 0x01,
    PIC_ICW1_SINGLE         = 0x02,
    PIC_ICW1_INTERVAL4      = 0x04,
    PIC_ICW1_LEVEL          = 0x08,
    PIC_ICW1_INIT           = 0x10,
} PIC_ICW1;

enum {
    PIC_ICW4_8086           = 0x1,
    PIC_ICW4_AUTOEOI        = 0x2,
    PIC_ICW4_BUFF_MASTER    = 0x4,
    PIC_ICW4_BUFF_SLAVE     = 0x1,
    PIC_ICW4_BUFFERED       = 0x1,
    PIC_ICW4_SP_FUL_NS_MD   = 0x1,
} PIC_ICW4;

enum {
    PIC_CMD_EOI             = 0x20, //end of interrupt
    PIC_CMD_READ_IRR        = 0x0A, //interrupt request register
    PIC_CMD_READ_ISR        = 0x0B, //interrupt service routine
} PIC_CMD;


void i686_pic_config(uint8_t pic1_offset, uint8_t pic2_offset)
{
    //init cw1
    i686_outb(PIC1_CMD_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
    i686_io_wait();
    i686_outb(PIC2_CMD_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
    i686_io_wait();

    //init cw2 (offsets)
    i686_outb(PIC1_DAT_PORT, pic1_offset);
    i686_io_wait();
    i686_outb(PIC2_DAT_PORT, pic2_offset);
    i686_io_wait();

    //init cw3
    i686_outb(PIC1_DAT_PORT, 0x4); //tells that a slave is at IRQ2 (0000 0100)
    i686_io_wait();
    i686_outb(PIC2_DAT_PORT, 0x2); //tells its cascade identity (0000 0010)
    i686_io_wait();

    //init cw4
    i686_outb(PIC1_DAT_PORT, PIC_ICW4_8086);
    i686_io_wait();
    i686_outb(PIC2_DAT_PORT, PIC_ICW4_8086);
    i686_io_wait();

    //clear data registers
    i686_outb(PIC1_DAT_PORT, 0);
    i686_io_wait();
    i686_outb(PIC2_DAT_PORT, 0);
    i686_io_wait();
}

void i686_pic_disable()
{
    i686_outb(PIC1_DAT_PORT, 0xFF); //mask everything
    i686_io_wait();
    i686_outb(PIC2_DAT_PORT, 0xFF); //mask everything
    i686_io_wait();
}

void i686_pic_mask(int irq)
{
    uint8_t port;

    if (irq >= 8)
    {
        irq -= 8;
        port = PIC2_DAT_PORT;
    }
    else
    {
        port = PIC1_DAT_PORT;
    }

    uint8_t mask = i686_inb(port);
    i686_outb(port, mask | (1 << irq) );
}

void i686_pic_unmask(int irq)
{
    uint8_t port;

    if (irq >= 8)
    {
        irq -= 8;
        port = PIC2_DAT_PORT;
    }
    else
    {
        port = PIC1_DAT_PORT;
    }

    uint8_t mask = i686_inb(port);
    i686_outb(port, mask & (~(1 << irq)) );
}

void i686_pic_send_eoi(int irq)
{
    if (irq >= 8)
        i686_outb(PIC2_CMD_PORT, PIC_CMD_EOI);
    i686_outb(PIC1_CMD_PORT, PIC_CMD_EOI);
}

uint16_t i686_pic_read_irr()
{
    i686_outb(PIC1_CMD_PORT, PIC_CMD_READ_IRR);
    i686_outb(PIC2_CMD_PORT, PIC_CMD_READ_IRR);
    return ( ((uint16_t)i686_inb(PIC2_CMD_PORT)) | (((uint16_t)i686_inb(PIC2_CMD_PORT)) << 8) );
}

uint16_t i686_pic_read_isr()
{
    i686_outb(PIC1_CMD_PORT, PIC_CMD_READ_ISR);
    i686_outb(PIC2_CMD_PORT, PIC_CMD_READ_ISR);
    return ( ((uint16_t)i686_inb(PIC2_CMD_PORT)) | (((uint16_t)i686_inb(PIC2_CMD_PORT)) << 8) );
}
