# pi5-bare-metal-gpio
Bare-metal ARM64 programming on the Raspberry Pi 5 — no operating system, no standard libraries, no frameworks. Just C, Assembly, and raw hardware.

This project is a learning environment for understanding what happens inside a computer chip at the lowest level: how transistors switch, how memory-mapped I/O works, and how a CPU boots from nothing.

---

## Quick Stats

| Property | Value |
|---|---|
| Architecture | ARM64 / AArch64 (ARMv8-A / ARMv9-A) |
| Hardware | Raspberry Pi 5 (BCM2712 SoC + RP1 I/O Controller) |
| Language | C + ARM64 Assembly |
| Entry point | `0x80000` in physical RAM |
| Build output | `kernel8.img` |

---

## How It Works

When the Pi 5 powers on without an OS, the firmware looks for a file called `kernel8.img` on the SD card and loads it directly into RAM at address `0x80000`. From there, execution jumps straight into your code — no kernel, no bootloader handoff, no runtime.

```
boot.S  ──────►  linker.ld  ──────►  main.c / gpio.c
Assembly setup   Memory layout       Hardware logic
       │
       ▼
  kernel8.img
  (raw binary)
```

The build pipeline cross-compiles everything on your machine into a single flat binary that the Pi can execute directly.

---

## File Structure

```
pi5-bare-metal-gpio/
├── boot.S       # Assembly startup — sets up the stack and parks unused CPU cores
├── linker.ld    # Tells the linker where to place code and data in RAM
├── main.c       # Your C entry point (called from boot.S)
├── gpio.h       # Hardware register addresses and helper macros
├── gpio.c       # GPIO driver — reads and writes hardware registers directly
└── Makefile     # Cross-compilation build pipeline
```

---

## The Boot Sequence (`boot.S`)

The Pi 5 has 4 CPU cores, and **all 4 start running simultaneously at power-on**. Before jumping into C, the assembly startup does three things:

**1. Park cores 1–3**
Only Core 0 should run. The others are put to sleep with `wfe` (Wait For Event) and looped indefinitely.

**2. Set up the stack pointer**
C needs a stack to store local variables and function call frames. The stack pointer is set to `0x80000` and grows *downward* into lower memory, while code runs *upward* from the same address — they won't collide.

**3. Jump to `main()`**
Once the environment is ready, a `bl main` instruction hands control to your C code.

```asm
_start:
    mrs     x0, mpidr_el1       // Read which core this is (0–3)
    and     x0, x0, #3
    cbz     x0, core_zero       // If Core 0, continue; otherwise sleep

halt_other_cores:
    wfe
    b       halt_other_cores

core_zero:
    ldr     x0, =_start
    mov     sp, x0              // Stack grows down from 0x80000
    bl      main                // Jump to C

hang:
    b       hang                // If main() ever returns, loop forever
```

---

## Concepts

### What is Memory-Mapped I/O (MMIO)?

Hardware peripherals (GPIO pins, UART, timers) don't have their own buses — they're accessed by reading and writing special addresses in the CPU's memory map. Writing to one of these addresses doesn't put data in RAM; it sends a signal directly to the hardware.

In C, this looks like:

```c
*(volatile uint32_t *)0x1F000D0004 = value;  // Write to a GPIO register on the RP1
```

The `volatile` keyword is critical — it tells the compiler *not* to optimize away or reorder this access, because every read/write has a real physical side-effect.

### Why `0x80000`?

The first 512 KB of RAM (addresses `0x0` to `0x7FFFF`) are reserved by the Pi firmware for interrupt vectors, ARM mailboxes, and the Device Tree Blob (DTB). Loading your kernel at `0x80000` keeps it safely above all of that.

### Why doesn't x86 assembly work here?

x86 (Intel/AMD) and ARM64 are completely different instruction set architectures. They use different register names (`eax` vs `x0`), different instruction encodings, and different boot conventions. Code compiled for one simply cannot run on the other.

---

## Building and Running

### 1. Install the cross-compiler

You're compiling ARM64 code on your own machine, so you need a cross-compiler toolchain:

```bash
# macOS
brew install aarch64-elf-gcc

# Ubuntu / Debian
sudo apt install gcc-aarch64-linux-gnu
```

### 2. Compile

```bash
make
```

This produces `kernel8.img` — a raw flat binary ready to run on the Pi.

### 3. Deploy to SD card

1. Format an SD card with a **FAT32** boot partition
2. Copy the official [Raspberry Pi 5 boot firmware files](https://github.com/raspberrypi/firmware/tree/master/boot) onto it
3. Copy your `kernel8.img` to the root of the SD card
4. Insert the SD card into the Pi 5 and power on

The firmware will find `kernel8.img` and jump straight into your code.

---

## What's Happening at the Hardware Level

At the physical level, a `1` is a high voltage (~0.8–1.1V) and a `0` is ground (0V). Inside the BCM2712, billions of MOSFET transistors act as electrically controlled switches. When voltage is applied to a transistor's gate, current flows — that's a logic `1`. Combining transistors into NAND/NOR gates, then into flip-flops, gives you the registers and state machines that make a CPU work.

Every instruction you write ultimately causes specific transistors on the die to switch on or off. Bare-metal programming is as close as software gets to that physical reality.