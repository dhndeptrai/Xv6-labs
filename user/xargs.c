// user/xargs.c
#include "kernel/types.h"
#include "user/user.h"

#define MAXARG 32
#define MAXLINE 128

int
main(int argc, char *argv[])
{
    int cmd_start = 1;  // vị trí bắt đầu command trong argv

    // Xử lý option -n 1 (bỏ qua)
    if (argc >= 4 && strcmp(argv[1], "-n") == 0 && strcmp(argv[2], "1") == 0) {
        cmd_start = 3;
    }

    if (argc <= cmd_start) {
        fprintf(2, "Usage: xargs [-n 1] command [args...]\n");
        exit(1);
    }

    int base_argc = argc - cmd_start;
    char *cmd_argv[MAXARG];
    for (int i = 0; i < base_argc; i++) {
        cmd_argv[i] = argv[cmd_start + i];
    }

    char line[MAXLINE];
    while (1) {
        int i = 0;
        // Đọc một dòng từ stdin
        while (i < MAXLINE - 1) {
            int n = read(0, &line[i], 1);
            if (n < 1)
                break; // EOF hoặc lỗi
            if (line[i] == '\n')
                break;
            i++;
        }
        if (i == 0)
            break; // không còn dòng nào

        line[i] = 0; // kết thúc chuỗi

        int total_argc = base_argc + 1;
        if (total_argc >= MAXARG) {
            fprintf(2, "Too many arguments\n");
            exit(1);
        }

        cmd_argv[base_argc] = line;
        cmd_argv[total_argc] = 0;

        int pid = fork();
        if (pid < 0) {
            fprintf(2, "fork failed\n");
            exit(1);
        } else if (pid == 0) {
            // child process
            exec(cmd_argv[0], cmd_argv);
            fprintf(2, "exec failed\n");
            exit(1);
        } else {
            // parent process đợi child kết thúc
            wait(0);
        }
    }
    exit(0);
}
