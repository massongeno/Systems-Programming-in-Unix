#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//let us assume that there will not be more than 100 players
#define MAXPLAYERS 100
//let us assume that max length for any name is 100 characters
#define MAXLEN 100

//arrays to store the player names and balances
char names[MAXPLAYERS][MAXLEN];
double balances[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, names[i]) == 0)
               return i;

        return -1;
}

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {

	if (argc > 1 && strcmp(argv[1], "new") == 0){
		int fd = open("gifts2.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		int length = 0;
		
		for (int i = 2; i < argc; i+=2){
			strcpy(names[numPlayers], argv[i]);
			balances[numPlayers] = atof(argv[i+1]);
			numPlayers++;
		}
		
		for (int i = 0; i < numPlayers;i++){
			printf("%10s: %6.2f\n", names[i], balances[i]);
		}
		
		for (int i = 0; i < numPlayers; i++){
			length = strlen(names[i])+1;
			write(fd, &length, sizeof(int));
			write(fd, names[i], length);
			write(fd, &balances[i], sizeof(double));
		}
		close(fd);
	} else {
		char name[100];
		double balance;
		int length = 0;
		int fd = open("gifts2.dat", O_RDONLY);

		while (1){
			if (read(fd, &length, sizeof(int)) == 0){
				break;
			}
			read(fd, name, length);
			read(fd, &balance, sizeof(double));
			strcpy(names[numPlayers], name);
			balances[numPlayers] = balance;
			numPlayers++;
		}
		
		int index = findIndex(argv[1]);
		balances[index] -= atof(argv[2]);
		double distribute = (atof(argv[2]) / (argc-3));
		for(int i = 3; i <= argc-1; i++){
                	balances[findIndex(argv[i])] += distribute;
		}
		close(fd);

		int fp = open("gifts2.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);		    
		
		for (int i = 0; i < numPlayers; i++){
			length = strlen(names[i]);
			write(fd, &length, sizeof(int));
			write(fd, names[i], length);
			write(fd, &balances[i], sizeof(double));
		}
		close(fp);
		
		for (int i = 0; i < numPlayers; i++)
			printf("%10s: %6.2f\n", names[i], balances[i]);


	}
	//code here!
	//following line makes sense only for writing to file
	//int fd = open("gifts2.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	//if the command is not "new",
	//int fd = open("gifts2.dat", O_RDONLY);

	//how to write and read individual name?
	//simplest approach is to write the name length first, then the actual content
	//to make reading the name from file easier.
	//write(fd, balances, sizeof(double));
	//close(fd);
}
