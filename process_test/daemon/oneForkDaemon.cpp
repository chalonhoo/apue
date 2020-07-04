#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>

void oneForkDaemon()
{
    umask(0);
    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(1);
    } else if(pid > 0) {
        //父进程退出
        printf("father, pid: %d, ppid: %d, pgid: %d, sid: %d, %d \n", getpid(), getppid(), getpgid(0), getsid(0), getsid(getpid()));
        exit(0);
    } else {
        //子进程
        printf("\nchild-before setsid, pid: %d, ppid: %d, pgid: %d, sid: %d, %d \n", getpid(), getppid(), getpgid(0), getsid(0), getsid(getpid()));
    }
    setsid();
    printf("child-after setsid, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));

    chdir("/");
    close(0);
    close(1);
    close(2);
    signal(SIGCHLD, SIG_IGN);
}

int main(void)
{
    oneForkDaemon();
    while(1);

    return 0;
}

