// Challenge 5: ret2win  (the classic)
// Goal: overflow buf and overwrite the saved return address so that vuln()
// "returns" into win(). Nothing in the program ever calls win() on its own.
//
// No canary, no PIE -> win()'s address is fixed and printed for you.
// NX (non-executable stack) is ON, but you don't need shellcode: you're
// reusing code that already exists. That's the whole point of ret2win.
//
// Find the offset to the return address (hint: pattern or just count),
// then: payload = b"A"*offset + p64(win_addr)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void win(void) {
    puts("[win] You redirected control flow!");
    system("/bin/cat flag.txt 2>/dev/null || echo 'flag{ret2win_demo}'");
}

void vuln(void) {
    char buf[64];
    printf("win() is at %p (no PIE, so it never moves).\n", (void*)win);
    fflush(stdout);
    read(0, buf, 256);   // overflow: 256 bytes into a 64-byte buffer
    printf("buf = %s\n", buf);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    vuln();
    return 0;
}
