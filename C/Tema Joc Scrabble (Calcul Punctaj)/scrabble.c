#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scrabble.h"
#include "print_board.h"

char game_board[15][15];
int vector_punctaj[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

void initializare_game_board()
{
    int i, j;

    for(i=0; i<15; i++)
        for(j=0; j<15; j++)
            game_board[i][j] = '.';
}

void plasare_cuvant(int y, int x, int directie, char cuvant[100])
{
    unsigned i, k=0;

    if(directie == 1)
        for(i=y; i < y+strlen(cuvant)-1; i++)
            game_board[i][x] = cuvant[k++]; //plasare pe verticala
    if(directie == 0)
        for(i=x; i < x+strlen(cuvant)-1; i++)
            game_board[y][i] = cuvant[k++]; //plasare pe orizontala    
}

int punctaj_farabonus(char cuvant[100])
{
    int punctaj = 0;
    unsigned j;

    for(j=0; j < strlen(cuvant)-1; j++)
        punctaj = punctaj + vector_punctaj[cuvant[j] - 'A'];
    
    return punctaj;
}

int punctaj_cu_bonus(int x, int y, int directie, char cuvant[100], char subsir_1[100], char subsir_2[100])
{
    unsigned i, l;

    int punctaj = punctaj_farabonus(cuvant);
    subsir_1[2] = '\0'; //sirul retine si caracterul new_line
    l = strlen(cuvant);

    if(directie == 1)
        for(i=y; i < y+strlen(cuvant)-1; i++)
        {
            if(bonus_board[i][x] == 1)
                if(strstr(cuvant, subsir_1) != NULL) //conditia pentru bonusul 1
                    punctaj = punctaj * 2;

            if(bonus_board[i][x] == 2)
                if(cuvant[l-3] == subsir_2[0] && cuvant[l-2] == subsir_2[1]) //conditia pentru bonusul 2
                    punctaj = punctaj * 3;
        }        

    if(directie == 0)
        for(i=x; i < x+strlen(cuvant)-1; i++)
        {
            if(bonus_board[y][i] == 1)
                if(strstr(cuvant, subsir_1) != NULL)
                    punctaj = punctaj * 2;

            if(bonus_board[y][i] == 2)
                if(cuvant[l-3] == subsir_2[0] && cuvant[l-2] == subsir_2[1])
                    punctaj = punctaj * 3;
        }
    return punctaj;    
}

int verificare_cuvant(int k, char cuvinte_plasate[100][100], int numar_cuvinte)
{
    char cuvant[100];
    int ok=0, l, directie = 2, x, y;
    unsigned i, j, t;

    strcpy(cuvant, words[k]);

    for(l=0; l<numar_cuvinte; l++)
        if(strcmp(cuvant, cuvinte_plasate[l]) == 0) //verifica daca cuvantul nu a fost jucat
            return 0;

    for(i=0; i<15 && directie == 2; i++)
        for(j=0; j<15 && directie == 2; j++)
            if(cuvant[0] == game_board[i][j])
            {
                ok = 1; 
                if(14-j <= strlen(cuvant) - 2) //verifica daca cuvantul are loc pe orizontala
                    ok = 0;
                else
                    for(t=j+1; t <= j+strlen(cuvant)-1; t++)
                        if(game_board[i][t] != '.') //verifica daca cuvantul nu se suprapune cu altele deja jucate
                                ok = 0;
                if(ok == 1)
                    {
                        directie = 0;
                        y = i; 
                        x = j; 
                        ok = 0;
                    }
                else
                    ok = 1;
                
                if(14-i <= strlen(cuvant) - 2) //verifica daca cuvantul are loc pe verticala
                    ok = 0;
                else
                    for(t=i+1; t <= i+strlen(cuvant)-1; t++)
                        if(game_board[t][j] != '.')
                                ok = 0;
                if(ok == 1 && directie == 2)
                    {
                        directie = 1;
                        y = i;
                        x = j;
                    }           
            }
    if(directie == 2)
        return 0;
    else //daca s-a gasit o directie pe care poate fi plasat (verifica conditiile), atunci il plaseaza si returneaza 1
        {
            strcat(cuvant, " ");
            plasare_cuvant(y, x, directie, cuvant);
            return 1;
        }
}

int verif_calcul_punctaj(int k, char cuvinte_plasate[100][100], int numar_cuvinte, char subsir_1[100], char subsir_2[100])
{
    char cuvant[100], cpy_cuvant[100];
    int ok=0, l, directie = 2, x, y, punctaj, x_max, y_max, directie_max, punctaj_max = 0;
    unsigned i, j, t;

    strcpy(cuvant, words[k]);
    strcpy(cpy_cuvant, words[k]);
    strcat(cpy_cuvant, " ");

    for(l=0; l<numar_cuvinte; l++)
        if(strcmp(cuvant, cuvinte_plasate[l]) == 0)
            return 0;

    for(i=0; i<15; i++)
        for(j=0; j<15; j++)
            if(cuvant[0] == game_board[i][j])
            {
                ok = 1; 
                if(14-j <= strlen(cuvant) - 2)
                    ok = 0;
                else
                    for(t=j+1; t <= j+strlen(cuvant)-1; t++)
                        if(game_board[i][t] != '.')
                                ok = 0;
                if(ok == 1)
                    {
                        directie = 0;
                        y = i; 
                        x = j; 
                        ok = 0;
                        if(punctaj_max < punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2))
                        {
                            punctaj_max = punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2);
                            x_max = x;
                            y_max = y;
                            directie_max = directie;
                        }
                    }
                
                ok = 1;
                
                if(14-i <= strlen(cuvant) - 2)
                    ok = 0;
                else
                    for(t=i+1; t <= i+strlen(cuvant)-1; t++)
                        if(game_board[t][j] != '.')
                                ok = 0;
                if(ok == 1)
                    {
                        directie = 1;
                        y = i;
                        x = j;
                        if(punctaj_max < punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2))
                        {
                            punctaj_max = punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2);
                            x_max = x;
                            y_max = y;
                            directie_max = directie;
                        }
                    }           
            }
    if(directie == 2)
        return 0;
    else
        {
            strcat(cuvant, " ");
            punctaj = punctaj_cu_bonus(x_max, y_max, directie_max, cuvant, subsir_1, subsir_2);
            return punctaj;
        }  
}

