// Challenge 6: Defeat the stack canary
// Built with -fstack-protector-all, so a naive overflow dies with
// "*** stack smashing detected ***" before it can return. Two bugs let you
// win cleanly:
//   (1) a LEAK that dumps stack bytes starting at buf -> read the canary
//   (2) an OVERFLOW where you rebuild the payload with the canary in its slot
//
// The canary sits just below the saved RBP (at rbp-8) and its lowest byte is
// 0x00 (so it also stops string-based leaks). It is randomized every run, so
// you must leak THIS run's value and splice it back in.
//
//   payload = b"A"*offset_to_canary + canary + b"B"*8 (saved rbp) + p64(win)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void win(void) {
    puts("[win] Canary preserved, control hijacked!");
    system("/bin/cat flag.txt 2>/dev/null || echo 'flag{canary_bypassed}'");
}

void vuln(void) {
    char buf[64];
    char num[16];
    long n;

    memset(buf, 'A', sizeof buf);

    printf("How many bytes to peek? ");
    fflush(stdout);
    if (!fgets(num, sizeof num, stdin)) return;
    n = strtol(num, NULL, 10);
    if (n < 0) n = 0;
    if (n > 200) n = 200;
    write(1, buf, (size_t)n);   // (1) leak: dumps the canary / saved-rbp / retaddr region

    printf("Your input: ");
    fflush(stdout);
    read(0, buf, 256);          // (2) overflow: keep the canary intact or you die
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin,  NULL, _IONBF, 0);
    vuln();
    return 0;
}
