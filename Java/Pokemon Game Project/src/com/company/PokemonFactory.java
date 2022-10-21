package com.company;

public class PokemonFactory {
    private static PokemonFactory unique;

    private PokemonFactory(){
    }

    public static PokemonFactory PokemonFactoryInstance(){
        if(unique == null)
            unique = new PokemonFactory();
        return unique;
    }

    public PokemonInterface getPokemon(String pokemonType) {
        if(pokemonType == null)
            return null;
        switch(pokemonType) {
            case "Neutrel1":
                return new Pokemon("Neutrel1").setDetails(new PokemonDetailsBuilder().withHP(10)
                                                    .withNormalAttack(3).withDef(1)
                                                    .withSpecialDef(1).finish());
            case "Neutrel2":
                return new Pokemon("Neutrel2").setDetails(new PokemonDetailsBuilder().withHP(20)
                                                    .withNormalAttack(4).withDef(1)
                                                    .withSpecialDef(1).finish());
            case "Pikachu":
                return new Pokemon("Pikachu").setDetails(new PokemonDetailsBuilder().withHP(35)
                        .withSpecialAttack(4).withDef(2).withSpecialDef(3).finish())
                        .setAbility(1, new PokemonAbility(6, 4, false, false))
                        .setAbility(2, new PokemonAbility(4, 5, true, true));
            case "Bulbasaur":
                return new Pokemon("Bulbasaur").setDetails(new PokemonDetailsBuilder().withHP(42)
                        .withSpecialAttack(5).withDef(3).withSpecialDef(1).finish())
                        .setAbility(1, new PokemonAbility(6, 4, false, false))
                        .setAbility(2, new PokemonAbility(5, 3, false, false));
            case "Charmander":
                return new Pokemon("Charmander").setDetails(new PokemonDetailsBuilder().withHP(50)
                        .withNormalAttack(4).withDef(3).withSpecialDef(2).finish())
                        .setAbility(1, new PokemonAbility(4, 4, true, false))
                        .setAbility(2, new PokemonAbility(7, 6, false, false));
            case "Squirtle":
                return new Pokemon("Squirtle").setDetails(new PokemonDetailsBuilder().withHP(60)
                        .withSpecialAttack(3).withDef(5).withSpecialDef(5).finish())
                        .setAbility(1, new PokemonAbility(4, 3, false, false))
                        .setAbility(2, new PokemonAbility(2, 2, true ,false));
            case "Snorlax":
                return new Pokemon("Snorlax").setDetails(new PokemonDetailsBuilder().withHP(62)
                        .withNormalAttack(3).withDef(6).withSpecialDef(4).finish())
                        .setAbility(1, new PokemonAbility(4, 5, true, false))
                        .setAbility(2, new PokemonAbility(0, 5, false, true));
            case "Vulpix":
                return new Pokemon("Vulpix").setDetails(new PokemonDetailsBuilder().withHP(36)
                        .withNormalAttack(5).withDef(2).withSpecialDef(4).finish())
                        .setAbility(1, new PokemonAbility(8, 6, true, false))
                        .setAbility(2, new PokemonAbility(2, 7, false, true));
            case "Eevee":
                return new Pokemon("Eevee").setDetails(new PokemonDetailsBuilder().withHP(39)
                        .withSpecialAttack(4).withDef(3).withSpecialDef(3).finish())
                        .setAbility(1, new PokemonAbility(5, 3, false, false))
                        .setAbility(2, new PokemonAbility(3, 3, true, false));
            case "Jigglypuff":
                return new Pokemon("Jigglypuff").setDetails(new PokemonDetailsBuilder().withHP(34)
                        .withNormalAttack(4).withDef(2).withSpecialDef(3).finish())
                        .setAbility(1, new PokemonAbility(4, 4, true, false))
                        .setAbility(2, new PokemonAbility(3, 4, true, false));
            case "Meowth":
                return new Pokemon("Meowth").setDetails(new PokemonDetailsBuilder().withHP(41)
                        .withNormalAttack(3).withDef(4).withSpecialDef(2).finish())
                        .setAbility(1, new PokemonAbility(5, 4, false, true))
                        .setAbility(2, new PokemonAbility(1, 3, false, true));
            case "Psyduck":
                return new Pokemon("Psyduck").setDetails(new PokemonDetailsBuilder().withHP(43)
                        .withNormalAttack(3).withDef(3).withSpecialDef(3).finish())
                        .setAbility(1, new PokemonAbility(2, 4, false, false))
                        .setAbility(2, new PokemonAbility(2, 5, true, false));
            case "Scut":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withDef(2).withSpecialDef(2).finish());
            case "Vesta":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withHP(10).finish());
            case "Sabiuta":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withNormalAttack(3).finish());
            case "Bagheta":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withSpecialAttack(3).finish());
            case "Vitamine":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withHP(2).withNormalAttack(2).withSpecialAttack(2).finish());
            case "Brad":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withNormalAttack(3).withDef(1).finish());
            case "Pelerina":
                return new PokemonItem().setDetails(new PokemonDetailsBuilder().withSpecialDef(3).finish());
            default:
                return null;
        }
    }
}
