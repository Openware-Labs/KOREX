# KOREX

An independent x86_64 operating system: two-stage BIOS bootloader, long-mode C kernel, no libc, no external runtime.

## Layout

```
arch/x86_64/
  boot/            stage 1 (MBR) and stage 2 (real mode -> long mode)
  kernel/          head, IDT, PIC, IRQ dispatch, CPUID, ISR stubs
  include/asm/     architecture-specific headers
  kernel.ld        linker script
include/korex/     portable kernel headers
init/              start_kernel()
kernel/            console, printk, panic, tty, version
drivers/
  video/           VGA text console
  input/           PS/2 keyboard
  sound/           PC speaker
  timer/           PIT (system tick, msleep)
lib/               string and vsnprintf
usr/
  include/usr/     shell and program headers
  shell/           command parsing and command table
  bin/             one file per program
```

Rule of thumb: code outside `arch/` never touches ports or registers directly, and `drivers/` talk to the rest of the kernel only through `console`, `tty` and `irq_register`.

## Boot flow

1. BIOS loads stage 1 (LBA 0) at 0x7C00; it loads stage 2 (LBA 1, 4 sectors) at 0x1000.
2. Stage 2 loads the kernel (LBA 9, 64 sectors) at 0x10000, enters protected mode, builds identity paging (1 GiB), enables long mode and copies the kernel to 0x100000.
3. `_start` (`arch/x86_64/kernel/head.c`) clears `.bss` and calls `start_kernel()`.
4. `start_kernel()` brings up the console, architecture (IDT + PIC), timer, keyboard, tty and shell, then idles.

## Input path

```
keyboard IRQ -> tty_push() (ring buffer)
main loop    -> tty_poll() -> line editing -> shell_execute()
```

Commands run in process context with interrupts enabled; the IRQ handler only enqueues the character.

## Build

```
make            build build/korex.img
make run        boot in QEMU
make debug      boot paused, GDB stub on :1234
make V=1        show full commands
make WERROR=1   treat warnings as errors
make clean
```

Requires `gcc`, `ld`, `objcopy`, `nasm`, `dd` and (for `run`) `qemu-system-x86_64`. For PC speaker audio pass your audio backend through `QEMUFLAGS`, for example `QEMUFLAGS="-audiodev pa,id=snd0 -machine pcspk-audiodev=snd0"`.

The build fails if stage 2 exceeds 2048 bytes or the kernel exceeds 32768 bytes, because the bootloaders read fixed sector counts.

## Adding a shell command

1. Create `usr/bin/<name>.c` with `int cmd_<name>(int argc, char** argv)`.
2. Declare it in `usr/include/usr/bin.h`.
3. Add one line to the table in `usr/shell/commands.c`.

## Adding a driver

1. Put it under `drivers/<class>/`.
2. Expose an `<name>_init()` in `include/korex/<name>.h` and call it from `start_kernel()`.
3. Hook hardware interrupts with `irq_register()` and keep the handler short.
