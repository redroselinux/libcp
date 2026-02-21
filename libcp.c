#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <libgen.h>

void usage() {
    printf("libcp - copy libraries of a binary into another root\n");
    printf("Why?\n  This program was created for people creating embedded Linux systems\n  that need libraries (sometimes static builds are not possible).\n  For quick prototyping, this is much faster.");
    printf("\n\nUsage:\n  libcp <binary> <root to copy to>");
    printf("\n\nby Redrose Linux - Unlicense\n");
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
        return 2;
    }

    char command[1024];
    snprintf(command, sizeof(command), "ldd %s 2>/dev/null", argv[1]);
    FILE *fp = popen(command, "r");
    if (!fp) return 1;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char lib_path[512] = {0};
        char dummy[512], arrow[10];

        if (sscanf(line, " %s %s %s", dummy, arrow, lib_path) != 3) {
            if (sscanf(line, " %s", lib_path) != 1) continue;
        }

        if (lib_path[0] != '/') continue;

        char dest_path[1024];
        snprintf(dest_path, sizeof(dest_path), "%s%s", argv[2], lib_path);

        char *path_tmp = strdup(dest_path);
        char *dir_path = dirname(path_tmp);
        char build_path[1024] = {0};
        char *p = NULL;

        for (p = dir_path + 1; *p; p++) {
            if (*p == '/') {
                *p = '\0';
                mkdir(dir_path, 0755);
                *p = '/';
            }
        }
        mkdir(dir_path, 0755);
        free(path_tmp);

        FILE *src = fopen(lib_path, "rb");
        if (src) {
            FILE *dst = fopen(dest_path, "wb");
            if (dst) {
                char buffer[8192];
                size_t bytes;
                while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                    fwrite(buffer, 1, bytes, dst);
                }
                fclose(dst);
                printf("%s -> %s\n", lib_path, dest_path);
            }
            fclose(src);
        }
    }

    pclose(fp);
    return 0;
}
