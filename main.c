/*
 * 인하대학교 컴퓨터공학과 12191579 김종하
 * 오퍼레이팅시스템 (202101-CSE3206-004)
 * 프로젝트 #1. myshell Program
 * */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

#define COMMAND_MEM 10
#define COMMAND_MAX 1024
#define TOKEN_MAX 512

typedef struct comrec_t comrec_t;   // type to implement a single command
struct comrec_t
{
    char input_command[COMMAND_MAX];
    char* tokenized[TOKEN_MAX];
    bool is_background;
};
void tokenize(comrec_t* self);      // tokenizes an input command
void execute(comrec_t* self);
void execute_bg(comrec_t* self);

int main()
{
    // initialize command line record
    comrec_t command_rec[COMMAND_MEM];
    int circular_cursor_l = 0;
    int circular_cursor_r = 0;

    while (true)
    {
            printf("12191579_shell$ ");
        fgets(command_rec[circular_cursor_r].input_command, COMMAND_MAX - 1, stdin);

        // built-in commands
        if (strcmp(command_rec[circular_cursor_r].input_command, "quit\n") == 0)
        {
            printf("myshell developed by 김종하(12191579)\n");
            break;
        }
        else if (strcmp(command_rec[circular_cursor_r].input_command, "history\n") == 0)
        {
            for (int i = circular_cursor_l; i < COMMAND_MEM; i++)
            {
                printf("%s\n", command_rec[i].input_command);
            }

            if (circular_cursor_l > circular_cursor_r)
            {
                for (int i = 0 ; i < circular_cursor_r ; i++)
                {
                    printf("%s", command_rec[i].input_command);
                }
            }
        }
        else if (strcmp(command_rec[circular_cursor_r].input_command, "help\n") == 0)
        {

        }
        // other commands
        else {
            tokenize(&command_rec[circular_cursor_r]);
            if (command_rec[circular_cursor_r].is_background)
            {
                execute(&command_rec[circular_cursor_r]);
            }
            else
            {
                execute_bg(&command_rec[circular_cursor_r]);
            }
        }

        // command record management
        if (circular_cursor_l < circular_cursor_r)
        {
            if (circular_cursor_r == COMMAND_MEM - 1)
            {
                circular_cursor_r = 0;
                circular_cursor_l = 1;
            }
            else
            {
                circular_cursor_r++;
            }
        }
        else if (circular_cursor_l > circular_cursor_r)
        {
            if (circular_cursor_l == COMMAND_MEM - 1)
            {
                circular_cursor_l = 0;
                circular_cursor_r = COMMAND_MEM - 1;
            }
            else
            {
                circular_cursor_l++;
                circular_cursor_r++;
            }
        }
    }

    return 0;
}

void tokenize(comrec_t* self)
{
    char* org_str = self->input_command;

    for (int i = COMMAND_MAX - 2 ; i >= 1 ; i--)
    {
        if (org_str[i] == '&')
        {
            self->is_background = true;
            org_str[i] = '\0';
            break;
        }
    }

    char* token_str = strtok(org_str, " ");

    int i = 0;
    while (token_str != NULL)
    {
        token_str = strtok(org_str, " ");
        self->tokenized[i++] = token_str;
    }
}

void execute(comrec_t* self)
{
    pid_t pid;
    int cur_state;

    if ((pid = fork()) < 0)
    {
        printf("error during fork");
    }
    else if (pid == 0)
    {
        execvp(self->tokenized[0], self->tokenized);
        exit(1);
    }

    wait(&cur_state);
}

void execute_bg(comrec_t* self)
{
    pid_t pid;
    int bg;

    if ((pid = fork()) < 0)
    {
        printf("error during fork");
    }
    else if (pid == 0)
    {
        bg = open("/dev/null", O_RDONLY);
        dup2(bg, STDOUT_FILENO);
        execvp(self->input_command, self->tokenized);
        exit(0);
    }
}