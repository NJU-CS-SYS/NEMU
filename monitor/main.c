#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_monitor(Monitor *m, const char *config);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s config-file type", argv[0]);
        exit(EXIT_FAILURE);
    }

    Monitor m;
    init_monitor(&m, argv[1]);

    printf("Start monitor\n");

    m.key_state->ready = 0;
    if (atoi(argv[2])) {
        for (;;) {
            system("clear");
            fprintf(stderr, "%s", m.text_buffer);
            sleep(1);
        }
    }
    else {
        for (;;) {
            printf("wait for consumption\n");
            while (m.key_state->ready) ;
            m.key_state->data = getchar();
            printf("input %c\n", m.key_state->data);
            m.key_state->ready = 1;
        }
    }
}
