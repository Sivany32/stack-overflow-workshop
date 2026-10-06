// Challenge 2: Where is the saved return address?
// Goal: in callee(), find the saved return address on the stack and confirm it
// points back into main(), right after the `call callee` instruction.
//
// Terminology check (this trips people up):
//   RIP = instruction pointer (where the CPU executes next)
//   RAX = where a function's return VALUE goes
//   saved return ADDRESS = a value sitting on the stack, pushed by `call`
// There is no "return register" for control flow — the return address lives
// on the stack, which is exactly why overflowing the stack is so powerful.
#include <stdio.h>

void callee(void) {
    long local = 0x1122334455667788;
    // Breakpoint here. Walk up the stack to the saved RIP:
    //   (gdb) info frame        <- gdb tells you "saved rip = ..."
    //   (gdb) x/gx $rbp+8       <- the saved return address by hand
    printf("callee() local at %p = 0x%lx\n", (void*)&local, local);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("main() starts at %p\n", (void*)main);
    callee();   // <-- the saved return address points right AFTER this call
    printf("back in main\n");
    return 0;
}
