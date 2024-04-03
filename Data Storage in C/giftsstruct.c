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

struct Player {
	char name[MAXLEN];
	double balance;
} ;

//struct is like class - we can use an array of struct (we can use like an array of objects).
struct Player players[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, players[i].name) == 0)
               return i;

        return -1;
}

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {
	//code here!
	int length = 0;

	if (argc > 1 && strcmp(argv[1], "new") == 0){
		FILE *filep = fopen("gifts3.dat", "wb");
		for (int i = 2; i < argc; i+=2){
		        strcpy(players[numPlayers].name, argv[i]);
	                players[numPlayers].balance = atof(argv[i+1]);
			printf("%10s: %6.2f\n", players[numPlayers].name, players[numPlayers].balance);
			numPlayers++;
			}
			fwrite(&players, sizeof(struct Player), numPlayers, filep);
		fclose(filep);
	} else {
		FILE *filep = fopen("gifts3.dat", "r");

		while(1){
			if (fread(&players[numPlayers], sizeof(struct Player), 1, filep) == 0){
				break;
			}
			numPlayers++;
		}
		fclose(filep);

		int index = findIndex(argv[1]);
		players[index].balance -= atof(argv[2]);
		double distribute = (atof(argv[2]) / (argc-3));

		for (int i = 3; i <= argc-1; i++){
			players[findIndex(argv[i])].balance += distribute;
		}
		
		FILE *filed = fopen("gifts3.dat", "wb");
		fwrite(&players, sizeof(struct Player), numPlayers, filep);
		fclose(filed);

		for (int i = 0; i < numPlayers; i++)
			printf("%10s: %6.2f\n", players[i].name, players[i].balance);

	}
										               

		

	/*reading data - array of Struct - just one fread()
	FILE *filep = fopen("gifts3.dat", "rb"); //for reading
	numPlayers = fread(players, sizeof(struct Player), MAXPLAYERS, filep);
	fclose(filep);
	//...
	//writing data - array of Struct - just one fwrite()
	filep = fopen("gifts3.dat", "wb"); //for writing
	fwrite(players, sizeof(struct Player), numPlayers, filep);
	fclose(filep);
	*/
}
