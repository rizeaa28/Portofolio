package com.company;

public class PokemonItem implements PokemonInterface{
    PokemonDetails details;

    public PokemonItem setDetails(PokemonDetails details) {
        this.details = details;
        return this;
    }
}
