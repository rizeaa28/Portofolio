package com.company;

public class Arena {
    Trainer trainer1, trainer2;
    //the trainer which has Neutrels (NPC = non-person character)
    Trainer NPC = new Trainer();

    public Arena(Trainer trainer1, Trainer trainer2) {
        this.trainer1 = trainer1;
        this.trainer2 = trainer2;
    }

    //sets up a duel
    public Adventure setUpDuel(int pokemon1, int pokemon2) {
        Adventure adventure = new Adventure(this.trainer1.pokemonList.get(pokemon1 - 1),
                                                this.trainer2.pokemonList.get(pokemon2 - 1));
        //attaches the trainers and their active pokemon for the fight
        this.trainer1.pokemonList.get(pokemon1 - 1).attachTrainer(trainer1);
        this.trainer2.pokemonList.get(pokemon2 - 1).attachTrainer(trainer2);
        adventure.setOpponents();

        return adventure;
    }

    //same this as set up duel, but it sets up 2 duels
    //one between npc and trainer1, and one between trainer 2 and npc
    public void setUpNPCDuel(int pokemon1, int pokemon2, int NPC) {
        Adventure adventure1 = new Adventure(this.trainer1.pokemonList.get(pokemon1 - 1),
                                this.NPC.pokemonList.get(NPC - 1));
        Adventure adventure2 = new Adventure(this.trainer2.pokemonList.get(pokemon2 - 1),
                                this.NPC.pokemonList.get(NPC - 1));
        this.trainer1.pokemonList.get(pokemon1 - 1).attachTrainer(trainer1);
        this.trainer2.pokemonList.get(pokemon2 - 1).attachTrainer(trainer2);
        this.NPC.pokemonList.get(NPC - 1).attachTrainer(this.NPC);

        //NPC vs trainer 1
        adventure1.setOpponents();
        adventure1.startDuel();

        //NPC vs trainer 2
        adventure2.setOpponents();
        adventure2.startDuel();
    }

    //chooses randomly which type of adventure for the first 3 arena rounds
    public void adventureSwitch(int arenaRound) {
        int typeOfAdventure = (int)(Math.random() * 10) % 3;
        switch(typeOfAdventure) {
            case 0:
                setUpNPCDuel(arenaRound, arenaRound, 1);
                break;
            case 1:
                setUpNPCDuel(arenaRound, arenaRound, 2);
                break;
            case 2:
                Adventure adventure = setUpDuel(arenaRound, arenaRound);
                adventure.startDuel();
                break;
        }
    }

    //starts the adventures in the arena
    public void startAdventure() {
        Logger logger = Logger.Instance();
        logger.startOfArena(trainer1.getName(), trainer2.getName());

        logger.announceArenaRound(1);
        adventureSwitch(1);
        logger.announceArenaRound(2);
        adventureSwitch(2);
        logger.announceArenaRound(3);
        adventureSwitch(3);

        //finding best pokemon for 4th round of arena
        int bestPokemon1 = trainer1.findBestPokemon();
        int bestPokemon2 = trainer2.findBestPokemon();

        logger.announceArenaRound(4);
        Adventure lastAdventure = setUpDuel(bestPokemon1 + 1, bestPokemon2 + 1);
        lastAdventure.startDuel();
    }
}
