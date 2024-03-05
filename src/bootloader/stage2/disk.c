#include "disk.h"
#include "x86.h"

int disk_init(DISK *disk, uint8_t drive_number)
{
    uint8_t drive_type;
    uint16_t cylinders, sectors, heads;

    if (!x86_disk_drive_params(drive_number, &drive_type, &cylinders, &sectors, &heads))
        return 0;

    disk->id = drive_number;
    disk->cylinders = cylinders;
    disk->sectors = sectors;
    disk->heads = heads;

    return 1;
}

int disk_read_sectors(DISK *disk, uint32_t lba, uint8_t sectors, void *data)
{
    uint32_t lba_sectors = lba / disk->sectors;

    uint16_t sector = lba % disk->sectors + 1;
    uint16_t cylinder = lba_sectors / disk->heads;
    uint16_t head = lba_sectors % disk->heads;

    for (int i = 0; i < 3; i++)
    {
        if (x86_disk_read(disk->id, cylinder, sector, head, sectors, data))
            return 1;
        
        x86_disk_reset(disk->id);
    }

    return 0;
}
