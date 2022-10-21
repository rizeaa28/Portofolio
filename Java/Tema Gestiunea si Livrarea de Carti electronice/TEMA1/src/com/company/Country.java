package com.company;

public class Country {
    int ID;
    String countryCode;

    public Country() {
    }

    public Country(int ID, String countryCode) {
        this.ID = ID;
        this.countryCode = countryCode;
    }

    public int getID() {
        return ID;
    }

    @Override
    public String toString() {
        return "Country{" +
                "ID=" + ID +
                ", countryCode='" + countryCode + '\'' +
                '}';
    }
}
