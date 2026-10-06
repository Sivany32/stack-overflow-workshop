// Challenge 4: Hexdump your own input
// Goal: send some input, then hexdump the buffer in gdb and find your bytes
// sitting on the stack. This is the mental model you need for every overflow:
// "my input lives HERE, the return address lives THERE, how far apart are they?"
//
//   printf 'AAAABBBBCCCC' | gdb -q ./bin/04_hexdump
//   (gdb) break echo
//   (gdb) run
//   (gdb) next   (a few times, until after read())
//   (gdb) x/64bx buf
#include <stdio.h>
#include <unistd.h>

void echo(void) {
    char buf[64];
    printf("Type something: ");
    ssize_t n = read(0, buf, sizeof buf - 1);
    if (n < 0) n = 0;
    buf[n] = '\0';
    // Breakpoint around here: x/64bx buf  (find your input in the dump)
    printf("you said: %s", buf);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    echo();
    return 0;
}
