#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments: %d", argc);
        fprintf(stderr, "Usage: %s <file> <string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *string_to_write = argv[2];

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        syslog(LOG_ERR, "Error opening file %s", filename);
        return 1;
    }

    fprintf(fp, "%s", string_to_write);
    fclose(fp);

    syslog(LOG_DEBUG, "Writing %s to %s", string_to_write, filename);

    closelog();
    return 0;
}
