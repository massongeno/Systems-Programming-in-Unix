README
Mason Doherty CS 3377 Assignment 6

Prompt -A bunch of friends are coming together to play a game like Monopoly. They all start with the specified initial balance. 
As the game proceeds, in each transaction, one player divides a specific amount of money equally and gives it to specific folks. After each transaction, output the current balance for each player.

We will develop 3 versions of the program, but their functionality will be exactly same. They should keep track of the balance amounts for each player, using text or binary files. 
Program will be run to setup once, then for each transaction, using command line parameters as shown below. Use a text or binary file to keep track of the names and current balances. 

No need to worry about negative balances, etc. Commands are in BOLD. We attach a special meaning to "new" when it appears as the first parameter - that means a fresh start - new set of friends with new balances.
Format the output nicely using format string in printf(). Use 2 decimal places for points, just like real money.



giftstxt.c - 	Uses fopen(), fscanf() and fprintf(). 
	     	Uses text data file gifts1.txt

giftsarray.c -  Uses the system calls open(), write(), read(), & close(). 
		Uses individual arrays for names and amounts. 
		Use a binary data file gifts2.dat

giftsstruct.c - Uses standard I/O library functions fopen(), fread(), fwrite(), & fclose(). 
		Uses an array of structs. 
		Uses a binary data file called gifts3.dat
