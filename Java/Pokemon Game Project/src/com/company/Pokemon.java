package com.company;

public class Pokemon implements Comparable, Runnable, PokemonInterface {
    private String name;
    private PokemonAbility ability1, ability2;
    private PokemonDetails details, activeDetails;
    //OBSERVER - SUBJECTS
    Trainer trainer, opponent;
    boolean stun = false;
    boolean dodge = false;

    public Pokemon(String name) {
        this.name = name;
    }

    //in run method the pokemon sees the ability usen against him
    //and takes the damage
    @Override
    public void run() {
        Logger logger = Logger.Instance();
        int damage = 0;
        int initialHP = this.activeDetails.HP;
        if(!this.dodge) { //if he didnt dodge. he takes damage
            switch(this.opponent.attackUsed.typeOfAttack) {
                case 0:
                    damage = this.opponent.attackUsed.damage - this.activeDetails.def;
                    break;
                case 1, 2:
                    damage = this.opponent.attackUsed.damage;
                    if(this.opponent.attackUsed.stun)
                        this.stun = true;
                    break;
                case 3:
                    damage = this.opponent.attackUsed.damage - this.activeDetails.specialDef;
                    break;
            }
            //sets new hp
            if(damage < 0)
                damage = 0;
            this.activeDetails.setHP(this.activeDetails.HP - damage);
            if(this.activeDetails.HP < 0)
                this.activeDetails.HP = 0;
        }
        //prints state of pokemon
        logger.print(this.toString(damage, initialHP));

        //goes out of dodge if he dodged this round
        if(this.dodge) {
            this.dodge = false;
            logger.print(this.name + " a dat dodge");
        }
    }

    //OBSERVER - UPDATE OBSERVER
    public void attack() {
        Logger logger = Logger.Instance();

        //if its stunned, it doesnt attack (typeOfAttack = 4)
        if(this.stun) {
            this.stun = false;
            trainer.attackUsed.typeOfAttack = 4;
            this.trainer.attackUsed.set(0, false, false);

            if(this.ability1 != null)
                this.ability1.reduceCooldown();
            if(this.ability2 != null)
                this.ability2.reduceCooldown();

            logger.print(this.name + " este stunned");
        }
        //if not stunned, it attacks according tt the attack type sent by trainer
        switch(trainer.attackUsed.typeOfAttack) {
            case 0, 3:
                attackClassic();
                break;
            case 1:
                if(this.ability1.isUp())
                    attackAbility(trainer.attackUsed.typeOfAttack);
                else
                    trainer.attackUsed.typeOfAttack = -1;
                break;
            case 2:
                if(this.ability2.isUp())
                    attackAbility(trainer.attackUsed.typeOfAttack);
                else
                    trainer.attackUsed.typeOfAttack = -1;
                break;
        }
        //if the attack sent by trainer was eligible (wasnt on cooldown, was avaiable to the pokemon)
        //it prints the type of attack used
        if(trainer.attackUsed.typeOfAttack != -1 && trainer.attackUsed.typeOfAttack != 4)
            logger.printAbility(this.name, trainer.attackUsed.typeOfAttack);
    }

    public int getActiveHP() {
        return this.activeDetails.HP;
    }

    //attaches the trainer for the fight
    //OBSERVER - ATTACH SUBJECT
    public void attachTrainer(Trainer trainer) {
        this.trainer = trainer;
        this.trainer.setActivePokemon(name);
        this.activeDetails = new PokemonDetails(this.details);
    }

    public void attackClassic() {
        int damage = 0;
        if(this.details.attack != 0)
            damage = this.details.attack;
        else
            damage = this.details.specialAttack;

        this.trainer.attackUsed.set(damage, false, false);

        if(this.ability1 != null)
            this.ability1.reduceCooldown();
        if(this.ability2 != null)
            this.ability2.reduceCooldown();
    }

    public void attackAbility(int abilityNumber) {
        if(abilityNumber == 1) {
            this.trainer.attackUsed.set(this.ability1.getDamage(), this.ability1.isStun(), this.ability1.isDodge());
            this.ability1.useAbility();
            this.ability2.reduceCooldown();
            if(this.ability1.isDodge())
                this.dodge = true;
        }
        if(abilityNumber == 2) {
            this.trainer.attackUsed.set(this.ability2.getDamage(), this.ability2.isStun(), this.ability2.isDodge());
            this.ability2.useAbility();
            this.ability1.reduceCooldown();
            if(this.ability2.isDodge())
                this.dodge = true;
        }
    }

    //sets the abilities of the pokemon
    public Pokemon setAbility(int whichAbility, PokemonAbility newAbility) {
        switch(whichAbility) {
            case 1:
                this.ability1 = newAbility;
                break;
            case 2:
                this.ability2 = newAbility;
                break;
        }
        return this;
    }

    //resets the pokemon after a fight
    //if winner it adds the +1 bonus
    public void resetPokemon(boolean winner) {
        Logger logger = Logger.Instance();

        if(winner) {
            this.details.HP++;
            if(this.details.attack != 0)
                this.details.attack++;
            if(this.details.specialAttack != 0)
                this.details.specialAttack++;

            this.details.def++;
            this.details.specialDef++;
        }
        this.activeDetails = new PokemonDetails(this.details);

        if(winner)
            logger.printWinnerPokemon(this);
    }

    public String getName() {
        return name;
    }

    public Pokemon setDetails(PokemonDetails details) {
        this.details = details;
        return this;
    }

    //adds an item to the pokemon
    public void addItem(String item) {
        PokemonFactory factory = PokemonFactory.PokemonFactoryInstance();
        PokemonItem pokemonItem = (PokemonItem)factory.getPokemon(item);
        if(pokemonItem.details.HP != 0)
            this.details.setHP(this.details.HP + pokemonItem.details.HP);
        if(pokemonItem.details.attack != 0)
            this.details.setAttack(this.details.attack + pokemonItem.details.attack);
        if(pokemonItem.details.specialAttack != 0)
            this.details.setSpecialAttack(this.details.specialAttack + pokemonItem.details.specialAttack);
        if(pokemonItem.details.def != 0)
            this.details.setDef(this.details.def + pokemonItem.details.def);
        if(pokemonItem.details.specialDef != 0)
            this.details.setSpecialDef(this.details.specialDef + pokemonItem.details.specialDef);
    }

    //compareTo method to see which is the best pokemon for the last battle
    @Override
    public int compareTo(Object o) {
        int sum1, sum2;
        sum1 = this.details.sumOfDetails();
        sum2 = ((Pokemon)o).details.sumOfDetails();

        if(sum1 == sum2)
            return this.name.compareTo(((Pokemon)o).getName());
        else
            return sum1 - sum2;
    }

    public String toString(int damage, int initialHP) {
        String retString = "";
        retString = retString + this.name + " HP " + this.activeDetails.HP
                    + "(" + initialHP + " - " + damage + ")" + " ";
        if(this.ability1 != null && this.ability2 != null)
        retString = retString + this.ability1.toString(1) + " " + this.ability2.toString(2);
        return retString;
    }

    public String detailsToString() {
        return this.details.toString();
    }

    public String pokemonStatistics() {
        String retString = "";
        retString = retString + this.name + " [HP " + this.activeDetails.HP + "] ";
        retString = retString + "[Attack " + this.details.attack + "] ";
        retString = retString + "[Special Attack " + this.details.specialAttack + "] ";
        retString = retString + "[Def " + this.details.def + "] ";
        retString = retString + "[Special Def " + this.details.specialDef + "] ";

        return retString;
    }
}

