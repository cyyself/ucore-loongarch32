#include <ulib.h>
#include <string.h>
#include <stdio.h>
#include <printfmt.h>

#define DEPTH 2

void forktree(const char *cur);

void
forkchild(const char *cur, char branch) {
    char nxt[DEPTH + 1];

    if (strlen(cur) >= DEPTH)
        return;

    snprintf(nxt, DEPTH + 1, "%s%c", cur, branch);
    if (fork() == 0) {
        forktree(nxt);
        yield();
        exit(0);
    }
}

void
forktree(const char *cur) {
    cprintf("%04x: I am '%s'\n", getpid(), cur);

    forkchild(cur, '0');
    forkchild(cur, '1');
}

int
main(void) {
    forktree("");
    return 0;
}

