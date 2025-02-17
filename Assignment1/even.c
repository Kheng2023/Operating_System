#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handle_hup_signal(int signum)
{
    printf("Ouch!\n");
}

void handle_int_signal(int signum)
{
    printf("Yeah!\n");
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

    signal(SIGHUP, handle_hup_signal);
    signal(SIGINT, handle_int_signal);

    for (int x = 0; x < n; x++)
    {
        printf("%d\n", x * 2);
        sleep(5);
    }

    return 0;
}
