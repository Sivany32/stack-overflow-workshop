// Challenge 8: C++ makes it complicated  (observe + bonus)
// Two things to see in gdb:
//   (A) a virtual call dispatches through a VTABLE POINTER stored in the object.
//       If an attacker controls that pointer, they control where the call goes.
//   (B) throwing an exception UNWINDS the stack: frames are torn down as the
//       runtime searches for a handler. Watch the backtrace shrink.
//
//   (gdb) break main
//   (gdb) run
//   (gdb) p a                 <- the object
//   (gdb) x/gx a              <- first 8 bytes = vtable pointer
//   (gdb) info vtbl *a        <- gdb decodes the vtable for you
//   (gdb) break risky
//   (gdb) continue / bt       <- recursion builds frames; the throw unwinds them
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <cstring>

struct Animal {
    virtual void speak() { printf("...\n"); }
    virtual ~Animal() {}
};
struct Dog : Animal {
    void speak() override { printf("woof\n"); }
};

void risky(int depth) {
    char frame_marker[32];
    memset(frame_marker, 'D', sizeof frame_marker);  // something to see per frame
    if (depth > 0) risky(depth - 1);
    else throw std::runtime_error("boom");           // unwinds all the way up
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);

    Animal *a = new Dog();
    a->speak();            // (A) virtual dispatch through the vtable pointer

    try {
        risky(3);          // (B) builds 4 frames, then unwinds them on throw
    } catch (const std::exception &e) {
        printf("caught: %s\n", e.what());
    }
    delete a;
    return 0;
}