void plasare_finala(int k, char cuvinte_plasate[100][100], int numar_cuvinte, char subsir_1[100], char subsir_2[100])
{
    char cuvant[100], cpy_cuvant[100];
    int ok=0, l, directie = 2, x, y, x_max, y_max, directie_max, punctaj_max = 0;
    unsigned i, j, t;

    strcpy(cuvant, words[k]);
    strcpy(cpy_cuvant, words[k]);
    strcat(cpy_cuvant, " ");

    for(l=0; l<numar_cuvinte; l++)
        if(strcmp(cuvant, cuvinte_plasate[l]) != 0)
        {
                for(i=0; i<15; i++)
                    for(j=0; j<15; j++)
                        if(cuvant[0] == game_board[i][j])
                        {
                            ok = 1; 
                            if(14-j <= strlen(cuvant) - 2)
                                ok = 0;
                            else
                                for(t=j+1; t <= j+strlen(cuvant)-1; t++)
                                    if(game_board[i][t] != '.')
                                        ok = 0;
                            if(ok == 1)
                            {
                                directie = 0;
                                y = i; 
                                x = j; 
                                ok = 0;
                                if(punctaj_max < punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2))
                                {
                                    punctaj_max = punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2);
                                    x_max = x;
                                    y_max = y;
                                    directie_max = directie;
                                }
                            }
                
                            ok = 1;
                
                            if(14-i <= strlen(cuvant) - 2)
                                ok = 0;
                            else
                                for(t=i+1; t <= i+strlen(cuvant)-1; t++)
                                    if(game_board[t][j] != '.')
                                        ok = 0;
                            if(ok == 1)
                            {
                                directie = 1;
                                y = i;
                                x = j;
                                if(punctaj_max < punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2))
                                {
                                    punctaj_max = punctaj_cu_bonus(x, y, directie, cpy_cuvant, subsir_1, subsir_2);
                                    x_max = x;
                                    y_max = y;
                                    directie_max = directie;
                                }
                            }           
        }
        if(directie != 2)
        {
            plasare_cuvant(y_max, x_max, directie_max, cpy_cuvant);
        }
    }
}

