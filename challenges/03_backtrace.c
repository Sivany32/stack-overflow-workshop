// Challenge 3: Who called who?
// Goal: break in c(), run `bt`, then reproduce that backtrace BY HAND by
// following the saved RBP chain (each frame stores the caller's RBP).
//
//   (gdb) break c
//   (gdb) run
//   (gdb) bt                 <- gdb's answer
//   (gdb) x/2gx $rbp         <- [saved rbp][saved rip]; chase saved rbp upward
#include <stdio.h>

void c(void) {
    printf("in c()\n");   // breakpoint here
}
void b(void) { printf("in b()\n"); c(); }
void a(void) { printf("in a()\n"); b(); }

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    a();   // main -> a -> b -> c
    return 0;
}
