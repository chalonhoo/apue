#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int twoForkDaemon(void)
{
	pid_t pid = fork();
	
	if( pid != 0 ) 
	{
		//父进程退出
    printf("father, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));
		exit(0);//parent
	}
	printf("\nchild-1-before setsid, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));
	//first children
	if(setsid() == -1)
	{
		printf("setsid failed\n");
		exit(-1);
	}
	printf("child-1-after setsid, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));
	
	umask(0);	
	pid = fork();
	
	if( pid != 0) 
	{
		printf("child-1-exit, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));
		exit(0);
	}
	
	printf("child-2, pid: %d, ppid: %d, pgid: %d, sid: %d \n", getpid(), getppid(), getpgid(0), getsid(0));
	//second children
	chdir ("/");
	
	for (int i = 0; i < 3; i++)
	{
		close (i);
	}
	
	int stdfd = open ("/dev/null", O_RDWR);
	dup2(stdfd, STDOUT_FILENO);
	dup2(stdfd, STDERR_FILENO);
	
	return 0;
}

int main(void)
{
    twoForkDaemon();
    while(1);

    return 0;
}

