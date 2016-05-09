#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s config-file type\n", argv[0]);
        fprintf(stderr, "type: key, scr\n");
        exit(EXIT_FAILURE);
    }

    Monitor m;
    init_monitor(&m, argv[1], NULL);

    printf("Start monitor\n");

    m.key_state->ready = 0;
    if (!strcmp(argv[2], "scr")) {
        for (;;) {
            system("clear");
            fprintf(stderr, "%s", m.text_buffer);
            sleep(1);
        }
    }
    else if (!strcmp(argv[2], "key")) {
        m.key_state->ready = 0;
        for (;;) {
            while (m.key_state->ready) {}
            printf("inputed %c\n", m.key_state->data);
            printf("input:\n");
            m.key_state->data = getchar();
            m.key_state->ready = 1;
            if (kill(*(m.pid), SIGUSR1) == -1) perror("");
        }
    }
    else {
        fprintf(stderr, "Unknown type '%s'\n", argv[2]);
        exit(EXIT_FAILURE);
    }
}
