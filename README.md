# arm-v8a-virt-ramfb
This simple project provides an example of how you can get a simple framebuffer working for Qemu's Virt machine emulating an aarch64 processor with numerous different pixel format/resolutions you can choose from, all included in one implementation file for simplicity.


## Motivation
I was looking for a simple framebuffer implementation when I was learning Arm and want to research certain aspects of it, and to do this needed a simple framebuffer/display driver working, but was surprised at how scarce these resources and documentation are, particularly when using the generic Qemu machine, Virt which is a generic Arm board which supports numerous different Arm processors, in this case the Cortex A72.

The only resources I could really find are some vague posts and the [SeaBios implementation of ramfb](https://github.com/qemu/seabios/blob/master/vgasrc/ramfb.c), however this was written for X86 and ways of interacting with Qemu's firmware config is a little different for Arm, and as a result wanted to write a simple barebones example for others in a similar position that just want to get a simple framebuffer working without having to understand all of Qemu's intrinsics (like I unfortunately had to do).
