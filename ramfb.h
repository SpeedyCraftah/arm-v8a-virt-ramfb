#pragma once

#include <stdint.h>

// Conversion functions between endian numbers since Qemu fw_cfg uses big endian.

#define BE16(x) __builtin_bswap16(x)
#define BE64(x) __builtin_bswap64(x)
#define BE32(x) __builtin_bswap32(x)

// Pixel format types.

#define FORMAT_RGB888 875710290 /**< 24 bits, red=8, green=8, blue=8 */
#define FORMAT_XRGB8888 875713112 /**< 32 bits, alpha (transparency)=8, red=8, green=8, blue=8 */
#define FORMAT_RGB565 909199186 /**< 16 bits, red=5, green=6, blue=5 */

/**
 * @brief ramfb configuration struct.
 */
struct QemuRamFBCfg {
    uint64_t address;   /**< The address of the framebuffer. */
    uint32_t fourcc;    /**< The four character code representing the pixel format (use FORMAT_ defines). */
    uint32_t flags;     /**< Flags for framebuffer configuration. */
    uint32_t width;     /**< The width of the framebuffer in pixels. */
    uint32_t height;    /**< The height of the framebuffer in pixels. */
    uint32_t stride;    /**< The stride (number of bytes per row) of the framebuffer (width * bytes_per_pixel). */
} __attribute__((packed));

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes a barebones QemuRamFBCfg struct with the highest depth (32 bits), taking care of endianness automatically.
 *
 * This function is responsible for initializing the given QemuRamFBCfg structure.
 *
 * @param cfg Pointer to the QemuRamFBCfg structure to be initialized.
 */
void qemu_ramfb_make_cfg(struct QemuRamFBCfg* cfg, void* fb_address, uint32_t fb_width, uint32_t fb_height);

/**
 * @brief Configures the QEMU ramfb.
 * 
 * @param cfg Pointer to the QemuRamFBCfg struct containing the configuration parameters.
 * @param fb_address Pointer to the framebuffer address.
 */
void qemu_ramfb_configure(struct QemuRamFBCfg* cfg);

#ifdef __cplusplus
}
#endif