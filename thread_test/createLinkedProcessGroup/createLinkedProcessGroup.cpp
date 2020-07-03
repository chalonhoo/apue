#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    //创建进程组1，父进程为组长进程
    setpgid(getpid(), getpid());
    pid_t group1 = getpgid(getpid());

    pid_t pid;
    int i = 0;
    for(;i < 2; i++) {
        pid = fork();
        if(pid < 0) {
            perror("fork error");
            exit(1);
        } else if(pid > 0) {
            if(i == 0) {
                //创建进程组2，第一个子进程作为组长进程
                setpgid(pid, pid);
            }

            if(i == 1) {
                //将第二个子进程加入到 group1中
                //setpgid(pid, group1);
                setpgid(pid, pid);
            }

            // 在进程链中，父进程操作完退出循环
            break;
        } else if(pid == 0) {

            if(i == 0) {
                //创建进程组2，第一个子进程作为组长进程
                setpgid(getpid(), getpid());
            }

            if(i == 1) {
                //将第二个子进程加入到 group1中
                //setpgid(getpid(), group1);
                setpgid(getpid(), getpid());
            }
        }
    }

    printf("pid: %d, ppid: %d, pgid: %d\n", getpid(), getppid(), getpgid(0));

    //防止僵尸进程产生
    for(i = 0; i < 2; i++) {
        wait(0);
    }

    return 0;
}
