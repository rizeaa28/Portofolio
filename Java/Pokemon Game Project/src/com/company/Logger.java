package com.company;

import java.io.FileWriter;
import java.io.IOException;

public class Logger {
    //output file writer
    FileWriter outputWriter;
    private static Logger unique;

    private Logger(){
    }

    public static Logger Instance() {
        if(unique == null)
            unique = new Logger();
        return unique;
    }

    public void setWriter(String filename) {
        try {
            outputWriter = new FileWriter(filename);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //print any message to standard output and to output file as well
    public void print(String message) {
        System.out.println(message);
        try {
            outputWriter.write(message + "\n");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("eroare");
        }
    }

    public void printWinnerPokemon(Pokemon pokemon) {
        print("Atribute " + pokemon.getName() + " (primeste +1 la toate caracteristicile):");
        print(pokemon.detailsToString());
    }

    public void startOfArena(String name1, String name2) {
        print("Batalia intre " + name1 + " si " + name2 + " a inceput!");
    }

    public void closeLogger(){
        try {
            outputWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void printStartOfFight(Pokemon pokemon1, Pokemon pokemon2) {
        print("Lupta incepe intre: ");
        print(pokemon1.trainer.getName() + " il foloseste in batalie pe " + pokemon1.pokemonStatistics());
        print(pokemon2.trainer.getName() + " il foloseste in batalie pe " + pokemon2.pokemonStatistics());
    }

    public void announceArenaRound(int i) {
        print("\n\n=====================================");
        print("Runda " + i + " din arena a inceput!");
    }

    public void printAbility(String name, int ability){

        switch(ability) {
            case 0:
                print(name + " foloseste atacul clasic.");
                break;
            case 1:
                print(name + " foloseste abilitatea 1.");
                break;
            case 2:
                print(name + " foloseste abilitatea 2.");
                break;
            case 3:
                print(name + " foloseste atacul special.");
                break;
        }
    }

}
