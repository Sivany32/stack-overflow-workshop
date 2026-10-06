// Challenge 7: Leaf functions & the red zone  (observe, don't exploit)
// Built TWICE: bin/07_leaf_O0 (no optimization) and bin/07_leaf_O2 (optimized).
// A "leaf" function calls nothing else. At -O2 the compiler often skips the
// usual frame setup (push rbp / mov rbp,rsp) and uses the 128-byte "red zone"
// below RSP instead of adjusting the stack pointer at all.
//
// Goal: disassemble leaf() in both builds and compare the prologue.
//   gdb -q ./bin/07_leaf_O0 -ex 'disassemble leaf' -ex quit
//   gdb -q ./bin/07_leaf_O2 -ex 'disassemble leaf' -ex quit
// Why it matters: "where is the frame?" is not always obvious, and that
// changes how you reason about overflows and where saved registers live.
#include <stdio.h>

// 'noinline' so it still exists as its own function at -O2.
__attribute__((noinline))
long leaf(long x) {
    long tmp = x * 3 + 7;    // uses locals, calls nothing -> it's a leaf
    return tmp ^ 0x5a5a;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    volatile long v = 12345;
    printf("leaf(%ld) = %ld\n", v, leaf(v));
    return 0;
}
