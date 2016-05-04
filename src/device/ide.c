#include "common.h"
#include "io/port-io.h"
#include "device/i8259.h"
#include "cpu/reg.h"

// FIXME 暂时删除文件操作，
// 我们并没有准备好通过 NPC 对外存进行控制，
// 所以目前准备像内核那样处理应用程序，即将二进制文件制作成 C 数组参与 NEMU 的编译。
// （2015 级 PA 将内核/loader也作为外部文件进行读取，现在看来取用 2014 级 PA 开了顺路倒车……）

#ifdef DEPLOY

#define fwrite(...)
#define fread(...)
#define fseek(...)
#define fopen(...) NULL

#endif // ifndef DEPLOY

#define IDE_CTRL_PORT 0x3F6
#define IDE_PORT 0x1F0
#define IDE_IRQ 14

static uint8_t *ide_port_base;
static uint32_t sector, disk_idx;
static uint32_t byte_cnt;
static bool ide_write;
static FILE *disk_fp;

void ide_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    assert(byte_cnt <= 512);
    if(is_write) {
        if(addr - IDE_PORT == 0 && len == 4) {
            assert(ide_write);
            fwrite(ide_port_base, 4, 1, disk_fp);

            byte_cnt += 4;
            if(byte_cnt == 512) {
                ide_port_base[7] = 0x40;
                i8259_raise_intr(IDE_IRQ);
            }
        }
        else if(addr - IDE_PORT == 7) {
            sector = (ide_port_base[6] & 0x1f) << 24 | ide_port_base[5] << 16
                | ide_port_base[4] << 8 | ide_port_base[3];
            disk_idx = sector << 9;
            fseek(disk_fp, disk_idx, SEEK_SET);
            
            byte_cnt = 0;

            if(ide_port_base[7] == 0x20) {
                ide_write = false;
                fread(ide_port_base, 4, 1, disk_fp);
                ide_port_base[7] = 0x40;
                i8259_raise_intr(IDE_IRQ);
            }
            else if(ide_port_base[7] == 0x30) {
                ide_write = true;
            }
            else {
                assert(0);
            }
        }
    }
    else {
        if(addr - IDE_PORT == 0 && len == 4) {
            assert(!ide_write);
            fread(ide_port_base, 4, 1, disk_fp);
            byte_cnt += 4;
            if(byte_cnt == 512) {
                ide_port_base[7] = 0x40;
            }
        }
    }
}

void init_ide() {
    ide_port_base = add_pio_map(IDE_PORT, 8, ide_io_handler);
    ide_port_base[7] = 0x40;

#ifndef DEPLOY
    extern char *exec_file;
#endif
    disk_fp = fopen(exec_file, "r+");
    assert(disk_fp);
}
