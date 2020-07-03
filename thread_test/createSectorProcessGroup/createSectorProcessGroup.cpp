#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{

    //创建进程组，组长进程为父进程
    //由于从shell启动会创建一个单独的进程组，个人认为这里可以不用设置也可以
    //setpgid(getpid(), getpid());

    /* 组1 */
    // 或 pid_t group1 = getpgid(0);
    pid_t group1 = getpgid(getpid());
    pid_t group2;

    int i = 0;
    pid_t pid;

    for(; i < 3; i++) {
        pid = fork();
        if(pid < 0) {
            perror("fork error`");
            exit(1);
        } else if(pid > 0) {//父进程
            //父进程中执行和子进程相同的操作
            if(i == 0) {
                //要注意fork 在父进程中返回的是子进程的 pid
                setpgid(pid, group1);
            }

            //第二个子进程作为组长进程，要创建进程组
            if(i == 1) {
                setpgid(pid, pid);
                group2 = getpgid(pid);
            }

            if(i == 2) {
                //第三个子进程加入到 group2
                setpgid(pid, group2);
            }
        } else {//子进程
            //将第一个子进程加入到 group1
            if(i == 0) {
                setpgid(getpid(), group1);
            }

            //第二个子进程作为组长进程，要创建进程组
            if(i == 1){
                setpgid(getpid(), getpid());
                group2 = getpgid(getpid());
            }

            if(i == 2) {
                setpgid(getpid(), group2);
            }

            //因为是进程扇，每一个子进程要退出循环
            //父进程继续循环 fork
            break;
        }
    }

    printf("pid: %d, ppid: %d, pgid: %d\n", getpid(), getppid(), getpgid(0));

    //防止僵尸进程产生
    for(i = 0; i < 3; i++) {
        wait(0);
    }

    exit(0);
}
