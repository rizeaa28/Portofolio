package com.company;

public class PokemonAbility {
    private int damage, cooldown;
    private boolean stun, dodge;
    public int activeCooldown = 0;

    public PokemonAbility(int damage, int cooldown, boolean stun, boolean dodge) {
        this.damage = damage;
        this.cooldown = cooldown;
        this.stun = stun;
        this.dodge = dodge;
    }

    public int getDamage() {
        return damage;
    }

    public boolean isStun() {
        return stun;
    }

    public boolean isDodge() {
        return dodge;
    }

    public boolean isUp() {
        if(activeCooldown == 0)
            return true;
        return false;
    }

    public void useAbility() {
        this.activeCooldown = this.cooldown;
    }

    public void reduceCooldown() {
        if(this.activeCooldown > 0)
            this.activeCooldown--;
    }

    public String toString(int nr) {
        if(this.activeCooldown != 0)
            return "[abilitatea " + nr + " cooldown " + this.activeCooldown + "]";
        return "";
    }
}
