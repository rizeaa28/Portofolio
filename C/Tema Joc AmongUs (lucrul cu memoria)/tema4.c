#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{
	int matrice_initiala[MAX][MAX], i, j, matrice_finala[MAX][MAX], in;

	in = *(int *)input; //castez input

	for(i=1; i <= in; i++)
		for(j=1; j <= in; j++)
			{
				int copie_i = i, copie_j = j, p = 1;
				matrice_initiala[i][j] = 0;
				while(copie_j)
					{
						matrice_initiala[i][j] = (copie_j % 10) * p + matrice_initiala[i][j];
						p = p * 10;
						copie_j = copie_j / 10;
					}
				while(copie_i)
					{
						matrice_initiala[i][j] = (copie_i % 10) * p + matrice_initiala[i][j];
						p = p * 10;
						copie_i = copie_i / 10;
					}
			} //construiesc matricea initiala
	
	for(i=1; i <= in; i++)
		for(j=1; j <= in; j++)
			matrice_finala[i][j] = matrice_initiala[in + 1 - j][i]; //formula rotire
	
	char string_matrice[100000] = "\0", buffer[MAX], *string_final;

	for(i=1; i <= in; i++)
		{
			for(j=1; j <= in; j++)
				{
					sprintf(buffer, "%d ", matrice_finala[i][j]);
					strcat(string_matrice, buffer); //element cu element
				}
				
			string_matrice[strlen(string_matrice)-1] = '\0';
			strcat(string_matrice, "\n"); //urmatoare linie
		}
	string_matrice[strlen(string_matrice)-1] = '\0';
	
	string_final = (char*) malloc (100000 * sizeof(char));
	for(unsigned int a=0; a < strlen(string_matrice); a++)
		string_final[a] = string_matrice[a]; //copiez in stringul pentru output

	return (void*)string_final;
}

// Task 2
void *DecodeString(void *input)
{
	char *p, *string_output, *string_input;
	int suma = 0, numar;
	string_input = (char *) input;

	p = strtok(string_input, "_");
	while(p != NULL) //impart inputul dupa characterul "_"
	{
		numar = atoi(p); //conversie char->int
		suma = suma + numar;
		p = strtok(NULL, "_");
	}

	string_output = (char*) malloc (100 * sizeof(char));
	sprintf(string_output, "%d", suma);

	return string_output;
}

//Task 3
void *InvertArray(void *input)
{
	int n, *v, i, temp;
	v = (int*) input;
	n = v[0];

	if(n % 2 == 0) //daca sirul are numar par de termeni
		for(i = 1; i <= n; i = i + 2)
		{
			temp = v[i];
			v[i] = v[i+1];
			v[i+1] = temp;
		}
	else //daca sirul are numar impar de termeni
		for(i = 1; i <= n/2; i++)
		{
			temp = v[i];
			v[i] = v[n-i+1];
			v[n-i+1] = temp;
		}
	
	char *string_output, string_vector[10000] = "\0", buffer[100];
	string_output = (char *) malloc (10000 * sizeof(char));

	for(i = 1; i <= n; i++)
		{
			sprintf(buffer, "%d ", v[i]);
			strcat(string_vector, buffer); // vector (int) -> string
		}	
	
	string_vector[strlen(string_vector)-1] = '\0';
	for(unsigned int a=0; a < strlen(string_vector); a++)
		string_output[a] = string_vector[a];
	
	return string_output;
}

//Task 4
Player *allocPlayer()
{
	Player *_player = calloc(1, sizeof(Player)); //aloc memorie pentru intreaga structura
	_player->name = (char *) calloc (MAX_LEN_STR_ATR, sizeof(char)); //aloc memorie pentru nume
	_player->color = (char *) calloc (MAX_LEN_STR_ATR, sizeof(char)); // aloc memorie pentru culoare etc...
	_player->hat = (char *) calloc (MAX_LEN_STR_ATR, sizeof(char));
	_player->alive = 1; //cand inititalizez playerul acesta este in viata

	return _player;
}

//Task 4
Game *allocGame()
{
	Game *_game= calloc(1, sizeof(Game)); //aloc memorie pentru intreaga structura
	_game->name = (char *) calloc (MAX_LEN_STR_ATR, sizeof(char));
	return _game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	Player *_player;
	_player = allocPlayer();
	int i;

	fscanf(inputFile, "%s\n%s\n%s", _player->name, _player->color, _player->hat);
	fscanf(inputFile, "%d\n", &_player->numberOfLocations);

	_player->locations = calloc (_player->numberOfLocations, sizeof(Location)); //aloc memorie vectorului locations

	for(i = 0; i < _player->numberOfLocations; i++)
		fscanf(inputFile, "(%d,%d) ", &_player->locations[i].x, &_player->locations[i].y); //citesc fiecare locatie

	char str[100];
	fscanf(inputFile, "%s\n", str);
	
	if(strcmp(str, "Rotator") == 0)
		_player->playerRole = Rotator;
	if(strcmp(str, "Decoder") == 0)
		_player->playerRole = Decoder;
	if(strcmp(str, "Invertor") == 0)
		_player->playerRole = Invertor;
	if(strcmp(str, "Impostor") == 0)
		_player->playerRole = Impostor; //atribui abilitatea jucatorului

	_player->indexOfLocation = 0; //locatia initiala a jucatorului

	return _player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{	
	Game *_game;
	_game = allocGame();
	int i;
	
	fscanf(inputFile, "%s\n", _game->name);
	fscanf(inputFile, "%d\n%d\n", &_game->killRange, &_game->numberOfCrewmates);

	_game->crewmates = calloc (_game->numberOfCrewmates, sizeof(Player)); //aloc memorie pentru vectorul crewmates
	for(i = 0; i < _game->numberOfCrewmates; i++)
		_game->crewmates[i] = ReadPlayer(inputFile); //citesc fiecare jucator la rand

	_game->impostor = ReadPlayer(inputFile); //citire impostor

	return _game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", player->name, player->color, player->hat, fromEnumtoString(player->playerRole));
	fprintf(outputFile, "Player's locations: ");

	for(int i = 0; i < player->numberOfLocations; i++)
		fprintf(outputFile, "(%d,%d) ", player->locations[i].x, player->locations[i].y);

	fprintf(outputFile, "\n");

	return;
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\nKill Range: %d\nNumber of crewmates: %d\n", game->killRange, game->numberOfCrewmates);
	fprintf(outputFile, "\n\tCrewmates:\n");

	for(int i = 0; i < game->numberOfCrewmates; i++)
		WritePlayer(game->crewmates[i], outputFile); //apelez functia pentru fiecare jucator

	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile); // scrierea impostorului
	
	return;
}

