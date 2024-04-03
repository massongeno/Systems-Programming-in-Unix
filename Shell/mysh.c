#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAXQUOTES 10000
#define MAXLEN 1000

char *commands[1000];
int fds[201][2];
int commandCount = 0;
int numPipes = 0;

//runs a simple command
//cmdname arg1 arg2 arg3 ...
void runCommand(char *command) {
	//split and assemble arguments
	char *args[MAXLEN];
	int index = 0;
	args[index] = strtok(command, " \n");
	while (args[index])
		args[++index] = strtok(NULL, " \n"); //use strtok(..)
	
	execvp(args[0], args);
	fprintf(stderr, "this is bad!\n");
	exit(1);
}

//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 ....
void child(int i) {
	//rewire pipes to 0 and 1
	if (i != 0){	
		close(fds[i-1][1]);
		dup2(fds[i-1][0],0);
		close(fds[i-1][0]);
	}
	if (i != numPipes){	
		close(fds[i][0]);
		dup2(fds[i][1], 1);
		close(fds[i][1]);
	}
	//close unnecessary pipes
	for (int j = 0; j < numPipes; j++){
		close(fds[j][0]);
		close(fds[j][1]);
	}
	//run ith command
	runCommand(commands[i]);
}

void runLoop(char* line) {
	char *command1 = strtok(line, "=");
	char *command2 = strtok(NULL, "\n");

	int toparent[2];
	int tochild[2];
	pipe(toparent);
	pipe(tochild);

	if (fork()) {
		dup2(toparent[0], 0);
		dup2(tochild[1], 1);
		close(toparent[0]); close(toparent[1]);
		close(tochild[0]); close(tochild[1]);
		runCommand(command1);
	} else {
		dup2(tochild[0], 0);
		dup2(toparent[1], 1);
		close(toparent[0]); close(toparent[1]);
		close(tochild[0]); close(tochild[1]);
		runCommand(command2);
	}
}

void processLine(char *line) {
	char *pipePtr = strchr(line, '|'); //does this command have | chars?
	char *equalPtr = strchr(line, '='); //does this command have =?
		

	if (pipePtr) { //not NULL - cmd1 | cmd2 | cmd3 ....
		//command has several sub-commands connected with pipes
		int index = 0;
		//setup commands array
		commands[index]=strtok(line,"|");
		while(commands[index])
			commands[++index] = strtok(NULL, "|");
		//setup pipes array
		numPipes = index-1;
		for (int i = 0; i < numPipes; i++){
			pipe(fds[i]);
		}
		//fork & create children
		for (int i = 0; i < index; i++){
			if (fork() == 0){
				child(i);
			}
		}
	} else if (equalPtr) { //command has = operator
		if (!fork())
			runLoop(line);
		wait(NULL);
    	} else { 
		//it is a simple command, no pipe or = character
		runCommand(line);
	}

	exit(0);
}

int main() {
	char line[MAXLEN];
	char *quotes[MAXQUOTES];
	int numQuotes = 0; 

	FILE *fp = fopen("quotes.txt", "r");
	if (!fp) {
		puts("quotes.txt cannot be opened for reading.");
		exit(1);
	}
	int i = 0;
	
	while (fgets(line,MAXLEN,fp)) {
		quotes[i] = (char *) malloc (strlen(line)+1);
		strcpy(quotes[i], line);
		i++;
	}
	numQuotes = i;
	// infinite loop to serve the customer
	srand(getpid() + time(NULL) + getuid());
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand()%numQuotes], stderr);
		fprintf(stderr, "# "); //show prompt to the user
		//get the user input -- command line
		fgets(line, 1000, stdin);

		//spawn a child for taking care of it
		if (fork() == 0){ 
			processLine(line);
		}
		//wait the child to finish the job!
		wait(NULL);
	}
}
