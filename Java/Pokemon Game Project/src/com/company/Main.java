package com.company;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) throws FileNotFoundException {

        Scanner s = new Scanner(new FileReader("test10.txt"));
        PokemonFactory factory = PokemonFactory.PokemonFactoryInstance();

        //read first trainer
        Trainer trainer1 = new Trainer(s.next(), s.nextInt());

        //Trainer1 - pokemon1
        Pokemon pokemon1 = (Pokemon)factory.getPokemon(s.next());
        pokemon1.addItem(s.next());
        pokemon1.addItem(s.next());
        pokemon1.addItem(s.next());
        trainer1.pokemonList.add(pokemon1);

        //Trainer1 - pokemon2
        Pokemon pokemon2 = (Pokemon)factory.getPokemon(s.next());
        pokemon2.addItem(s.next());
        pokemon2.addItem(s.next());
        pokemon2.addItem(s.next());
        trainer1.pokemonList.add(pokemon2);

        //Trainer1 - pokemon3
        Pokemon pokemon3 = (Pokemon)factory.getPokemon(s.next());
        pokemon3.addItem(s.next());
        pokemon3.addItem(s.next());
        pokemon3.addItem(s.next());
        trainer1.pokemonList.add(pokemon3);

        //read trainer 2
        Trainer trainer2 = new Trainer(s.next(), s.nextInt());

        //Trainer2 - pokemon 1
        Pokemon pokemon4 = (Pokemon)factory.getPokemon(s.next());
        pokemon4.addItem(s.next());
        pokemon4.addItem(s.next());
        pokemon4.addItem(s.next());
        trainer2.pokemonList.add(pokemon4);

        //Trainer2 - pokemon 2
        Pokemon pokemon5 = (Pokemon)factory.getPokemon(s.next());
        pokemon5.addItem(s.next());
        pokemon5.addItem(s.next());
        pokemon5.addItem(s.next());
        trainer2.pokemonList.add(pokemon5);

        //Trainer2 - pokemon 3
        Pokemon pokemon6 = (Pokemon)factory.getPokemon(s.next());
        pokemon6.addItem(s.next());
        pokemon6.addItem(s.next());
        pokemon6.addItem(s.next());
        trainer2.pokemonList.add(pokemon6);

        Logger logger = Logger.Instance();
        logger.setWriter("test_out.txt");

        //START ULTIMATE ARENA BATTLE
        Arena testArena = new Arena(trainer1, trainer2);
        testArena.startAdventure();
        logger.closeLogger();
    }
}
