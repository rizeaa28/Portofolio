#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = 0;

    /* TODO */

    int i, gasit=0, nr=0, mask;
    mask = 65535;  //masca cu 16 biti cu valoarea 1

    for(i=0; i<64 && gasit==0; i++)
    {
        if((memory & (1ULL<<i)) !=0)
            nr++;
        else
            nr=0;

        if(nr == 5)
            gasit = i; //retinem pozitia unde sirul de 5 de biti cu valoarea 1 se incheie
    }

    res = (res | ((memory>>(gasit+1)) & mask)); //valoarea res preia valoarea celor 16 biti urmatori

    return res;
}


uint16_t find_key(uint64_t memory)
{
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = 0;

    /* TODO */

    int i, gasit=0, nr=0, mask;
    mask = 65535;

    for(i=0; i<64 && gasit==0; i++)
    {
        if((memory & (1ULL<<i)) !=0)
            nr++;
        else
            nr=0;

        if(nr == 3)
            gasit = i; //retinem pozitia grupului de 3 biti cu valoarea 1
    }

    res = (res | ((memory>>(gasit-18)) & mask)); 

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = 0;

    /* TODO */

    res = (spell ^ key);

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    uint32_t res = 0;

    /* TODO */

    int value, i, nr=0;

    for(i=0; i<16; i++)
    {
        if((enemy & (1<<i))!=0)
            nr++; //retinem cati biti au valoarea 1
    }

    if(nr%2 == 0)
        res = 9; //daca e monstru
    else 
        res = 6; //daca e om
    
    res = (res<<28);

    if(nr%2 == 0)
        value = (enemy & (1 + (~(enemy - 1)))); //daca e monstru (a ^ b = 0 => a = b)
    else
        {
            value = (~(enemy)) + 1;
            value = (value & 65535); //daca e om ( (-a) = 2^16 - a; pe biti)
        }

    res = (res | value);

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */

    uint32_t res = 0;

    /* TODO */

    res = (cocktail << 16); //antibodies_high = cocktail si antibodies_low = 0 verifica ecuatiile

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = 3;

    /* TODO */

    int i, nr=0, patru=0, gasit=0;

    for(i=0; i<64; i++)
    {
        if((map & (1ULL<<i))!=0)
        {
            nr++; //retinem numarul de copaci
            if(patru != -1)
                gasit++; //retinem numaru de copaci din grupul actual
        }
        else
        {
            if(gasit == 4)
                {
                    patru++; 
                    gasit = 0; 
                }
            else
                if(gasit!=0)
                    patru = -1;
        }
    }
    
    //retinem in variabila "patru" numarul de grupuri de 4 copaci sau -1 daca un macar grup nu este de 4 copaci
    
    if(gasit == 4)
                patru++;
            else
                if(gasit!=0)
                    patru = -1; //am repetat verificarea pentru ultimul grup de biti 
   
    if(patru >= 0 || nr == 0)
        res = 0; 
    
    if(nr == 64)
        res = 2;
    
    if(map == (3ULL << 31))
        res = 1;

    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1;

    /* TODO */

    int nr=0, i, gasit=0;

    for(i=0; i<32 && gasit<2; i++)
    {
        if((map & (1ULL<<i))!=0)
        {
            gasit++;
        }
        
        if(gasit == 1)
            nr++; //daca am gasit primul bit de valoarea 1, incepem sa numaram pasii
    }

    res = nr;

    return res;
}
