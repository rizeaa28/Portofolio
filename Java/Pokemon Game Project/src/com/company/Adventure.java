package com.company;

public class Adventure {
    Pokemon pokemon1, pokemon2;

    public Adventure(Pokemon pokemon1, Pokemon pokemon2) {
        this.pokemon1 = pokemon1;
        this.pokemon2 = pokemon2;
    }

    //sets the opponent trainer to the pokemon
    public void setOpponents() {
        this.pokemon1.opponent = this.pokemon2.trainer;
        this.pokemon2.opponent = this.pokemon1.trainer;
    }

    //starts a duel between 2 trainers and their pokemons
    public void startDuel() {
        Logger logger = Logger.Instance();
        logger.printStartOfFight(pokemon1, pokemon2);

        Thread fighter1 = new Thread(pokemon1);
        Thread fighter2 = new Thread(pokemon2);

        int k = 0; //round

        while(pokemon1.getActiveHP() > 0 && pokemon2.getActiveHP()  > 0) {
            k++;
            logger.print("\n==Runda " + k);
            pokemon1.trainer.commandAttack();
            pokemon2.trainer.commandAttack();

            fighter1.run();
            fighter2.run();
        }

        //Check if draw
        if(pokemon1.getActiveHP() == 0 && pokemon2.getActiveHP() == 0) {
            logger.print("\n====Meciul se termina la o egalitate====");
            pokemon1.resetPokemon(false);
            pokemon2.resetPokemon(false);
            return;
        }

        //check if pokemon2 won
        if(pokemon1.getActiveHP() > 0) {
            logger.print("\n====Antrenorul " + pokemon1.trainer.getName() +" castiga.====");
            pokemon1.resetPokemon(true);
            pokemon2.resetPokemon(false);
        }
        //check if pokemon1 won
        else {
            logger.print("\n====Antrenorul " + pokemon2.trainer.getName() +" castiga.====");
            pokemon2.resetPokemon(true);
            pokemon1.resetPokemon(false);
        }
    }
}

//class which defines attack by just the needed information
//about the attack needed to be transmitted to other pokemon
class AttackUsed {
    int typeOfAttack;
    boolean stun, dodge;
    int damage;

    public void set(int damage, boolean stun, boolean dodge) {
        this.stun = stun;
        this.damage = damage;
        this.dodge = dodge;
    }
}

