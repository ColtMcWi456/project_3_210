#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

#define MAX_ARGS 20

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	for (int i = 0; i < (sizeof(allowed)/ sizeof(allowed[0])); i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			return 1;
		}
	}
	// return 0 otherwise
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	char token_string[strlen(line)+1];
	strcpy(token_string, line);
	
	char *argv[MAX_ARGS];
	char *token = strtok(token_string, " ");
	int i = 0;

	while(token != NULL && i < MAX_ARGS) {
		argv[i] = token;
		token = strtok(NULL, " ");
		i++;
	}

	argv[i]=NULL;

	// TODO
	char cmd[strlen(argv[0])+1];
	strcpy(cmd, argv[0]);

	if (isAllowed(cmd) == 0) {
		printf("NOT ALLOWED!\n");
		continue;
	}

	int cmd_index = 0;

	for (int i = 0; i < (sizeof(allowed)/ sizeof(allowed[0])); i++) {
		if (strcmp(cmd, allowed[i]) == 0) {
			cmd_index = i;
		}
	}
	// Add code to spawn processes for the first 9 commands
	if (cmd_index < 9) {
		pid_t pid;
		int status;
		posix_spawnattr_t attr;

		// Initialize spawn attributes
		posix_spawnattr_init(&attr);

		// Set flags if needed, for example, to specify the scheduling policy
		// posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

		// Spawn a new process
		if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
			perror("spawn failed");
			exit(EXIT_FAILURE);
		}

		// Wait for the spawned process to terminate
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}

		// Destroy spawn attributes
		posix_spawnattr_destroy(&attr);

		continue;
	}
	// And add code to execute cd, exit, help commands
	else {
		if (cmd_index == 9) {
			if (i > 3) {
				printf("-rsh: cd: too many arguments\n");
				continue;
			} else {
				chdir(argv[1]);
			}
		}
		else if (cmd_index == 10) {
			return 0;
		} 
		else {
			printf("The allowed commands are:\n");
			for (int i = 0; i < (sizeof(allowed)/ sizeof(allowed[0])); i++) {
				printf("%d: %s\n", i+1, allowed[i]);
			}
		}
	}
	// Use the example provided in myspawn.c

    }
    return 0;
}
