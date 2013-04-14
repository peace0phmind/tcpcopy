
#include <xcopy.h>

int
daemonize(char * pidFile)
{
    int fd;
    char buf[32] = {'\0'};
    int pid_fd = -1;
    pid_t pidt;

    switch (fork()) {
        case -1:
            return (-1);
        case 0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }
    if (setsid() == -1) {
        return (-1);
    }

    if (pidFile != NULL){
        pid_fd = open(pidFile,O_RDWR|O_CREAT|O_TRUNC, 0644);
        if (pid_fd != -1){
            pidt = getpid();

            sprintf(buf, "%d", pidt);

            write(pid_fd, buf, strlen(buf));

            close(pid_fd);
        }
    }


    if (chdir("/") != 0) {
        perror("chdir");
        return (-1);
    }

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 stdin");
            return (-1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 stdout");
            return (-1);
        }
        if (dup2(fd, STDERR_FILENO) < 0) {
            perror("dup2 stderr");
            return (-1);
        }

        if (fd > STDERR_FILENO) {
            if (close(fd) < 0) {
                perror("close");
                return (-1);
            }
        }
    }
    return (0);
}

