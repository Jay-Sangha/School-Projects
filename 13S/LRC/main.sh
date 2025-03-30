int num_players =3;

printf("Number of players (3 to 10)? ");
int scanf_result = scanf("%d", &num_players);

if (scanf_result < 1 || num_players < 3 || num_players >10) {
	fprintf(stderr, "Invalid number of players. Using 3 instead. \n");
}

unsigned seed = 4823;

printf("Random-number seed? ");
scanf_results = scanf("%u", &seed);

if (scanf_results < 1) {
	fprintf(stderr, "Invalid seed. Using 4823 instead. \n");
}




