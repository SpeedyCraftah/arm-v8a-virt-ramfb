# arm-v8a-virt-ramfb
This simple project provides an example of how you can get a simple framebuffer working for Qemu's Virt machine emulating an aarch64 processor with numerous different pixel format/resolutions you can choose from, all included in a few implementation files with lots of comments.

## What's Included
- A `ramfb.h` and `ramfb.c` library that can be easily integrated into a C/C++ codebase, and a few functions which simplify the ramfb setup process:
  ```cpp
  // Address of framebuffer in RAM.
  #define fb_loc 0x40002000
  
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
  ```
  Or even simpler:
  ```
  // Address of framebuffer in RAM.
  #define fb_loc 0x40002000

  // Creates a default config with a 4-byte colour format.
  QemuRamFBCfg cfg;
  qemu_ramfb_make_cfg(&cfg, fb_loc, 800, 600);

  // Setup the framebuffer.
  qemu_ramfb_configure(&ramfb_cfg);
  ```
- A `_start.asm` file which contains the bootup assembly required to run the library as well as `main.cpp`.
- Other misc files needed to build such as the linker (ld) script. 

## Motivation
I was looking for a simple framebuffer implementation when I was learning Arm and want to research certain aspects of it, and to do this needed a simple framebuffer/display driver working, but was surprised at how scarce these resources and documentation are, particularly when using the generic Qemu machine, Virt which is a generic Arm board which supports numerous different Arm processors, in this case the Cortex A72.

The only resources I could really find are some vague posts and the [SeaBios implementation of ramfb](https://github.com/qemu/seabios/blob/master/vgasrc/ramfb.c), however this was written for X86 and ways of interacting with Qemu's firmware config is a little different for Arm, and as a result wanted to write a simple barebones example for others in a similar position that just want to get a simple framebuffer working without having to understand all of Qemu's intrinsics (like I unfortunately had to do).
