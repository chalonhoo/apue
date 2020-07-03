#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    //创建进程组，父进程作为组长进程
    setpgid(getpid(), getpid());

    pid_t pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(1);
    } else if(pid > 0) {
        //将子进程加入到父进程所在的组
        setpgid(pid, getpgid(getpid()));
    } else {
        //将子进程加入到父进程所在的组
        setpgid(getpid(), getpgid(getppid()));
    }

    printf("pid: %d, ppid: %d, pgid: %d\n", getpid(), getppid(), getpgid(0));

    pause();//进程暂停，等待信号

    return 0;
}