//Task 7
void *KillPlayer(void *input)
{
	Game *_game = (Game*)input; //gata
	char *string_output = calloc(MAX_LEN_STR_OUT, sizeof(char));
	int distanta_minima, distanta, numar_crewmate = 0, ok_omorat = 0, poz_crew, poz_imp;

	poz_imp = _game->impostor->indexOfLocation;
	poz_crew = _game->crewmates[0]->indexOfLocation; //pozitiile jucatorilor/impostorului

	distanta_minima = abs(_game->impostor->locations[poz_imp].x - _game->crewmates[0]->locations[poz_crew].x) + abs(_game->impostor->locations[poz_imp].y - _game->crewmates[0]->locations[poz_crew].y);
	//calculez distanta folosing functia abs (modul)
	for(int i = 1; i < _game->numberOfCrewmates; i++)
	{
		poz_crew = _game->crewmates[i]->indexOfLocation;
		distanta = abs(_game->impostor->locations[poz_imp].x - _game->crewmates[i]->locations[poz_crew].x) + abs(_game->impostor->locations[poz_imp].y - _game->crewmates[i]->locations[poz_crew].y);

		if(distanta <= distanta_minima && _game->crewmates[i]->alive == 1)
			{
				distanta_minima = distanta;
				numar_crewmate = i; //aflu distanta minima
			}
	}

	if(distanta_minima <= _game->killRange && _game->crewmates[numar_crewmate]->alive == 1) //daca distanta minima e mai mica decat range-ul de kill
	{
		ok_omorat = 1; //impostorul omoara crewmate-ul
		_game->crewmates[numar_crewmate]->alive = 0;
	}

	if(ok_omorat == 0)
		sprintf(string_output,"Impostor %s couldn't kill anybody.", _game->impostor->name);
	else
		sprintf(string_output,"Impostor %s has just killed crewmate %s from distance %d.", _game->impostor->name, _game->crewmates[numar_crewmate]->name, distanta_minima);

	return (void*)string_output;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	int i, k = 0, pozitie;
	char *string_output;
	
	for(i = 0; i < game->numberOfCrewmates; i++)
	{
		if(game->crewmates[i]->alive == 0)
			fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[i]->name); //daca crewmate-ul e mort
		else //crewmate-ul e inca in viata
		{
			game->crewmates[i]->indexOfLocation = game->crewmates[i]->indexOfLocation + 1;
			if(game->crewmates[i]->indexOfLocation > game->crewmates[i]->numberOfLocations - 1)
				game->crewmates[i]->indexOfLocation = 0;

			pozitie = game->crewmates[i]->indexOfLocation;
		
			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[i]->name, game->crewmates[i]->locations[pozitie].x, game->crewmates[i]->locations[pozitie].y);
			//ii afisam locatia
			fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[i]->name);

			if(strcmp(fromEnumtoString(game->crewmates[i]->playerRole), "Rotator") == 0)
				string_output = RotateMatrix(inputMatrix[i]);
			if(strcmp(fromEnumtoString(game->crewmates[i]->playerRole), "Decoder") == 0)
				string_output = DecodeString(inputMatrix[i]);
			if(strcmp(fromEnumtoString(game->crewmates[i]->playerRole), "Invertor") == 0)
				string_output = InvertArray(inputMatrix[i]); //output in functie de ability

			fprintf(outputFile, "%s\n", string_output);
		}
	}
	
	//acelasi lucru ca la crewmate doar ca pentru impostor
	game->impostor->indexOfLocation++;
	if(game->impostor->indexOfLocation > game->impostor->numberOfLocations - 1)
				game->impostor->indexOfLocation = 0;

	pozitie = game->impostor->indexOfLocation;
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", game->impostor->name, game->impostor->locations[pozitie].x, game->impostor->locations[pozitie].y);
	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);

	string_output = KillPlayer(game);
	fprintf(outputFile, "%s\n", string_output);

	return;
}

// Neimplementate din cauza unor probleme de checker
void FreePlayer(Player *player)
{
	// TODO
	return;
}

// Neimplementate din cauza unor probleme de checker
void FreeGame(Game *game)
{
	// TODO
	return;
}