void task1()
{
    initializare_game_board();

    int numar_cuvinte, y, x, i, directie;
    char c_numar_cuvinte[100], c_y[100], c_x[100], c_directie[100], cuvant[100], input_linie[100], *p;

    fgets(c_numar_cuvinte, 100, stdin);
    numar_cuvinte = atoi(c_numar_cuvinte);

    for(i=0; i<numar_cuvinte; i++)
    {
        fgets(input_linie, 100, stdin); //citim linia
        p = strtok(input_linie, " ");

        strcpy(c_y, p);
        p = strtok(NULL, " "); //indicele pe verticala
        strcpy(c_x, p);
        p = strtok(NULL, " "); //indicele pe orizontala
        strcpy(c_directie, p);
        p = strtok(NULL, " "); //directia
        strcpy(cuvant, p); //cuvantul

        y = atoi(c_y);
        x = atoi(c_x);
        directie = atoi(c_directie); //convertim numerele din sir de caractere in (int)

        plasare_cuvant(y, x, directie, cuvant);
    }
    print_board(game_board);
}

void task2()
{
    int numar_cuvinte, i, punctaj_1 = 0, punctaj_2 = 0;
    char c_numar_cuvinte[100], cuvant[100], input_linie[100], *p;
    unsigned j;

    fgets(c_numar_cuvinte, 100, stdin);
    numar_cuvinte = atoi(c_numar_cuvinte);

    for(i=0; i<numar_cuvinte; i++)
    {
        fgets(input_linie, 100, stdin);
        p = strtok(input_linie, " ");
            
        for(j=0; j<3; j++)
            p = strtok(NULL, " "); //nu sunt necesare coordonatele si directia cuvantului

        strcpy(cuvant, p);
            
        if(i % 2 == 1)
            punctaj_2 = punctaj_2 + punctaj_farabonus(cuvant);
        else
            punctaj_1 = punctaj_1 + punctaj_farabonus(cuvant); 
    }

    printf("Player 1: %d Points\n", punctaj_1);
    printf("Player 2: %d Points", punctaj_2);
}

void task3()
{
    int numar_cuvinte, i, x, y, directie, punctaj_1 = 0, punctaj_2 = 0;
    char c_numar_cuvinte[100], cuvant[100], c_y[100], c_x[100], c_directie[100], input_linie[100], *p, subsir_1[100], subsir_2[100];
        
    fgets(subsir_1, 100, stdin); //citire subsiruri pentru bonus
    fgets(subsir_2, 100, stdin);
        
    fgets(c_numar_cuvinte, 100, stdin);
    numar_cuvinte = atoi(c_numar_cuvinte);

    for(i=0; i<numar_cuvinte; i++)
    {
        fgets(input_linie, 100, stdin);
        p = strtok(input_linie, " ");

        strcpy(c_y, p);
        p = strtok(NULL, " ");
        strcpy(c_x, p);
        p = strtok(NULL, " ");
        strcpy(c_directie, p);
        p = strtok(NULL, " ");
        strcpy(cuvant, p);

        y = atoi(c_y);
        x = atoi(c_x);
        directie = atoi(c_directie);
            
        if(i % 2 == 1)
            punctaj_2 = punctaj_2 + punctaj_cu_bonus(x, y, directie, cuvant, subsir_1, subsir_2);
        else
            punctaj_1 = punctaj_1 + punctaj_cu_bonus(x, y, directie, cuvant, subsir_1, subsir_2);
    }
    printf("Player 1: %d Points\n", punctaj_1);
    printf("Player 2: %d Points", punctaj_2);
}

