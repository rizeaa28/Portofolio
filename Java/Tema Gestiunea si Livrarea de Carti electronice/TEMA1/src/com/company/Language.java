package com.company;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class Language {
    int ID;
    String code;
    String name;

    public Language(int ID, String code, String name) {
        this.ID = ID;
        this.code = code;
        this.name = name;
    }

    public int getID() {
        return ID;
    }

    public String toString(){
        return "Language: " + ID + ", " + code + ", " + name;
    }
}
