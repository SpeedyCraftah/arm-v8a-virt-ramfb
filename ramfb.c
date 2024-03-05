#include "ramfb.h"

// Definitions for boolean type.
#define bool char
#define true 1
#define false 0

// Addresses for the fw_cfg registers (source - https://github.com/qemu/qemu/blob/db596ae19040574e41d086e78469014191d7d7fc/hw/arm/virt.c#L137).
#define fw_cfg_address ((uint8_t*)0x09020000)
#define selector_register ((uint16_t*)(fw_cfg_address + 8))
#define data_register ((uint64_t*)(fw_cfg_address + 0))
#define dma_address ((uint64_t*)(fw_cfg_address + 16))


// Structs needed for fw_cfg DMA access.

struct QemuCfgDmaAccess {
    uint32_t control;
    uint32_t length;
    uint64_t address;
} __attribute__((packed));

struct FWCfgFile {
    uint32_t size;
    uint16_t select;
    uint16_t reserved;
    char name[56];
} __attribute__((packed));

struct FWCfgFiles {
    uint32_t count;
    struct FWCfgFile files[];
} __attribute__((packed));


// Misc functions needed for functionality.

uint64_t strlen(const char* str) {
    uint64_t i = 0;
    while (*str++ != '\0') i++;
    return i;
}

bool memeq(const char* b1, const char* b2, uint64_t length) {
    for (uint64_t i = 0; i < length; i++) {
        if (*b1++ != *b2++) return false;
    }

    return true;
}


// Functions to interface with fw_cfg.

void fw_cfg_write_selector(uint16_t selector) {
    *selector_register = BE16(selector);
}

uint64_t fw_cfg_read_data() {
    return *data_register;
}

void fw_cfg_dma_transfer(void* address, uint32_t length, uint32_t control) {
    struct QemuCfgDmaAccess dma;
    dma.control = BE32(control);
    dma.address = BE64((uint64_t)address);
    dma.length = BE32(length);

    *dma_address = BE64((uint64_t)&dma);
    while (BE32(dma.control) & ~0x01);
}

void fw_cfg_dma_read(void* buf, int e, int length) {
    uint32_t control = (e << 16) | 0x08 | 0x02;
    fw_cfg_dma_transfer(buf, length, control);
}

void fw_cfg_dma_write(void* buf, int e, int length) {
    uint32_t control = (e << 16) | 0x08 | 0x10;
    fw_cfg_dma_transfer(buf, length, control);
}

// Finds the requested file name from fw_cfg.
bool fw_cfg_find_file(struct FWCfgFile* out, const char* name) {
    uint64_t name_size = strlen(name);
    uint32_t files_count;
    fw_cfg_dma_read(&files_count, 0x19, sizeof(files_count));
    files_count = BE32(files_count);

    // Since we don't have a memory allocator or buffer, we have to use the stack to store the directory.
    uint64_t directory_size = sizeof(struct FWCfgFiles) + (sizeof(struct FWCfgFile) * files_count);
    struct FWCfgFiles* directory = __builtin_alloca(directory_size);
    fw_cfg_dma_read(directory, 0x19, directory_size);

    for (int i = 0; i < files_count; i++) {
        struct FWCfgFile* file = &directory->files[i];
        if (memeq(name, file->name, name_size) == true) {
            file->size = BE32(file->size);
            file->select = BE16(file->select);
            *out = *file;

            return true;
        }
    }

    return false;
}


// Global functions.

extern void qemu_ramfb_configure(struct QemuRamFBCfg* cfg) {
    // Find ramfb handle.
    struct FWCfgFile ramfb_file;
    fw_cfg_find_file(&ramfb_file, "etc/ramfb");

    // Configure ramfb.
    fw_cfg_dma_write(cfg, ramfb_file.select, sizeof(struct QemuRamFBCfg));
}

extern void qemu_ramfb_make_cfg(struct QemuRamFBCfg* cfg, void* fb_address, uint32_t fb_width, uint32_t fb_height) {
    cfg->address = BE64((uint64_t)fb_address);
    cfg->fourcc = BE32(FORMAT_XRGB8888);
    cfg->width = BE32(fb_width);
    cfg->height = BE32(fb_height);
    cfg->flags = BE32(0);
    cfg->stride = BE32(cfg->width * 4);
}