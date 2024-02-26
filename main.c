#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>

static volatile int loop = 1;
static volatile pid_t pid = 0;

static void signal_handler(int sig)
{
	switch(sig) {
		case SIGINT:
		case SIGQUIT:
			if (pid)
				kill(pid, SIGKILL);
			loop = 0;
			break;
		default:
			break;
	}
}

static void execute_line(char *line)
{
	char buffer[4096] = {0};
	int status;
	char *argv[32];
	int pfd[2];
	int i = 0;

	argv[i] = strtok(line, " ");
	while (argv[i] != NULL)
		argv[++i] = strtok(NULL, " ");

	argv[++i] = NULL;

	/* create a pipe */
	if (pipe(pfd) == -1) {
		perror("pipe");
		return;
	}

	if ((pid = fork()) == -1)
		perror("fork");
	else if (pid == 0) {
		/* child */

		/* duplicate stdout to writing pipe */
		dup2(pfd[1], STDOUT_FILENO);

		/* close pipes */
		close(pfd[0]);
		close(pfd[1]);

		if (execv(argv[0], &argv[0]) == -1)
			printf("command invalid.\n");

		exit(0);
	} else {
		/* parent */

		/* close writing pipe */
		close(pfd[1]);

		/* read from reading pipe */
		while (read(pfd[0], buffer, sizeof(buffer)))
			printf("%s", buffer);

		waitpid(pid, &status, 0);
		pid = 0;
    }
}

static void line_handler(char *line)
{
	if (line) {
		if (*line) {
			if (strcmp("exit", line) == 0)
				loop = 0;

			add_history(line);

			//printf("%s\n", line);
			execute_line(line);
		} else
			printf("%c", 0x00);

		free(line);
	}
}

int main(int argc, char **argv)
{
	fd_set fds;
	int r;

	setlocale (LC_ALL, "");

	signal(SIGINT, &signal_handler);

	rl_callback_handler_install ("> ", line_handler);

	while (loop)
	{
		FD_ZERO (&fds);
		FD_SET (fileno (rl_instream), &fds);    

		r = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
		if (r < 0 && errno != EINTR) {
			rl_callback_handler_remove();
			break;
		}

		if (r < 0)
			continue;     

		if (FD_ISSET (fileno (rl_instream), &fds))
			rl_callback_read_char();
    }

	rl_callback_handler_remove();

	printf("\n");

	return EXIT_SUCCESS;
}

