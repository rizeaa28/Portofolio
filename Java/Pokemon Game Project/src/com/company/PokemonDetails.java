package com.company;

public class PokemonDetails{
    protected int HP = 0, attack = 0, specialAttack = 0, def = 0, specialDef = 0;

    public void setHP(int HP) {
        this.HP = HP;
    }

    public void setAttack(int attack) {
        this.attack = attack;
    }

    public void setSpecialAttack(int specialAttack) {
        this.specialAttack = specialAttack;
    }

    public void setDef(int def) {
        this.def = def;
    }

    public void setSpecialDef(int specialDef) {
        this.specialDef = specialDef;
    }

    public int sumOfDetails(){
        return this.HP + this.attack + this.specialAttack + this.def + this.specialDef;
    }

    public PokemonDetails() {
    }

    public PokemonDetails(PokemonDetails details) {
        this.HP = details.HP;
        this.attack = details.attack;
        this.specialAttack = details.specialAttack;
        this.def = details.def;
        this.specialDef = details.specialDef;
    }

    @Override
    public String toString() {
        String returnString = "";
        returnString = returnString + "HP " + this.HP;
        if(this.attack != 0)
            returnString = returnString + " Attack " + this.attack;
        if(this.specialAttack != 0)
            returnString = returnString + " Special Attack " + this.specialAttack;
        returnString = returnString + " Def " + this.def;
        returnString = returnString + " Special Def " + this.specialDef;
        return returnString;
    }
}

class PokemonDetailsBuilder{
    private PokemonDetails pokemonDetails = new PokemonDetails();

    public PokemonDetailsBuilder withHP(int newHP){
        this.pokemonDetails.setHP(newHP);
        return this;
    }

    public PokemonDetailsBuilder withNormalAttack(int newNormalAttack) {
        this.pokemonDetails.setAttack(newNormalAttack);
        return this;
    }

    public PokemonDetailsBuilder withSpecialAttack(int newSpecialAttack) {
        this.pokemonDetails.setSpecialAttack(newSpecialAttack);
        return this;
    }

    public PokemonDetailsBuilder withDef(int newDef) {
        this.pokemonDetails.setDef(newDef);
        return this;
    }

    public PokemonDetailsBuilder withSpecialDef(int newSpecialDef) {
        this.pokemonDetails.setSpecialDef(newSpecialDef);
        return this;
    }

    public PokemonDetails finish() {
        return this.pokemonDetails;
    }
}
