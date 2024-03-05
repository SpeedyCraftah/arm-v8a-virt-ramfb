#include "ramfb.h"
#include <stdint.h>

#define fb_loc 0x40002800

extern "C" void start() {
    // Configure ramfb.
    QemuRamFBCfg ramfb_cfg = {
        .address = BE64(fb_loc),
        .fourcc = BE32(FORMAT_XRGB8888),
        .flags = BE32(0),
        .width = BE32(800),
        .height = BE32(600),
        .stride = BE32(4 * 800)
    };
    
    // Setup the framebuffer.
    qemu_ramfb_configure(&ramfb_cfg);

    // Set the entire screen to be red.
    uint32_t* fb_address = (uint32_t*)fb_loc;
    for (int i = 0; i < 800*600; i++) {
        fb_address[i] = 0xFFFF0000;
    }
}