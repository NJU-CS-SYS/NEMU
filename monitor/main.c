#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void init_monitor(Monitor *m, const char *config);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s config-file type\n", argv[0]);
        fprintf(stderr, "type: key, scr\n");
        exit(EXIT_FAILURE);
    }

    Monitor m;
    init_monitor(&m, argv[1]);

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
        for (;;) {
            printf("wait for consumption\n");
            while (m.key_state->ready) ;
            m.key_state->data = getchar();
            printf("input %c\n", m.key_state->data);
            m.key_state->ready = 1;
        }
    }
    else {
        fprintf(stderr, "Unknown type '%s'\n", argv[2]);
        exit(EXIT_FAILURE);
    }
}