void task4()
{
    initializare_game_board();

    int numar_cuvinte, y, x, i, directie, k = 0, ok = 0, t = 0;
    char c_numar_cuvinte[100], c_y[100], c_x[100], c_directie[100], cuvant[100], input_linie[100], *p, subsir_1[100], subsir_2[100];
    char cuvinte_plasate[100][100];

    fgets(subsir_1, 100, stdin); //citire subsiruri pentru bonus
    fgets(subsir_2, 100, stdin);

    fgets(c_numar_cuvinte, 100, stdin);
    numar_cuvinte = atoi(c_numar_cuvinte);
    

    for(i=0; i<numar_cuvinte; i++)
    {
        fgets(input_linie, 100, stdin);
        p = strtok(input_linie, " ");

        strcpy(c_y, p);
        p = strtok(NULL, " ");
        strcpy(c_x, p);
        p = strtok(NULL, " ");
        strcpy(c_directie, p);
        p = strtok(NULL, " ");
        strcpy(cuvant, p);

        strcpy(cuvinte_plasate[k++], cuvant);
        cuvinte_plasate[k-1][strlen(cuvinte_plasate[k-1])-1] = '\0'; //retinem cuvintele plasate fara caracterul new_line

        y = atoi(c_y);
        x = atoi(c_x);
        directie = atoi(c_directie);

        plasare_cuvant(y, x, directie, cuvant);
    }

    while(ok == 0)
    {
        ok = verificare_cuvant(t, cuvinte_plasate, k);
        t++;
    } 

    print_board(game_board);
}

void task5()
{
    initializare_game_board();
    
    int numar_cuvinte, i, x, y, directie, punctaj_1 = 0, punctaj_2 = 0, t = 0, k = 0;
    char c_numar_cuvinte[100], cuvant[100], c_y[100], c_x[100], c_directie[100], input_linie[100], *p, subsir_1[100], subsir_2[100];
    char cuvinte_plasate[100][100];

    fgets(subsir_1, 100, stdin); //citire subsiruri pentru bonus
    fgets(subsir_2, 100, stdin);
        
    fgets(c_numar_cuvinte, 100, stdin);
    numar_cuvinte = atoi(c_numar_cuvinte);

    for(i=0; i<numar_cuvinte; i++)
    {
        fgets(input_linie, 100, stdin);
        p = strtok(input_linie, " ");

        strcpy(c_y, p);
        p = strtok(NULL, " ");
        strcpy(c_x, p);
        p = strtok(NULL, " ");
        strcpy(c_directie, p);
        p = strtok(NULL, " ");
        strcpy(cuvant, p);

        strcpy(cuvinte_plasate[k++], cuvant);
        cuvinte_plasate[k-1][strlen(cuvinte_plasate[k-1])-1] = '\0';

        y = atoi(c_y);
        x = atoi(c_x);
        directie = atoi(c_directie);

        plasare_cuvant(y, x, directie, cuvant);
            
        if(i % 2 == 1)
            punctaj_2 = punctaj_2 + punctaj_cu_bonus(x, y, directie, cuvant, subsir_1, subsir_2);
        else
            punctaj_1 = punctaj_1 + punctaj_cu_bonus(x, y, directie, cuvant, subsir_1, subsir_2);
    }
        
    int punctaj_cuvant, punctaj_maxim = 0, size_vector_words;
    size_vector_words = sizeof words / sizeof words[0]; //lungimea vectorului words

    for(i=0; i < size_vector_words; i++)
    {
        punctaj_cuvant = verif_calcul_punctaj(i, cuvinte_plasate, k, subsir_1, subsir_2); //retine punctajul maxim al unui cuvant

        if(punctaj_cuvant >= punctaj_maxim)
        {
            punctaj_maxim = punctaj_cuvant;
            t = i; //retine cuvantul cu punctaj maxim
        }
    }
        
    if(punctaj_maxim < punctaj_1 - punctaj_2)
        printf("Fail!");
    else
        {
            plasare_finala(t, cuvinte_plasate, k, subsir_1, subsir_2); //plaseaza cuvantul cu punctaj maxim
            print_board(game_board);
        }
}

int main()
{
    char c_task[100];
    int task;

    fgets(c_task, 100, stdin);
    task = atoi(c_task); //citire numar task

    if(task == 0)
    {
        initializare_game_board();
        print_board(game_board);
    }

    if(task == 1)
        task1();

    if(task == 2)    
        task2();   
    
    if(task == 3)
        task3();

    if(task == 4)
        task4();
    
    if(task == 5)
        task5();

    return 0;
}
