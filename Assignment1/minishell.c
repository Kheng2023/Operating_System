/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define NV 20           /* max number of command tokens */
#define NL 100          /* input buffer size */
char line[NL];          /* command input buffer */

typedef struct
{  /*create a struct(similar to class in python) to store the corresponding process ID,
    number and command name for corresponding background process*/
    pid_t pid;          //Store the background process ID
    int number;         //Store the background process number
    char command[100];  // Store the background process command name
} BackgroundProcess;

typedef struct
{
    int number;         // Store the completed background process number
    pid_t pid;          // Store the completed background process ID
    char command[100];  // Store the completed background process command name
} CompletedBackgroundProcess;

CompletedBackgroundProcess completed_bg[NV];
BackgroundProcess bg_process[NV];
int completed_bg_count = 0; // Initialize the counter for completed background processes
int bg_count = 0; // Initialize the counter for background processes

void sigchld_handler(int signo)
{/* a handler that help to keep track of background processes,
also enables the shell to continue to do other task */
    int status;
    pid_t pid;

    //Catch the signal of completed background process
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i <= bg_count; i++)
        {
            if (bg_process[i].pid == pid) //Match the completed process pid with the ones kept in record in struct bg_process
            {
                // Store completed background process information
                completed_bg[completed_bg_count].number = bg_process[i].number;
                completed_bg[completed_bg_count].pid = pid;
                snprintf(completed_bg[completed_bg_count].command, sizeof(completed_bg[completed_bg_count].command), "%s", bg_process[i].command);
                bg_process[i].pid = 0; // Clear the ID as the background process is completed
                completed_bg_count++;
                break;
            }
        }
    }

    // Check if all background processes have completed
    int all_completed = 1;
    for (int i = 0; i <= bg_count; i++)
    {
        if (bg_process[i].pid != 0) // !=0 means there's still background process running
        {
            all_completed = 0;
            break;
        }
    }

    // If all background processes completed, reset the counter
    if (all_completed)
    {
        bg_count = 0;
    }
}

/*
    shell prompt
 */

void prompt(void)
{
    //fprintf(stdout, "\n msh> ");

    //Print out all completed background processes
    for (int i = 0; i < completed_bg_count; i++)
        {
            printf("[%d]+ Done\t\t\t%s\n", completed_bg[i].number, completed_bg[i].command);
        }
        completed_bg_count = 0; //reset the counter
    fflush(stdout);
}

int main(int argk, char *argv[], char *envp[])
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
{
    int frkRtnVal;    /* value returned by fork sys call */
    char *v[NV];    /* array of pointers to command line tokens */
    char *sep = " \t\n";/* command line token separators    */
    int i;        /* parse index */

    // Install SIGCHLD signal handler
    //signal(SIGCHLD, sigchld_handler);

    /* prompt for and process one command line at a time  */

    while (1)
    {       /* do Forever */
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        signal(SIGCHLD, sigchld_handler);

        if (feof(stdin))
        {       /* non-zero on EOF  */
            exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
            continue;            /* to prompt */

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++)
        {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL)
                break;
        }
        /* assert i is number of tokens + 1 */

        int is_background = 0; // Flag to indicate background process
        if (v[i - 1] != NULL && strcmp(v[i - 1], "&") == 0)
        {
            is_background = 1;
            v[i - 1] = NULL; // Remove "&" from the command tokens
            i--;
        }

        if (strcmp(v[0], "cd") == 0)
            {
            if (v[1] != NULL)
                {
                if (chdir(v[1]) != 0)
                    {
                    perror("cd");
                    }
                }
            }
        else
        {
        /* fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork())
            {
            case -1: /* fork returns error to parent process */
                perror("fork");
                exit(1);

            case 0: /* code executed only by child process */
                execvp(v[0], v);
                perror("execvp");
                exit(1);

            default: /* code executed only by parent process */
            if (is_background)
                {
                bg_count++; // Increment the counter for the next background process
                printf("[%d] %d\n", bg_count, frkRtnVal); // [x] pid
                bg_process[bg_count].pid = frkRtnVal;
                bg_process[bg_count].number = bg_count;
                // Construct the command string in the BackgroundProcess struct
                snprintf(bg_process[bg_count].command, sizeof(bg_process[bg_count].command), "%s", v[0]);
                for (int j = 1; j < i; j++)
                    {
                    strcat(bg_process[bg_count].command, " ");
                    strcat(bg_process[bg_count].command, v[j]);
                    }
                }
            else
                {
                int status;
                waitpid(frkRtnVal, &status, 0); // Wait for foreground child process
                }
            break;
                }    /* switch */
            } /*else*/
    }    /* while */
    return 0;
}    /* main */
