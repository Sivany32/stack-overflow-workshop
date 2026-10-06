// Challenge 1: Find the stack
// Goal: break in secret(), find $rsp, and dump the first few bytes on the stack.
//
//   gdb -q ./bin/01_find_stack
//   (gdb) break secret
//   (gdb) run
//   (gdb) x/5xb $rsp      <- first 5 bytes on the stack
//   (gdb) x/16xg $rsp     <- 16 eight-byte words, the "real stack"
#include <stdio.h>
#include <string.h>

void secret(void) {
    char marker[16];
    memset(marker, 0, sizeof marker);
    // A recognizable pattern you can hunt for in memory.
    strcpy(marker, "ST4CK!");
    // Breakpoint target: look at the stack here, then find `marker`.
    printf("secret() running. marker = %s\n", marker);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    secret();
    return 0;
}
