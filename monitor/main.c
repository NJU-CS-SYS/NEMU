#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_monitor(Monitor *m, const char *config);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s config-file", argv[0]);
        exit(EXIT_FAILURE);
    }

    Monitor m;
    init_monitor(&m, argv[1]);

    printf("Start monitor\n");

    for (;;) {
        system("clear");
        fprintf(stderr, "%s", m.text_buffer);
        sleep(1);
    }
}
