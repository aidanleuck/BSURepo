#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define ERR(s) err(s, __FILE__, __LINE__)

static void err(char *s, char *file, int line)
{
    fprintf(stderr, "%s:%d: %s\n", file, line, s);
    exit(1);
}

static void printUsage(int scanner)
{
    printf("%s", "Usage:");
    printf("%s", "\n<./TestRunner DEFAULT>");
    printf("%s", "\n<./TestRunner sepCharacters separatorCount>\n");
    close(scanner);
    exit(-1);
}

int main(int argc, char *argv[])
{
    enum {max = 100};
    int sepCount;

    char *separg = argv[1];

    char buf[max + 1];
    int len;

    int scanner = open("/dev/Scanner", O_RDWR);

    if (argc < 2)
    {
        printUsage(scanner);
    }
    else
    {
        if ((strcmp(separg, "DEFAULT")))
        {
            if (argc < 3)
            {
                printUsage(scanner);
            }
            sepCount = atoi(argv[2]);
            if (ioctl(scanner, 0, 0))
                ERR("ioctl() for separators failed");
            if (write(scanner, separg, sepCount) != sepCount)
                ERR("write() of separators failed");
        }

        if (scanner < 0)
            ERR("open() failed");

        char *line;
        while (scanf("%m[^\n]\n", &line) != EOF)
        {
            len = strlen(line);
            if (len != write(scanner, line, len))
                ERR("write() of data failed");
            free(line);
            while ((len = read(scanner, buf, max)) >= 0)
            {
                buf[len] = 0;
                printf("%s%s", buf, (len ? "" : "\n"));
            }
        }

        close(scanner);
        return 0;
    }
}
