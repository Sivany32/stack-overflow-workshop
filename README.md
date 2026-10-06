# Stack Overflow Workshop

Hands-on companion for the talk *"Stack overflow exploitation and how modern
mitigations try to stop it."* You get a real Linux box in the browser with
`gcc`, `gdb` + GEF, and eight challenge binaries — look at real stacks, then
break real mitigations.

## Start here (no install)

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Sivany32/stack-overflow-workshop)

1. Click the badge (edit the URL to your repo), or **Code → Codespaces → Create**.
2. Wait ~1 min for setup to install the toolchain and build everything.
3. In the terminal: `gdb -q ./bin/01_find_stack`

Prefer local? `git clone`, then `make` on any Linux box with gcc/gdb.
On macOS/Windows use the Codespace — the binaries are Linux/x86-64.

## The challenges

Work top to bottom — the difficulty ramps and each one builds on the last.

| # | File | What you do | Mitigation in play |
|---|------|-------------|--------------------|
| 1 | `01_find_stack`     | Find `$rsp`, dump the first bytes of the stack | — orientation |
| 2 | `02_return_address` | Locate the saved return address, prove it points into `main` | — |
| 3 | `03_backtrace`      | `bt`, then walk the saved-RBP chain by hand | — |
| 4 | `04_hexdump`        | Hexdump a buffer, find your own input | — |
| 5 | `05_ret2win`        | Overflow → return into `win()` | NX (bypassed by reusing code) |
| 6 | `06_canary`         | Leak the canary, overflow *keeping it intact* | Stack canary |
| 7 | `07_leaf_O0/_O2`    | Compare prologues; find the frame that isn't there | Leaf opt / red zone |
| 8 | `08_cpp_exception`  | Inspect a vtable pointer; watch an exception unwind | C++ complications |

Each `.c`/`.cpp` file in `challenges/` has the goal and the exact gdb commands
at the top. Flags per challenge live in the `Makefile`.

## gdb cheat sheet

```
break <fn> / b *<addr>     set a breakpoint
run / r                    start (pipe input: printf 'AAAA' | gdb -q ./bin/x)
continue / c, next/n, step/s, finish
info frame                 gdb shows saved rip / saved rbp for this frame
bt                         backtrace (who called who)

x/16xg $rsp                16 words at the stack pointer
x/64bx buf                 64 bytes of a named buffer
x/gx $rbp+8                the saved return address by hand
x/i $rip                   instruction at the program counter
p $rsp   p $rbp   p $rip   registers (RAX = return value, RIP = next instr)

disassemble <fn>           see the prologue / epilogue
info functions win         find a function's address
info vtbl *obj             (C++) decode an object's vtable
```

Stuck on 5, 6, or 8? Progressive hints and full walkthroughs are in
[`SOLUTIONS.md`](SOLUTIONS.md) — try not to peek until you've sat with it.

## Why the addresses don't move

Everything is built `-no-pie`, so code addresses (including `win()`) are fixed —
this takes ASLR out of the picture so you can focus on the overflow itself.
The talk covers what changes when PIE/ASLR, full RELRO, and hardware CFI
(Intel CET shadow stack / ARM PAC+BTI) are turned on.
