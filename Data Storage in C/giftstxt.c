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

int main(int argc, char *argv[]) {

	if (argc > 1 && strcmp(argv[1], "new") == 0){
		FILE *filep = fopen("gifts1.txt", "w");
		for (int i = 2; i < argc; i+=2)
			fprintf(filep, "%s %lf\n", argv[i], atof(argv[i+1]));
		
		int count = 0;
		fclose(filep);
		FILE *fp = fopen("gifts1.txt", "r");
		while (fscanf(filep, "%s %lf", names[count], &balances[count]) == 2)
		{
			count++;
		}
		
		for (int i = 0; i < count; i++)
			printf("%10s: %6.2f\n", names[i], balances[i]);	
		
		fclose(fp);
	} else {
		FILE *filep = fopen("gifts1.txt", "r");
		
		int count = 0;
		while (fscanf(filep, "%s %lf", names[count], &balances[count]) == 2)
		{
			count++;
		}
		
		numPlayers = count;
		int index = findIndex(argv[1]);
		balances[index] -= atof(argv[2]);
		double distribute = (atof(argv[2]) / (argc-3));
		
		for(int i = 3; i <= argc-1; i++){
			balances[findIndex(argv[i])] += distribute;
		}
		fclose(filep);
		FILE *fp = fopen("gifts1.txt", "w");
		for (int i = 0; i < numPlayers; i++)
			fprintf(filep, "%s %lf\n", names[i], balances[i]);
		fclose(fp);

		for (int i = 0; i < numPlayers; i++)
			printf("%10s: %6.2f\n", names[i], balances[i]);
		
	}

		

	//code here! use the following code just as reference 
	/*
	FILE *filep = fopen("gifts1.txt", "w");
	int n = 5; //let us assume players
	fprintf(filep, "%s %lf\n", names[0], balances[0]);
	fclose(filep);

	//when reading text data, we may not know how many players
	//you can read the following in a loop
	if (fscanf("%s%lf", names[i], &balances[i]) < 2)
		break;
	*
	*/
}
