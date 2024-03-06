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
  ```cpp
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

## How To Compile & Run?
- Download the latest aarch64 compiler toolchain from the [Arm Developer website](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
  - If you are using Linux, you must install it manually and cannot install it through your package managers repository as they contain outdated binaries.
  - Ensure you have and are able to use `aarch64-none-elf-gcc`, `aarch64-none-elf-ld` and `aarch64-none-elf-as`.
- Download and install Qemu's aarch64 virtual machine (`qemu-system-aarch64`).

Compile the code with the following (adjust optimization level if needed!):
- `aarch64-none-elf-as -mcpu=cortex-a72 -march=armv8-a _start.asm -o _start.o`
- `aarch64-none-elf-g++ -O1 -ffreestanding -nostdlib -o main.o -c main.cpp`
- `aarch64-none-elf-gcc -O1 -ffreestanding -nostdlib -o ramfb.o -c ramfb.c`
- `aarch64-none-elf-ld -T linker.ld _start.o main.o -o kernel.elf`

Now you can run the code with 128MB of RAM, emulating an `Arm Cortex A72` with Qemu's ramfb with:
- `qemu-system-aarch64 -machine virt -m 128M -cpu cortex-a72 -vga none -device ramfb -kernel kernel.elf`

If everything went to plan, you should see Qemu with a red screen:
![image](https://github.com/SpeedyCraftah/arm-v8a-virt-ramfb/assets/45142584/6d62d88a-9376-4906-ae94-334d775c5785)

## Note
ramfb is meant to be an extremely basic graphics solution, Qemu recommends against using it for any practical reasons other than small graphics or testing as it will result in extremely poor performance due to the framebuffer being updated on every single pixel update.
For more serious uses, you should opt into using a GPU backed framebuffer using something along the lines of the Virtio/MailBox API, which can be quite complicated for beginners and so a library is recommended.

## Footnotes
I was looking for a simple framebuffer implementation when I was learning Arm and want to research certain aspects of it, and to do this needed a simple framebuffer/display driver working, but was surprised at how scarce these resources and documentation are, particularly when using the generic Qemu machine, Virt which is a generic Arm board which supports numerous different Arm processors, in this case the Cortex A72.

The only resources I could really find are some vague posts and the [SeaBios implementation of ramfb](https://github.com/qemu/seabios/blob/master/vgasrc/ramfb.c), however this was written for X86 and ways of interacting with Qemu's firmware config is a little different for Arm, and as a result wanted to write a simple barebones example for others in a similar position that just want to get a simple framebuffer working without having to understand all of Qemu's intrinsics (like I unfortunately had to do).

This [Qemu documentation](https://www.qemu.org/docs/master/specs/fw_cfg.html) was also very helpful in understanding the `fw_cfg` interface and setting this whole thing up, although does not explain how you can get DMA access working with Arm and gives pointers on how you can do it.
