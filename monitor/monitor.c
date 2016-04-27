#include "monitor.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *create_mapping_file(const char *filename, size_t length)
{
    printf("Create mapping file %s\n", filename);

    int fd;
    while ((fd = open(filename, O_RDWR)) == -1) {
        perror("open");
        fprintf(stderr, "Will creating a new %s\n", filename);
        // see http://bbs.chinaunix.net/thread-1932724-1-1.html
        FILE *fp = fopen(filename, "w");
        fseek(fp, length, SEEK_SET);
        fputc('\0', fp);
        fclose(fp);
    }

    void *map = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap on text buffer");
        exit(EXIT_FAILURE);
    }
    printf("Done\n");

    memset(map, 0, length);
    return map;
}

void init_monitor(Monitor *m, const char *path_to_config)
{
    FILE *config = fopen(path_to_config, "r");
    if (config == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    char screen_map_file[256];
    char key_map_file[256];

    // Parse config file
    {
        char linebuf[256];
        int size_info[4];  // scr_w, scr_h, font_w, font_h
        int idx_scr_w = 0, idx_scr_h = 1, idx_font_w = 2, idx_font_h = 3;
		int i;
        for (i = 0; i < sizeof(size_info) / sizeof(size_info[0]); i++) {
            fgets(linebuf, sizeof(linebuf), config);
            sscanf(linebuf, "%d", &size_info[i]);
        }

        fgets(linebuf, sizeof(linebuf), config);
        sscanf(linebuf, "%s", screen_map_file);

        fgets(linebuf, sizeof(linebuf), config);
        sscanf(linebuf, "%s", key_map_file);

        m->width = size_info[idx_scr_w] / size_info[idx_font_w];
        m->height = size_info[idx_scr_h] / size_info[idx_font_h];

        printf("W: %d H: %d\n", m->width, m->height);
    }

    // Memory mapping
    m->text_buffer = create_mapping_file(screen_map_file, sizeof(m->text_buffer[0]) * (m->width * m->height + 1));
    m->key_state = create_mapping_file(key_map_file, sizeof(*(m->key_state)));
}

