package com.company;

import java.util.ArrayList;
import java.util.List;

//OBSERVER - SUBJECT
public class Trainer{
    private String name;
    private int age;
    public List<Pokemon> pokemonList = new ArrayList<Pokemon>(0);
    //OBSERVER - STATE
    public AttackUsed attackUsed = new AttackUsed();
    //OBSERVER - THE OBSERVER
    private int activePokemon;

    public Trainer(String name, int age) {
        this.name = name;
        this.age = age;
    }

    //constructor for the NPC trainer
    public Trainer() {
        this.name = "NPC";
        this.age = 0;
        this.pokemonList.add((Pokemon)PokemonFactory.PokemonFactoryInstance().getPokemon("Neutrel1"));
        this.pokemonList.add((Pokemon)PokemonFactory.PokemonFactoryInstance().getPokemon("Neutrel2"));
    }

    public String getName() {
        return name;
    }

    //commands an attack for the active pokemon in the fight
    //OBSERVER - SET STATE
    public void commandAttack(){
        attackUsed.typeOfAttack = -1;
        while(attackUsed.typeOfAttack == -1) {
            if(this.name.equals("NPC")) //if npc it just uses normal attack
                this.attackUsed.typeOfAttack = 0;
            else
                this.attackUsed.typeOfAttack = ((int) ((Math.random() * 10) % 4));
            //OBSERVER - NOTIFIES OBSERVER
            battleActivePokemon();
        }
    }

    //OBERSVER - NOTIFIES OBSERVER
    public void battleActivePokemon() {
        //OBSERVER - UPDATE OBSERVER
        pokemonList.get(activePokemon).attack();
    }

    //sets which pokemon is active in the fight
    //OBSERVER - ATTACHES OBSERVER
    public void setActivePokemon(String name){
        for(int i = 0; i < pokemonList.size(); i++) {
            if(pokemonList.get(i).getName().equals(name)){
                activePokemon = i;
                break;
            }
        }
    }

    //finds his best pokemon
    public int findBestPokemon() {
        int bestPokemon = 0;
        for(int i = 1; i < this.pokemonList.size(); i++) {
            if(this.pokemonList.get(bestPokemon).compareTo(this.pokemonList.get(i)) < 0)
                bestPokemon = i;
        }
        return bestPokemon;
    }
}
