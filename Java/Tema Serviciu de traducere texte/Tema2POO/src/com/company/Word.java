package com.company;

import java.util.ArrayList;
import java.util.Arrays;

public class Word implements Comparable{
    String word;
    String word_en;
    String type;
    String[] singular;
    String[] plural;
    Definition[] definitions;

    public Word() {
    }

    @Override
    public int compareTo(Object o) {
        return ((Word) o).getWord().compareTo(this.getWord()) * (-1);
    }

    public void addDefinition(Definition definition) {
        //Makes a new array to add definitions to initial array
        Definition[] newArray = new Definition[this.definitions.length + 1];
        System.arraycopy(this.definitions, 0, newArray, 0, this.definitions.length);
        newArray[this.definitions.length] = definition;
        this.definitions = newArray;
    }

    public void removeDefinition(int index) {
        Definition[] newArray = new Definition[this.definitions.length - 1];
        int k = 0;

        //iterates through the definitions and if it finds the definitions
        //that has to be removed, it doesnt add it to new array
        for(int i = 0; i < this.definitions.length; i++) {
            if(i != index)
                newArray[k++] = this.definitions[i];
        }

        this.definitions = newArray;
    }

     public ArrayList<String> getSynonyms() {
        ArrayList<String> synonyms = new ArrayList<String>();

        //retrieves the synonyms of a word if it has any
        for(int i = 0; i < this.definitions.length; i++) {
            if(this.definitions[i].getDictType().equals("synonyms")) {
                String[] wordsSynonyms = this.definitions[i].getText();
                for(int j = 0; j < wordsSynonyms.length; j++) {
                    synonyms.add(wordsSynonyms[j]);
                }
            }
        }
        return synonyms;
    }

    public String getWord() {
        return word;
    }

    public String getWord_en() {
        return word_en;
    }

    public String getType() {
        return type;
    }

    public String[] getSingular() {
        return singular;
    }

    public String[] getPlural() {
        return plural;
    }

    public Definition[] getDefinitions() {
        return definitions;
    }

    public void setWord(String word) {
        this.word = word;
    }

    public void setWord_en(String word_en) {
        this.word_en = word_en;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setSingular(String[] singular) {
        this.singular = singular;
    }

    public void setPlural(String[] plural) {
        this.plural = plural;
    }

    public void setDefinitions(Definition[] definitions) {
        this.definitions = definitions;
    }

    @Override
    public String toString() {
        return "Word{" +
                "word='" + word + '\'' +
                ", word_en='" + word_en + '\'' +
                ", type='" + type + '\'' +
                ", singular=" + Arrays.toString(singular) +
                ", plural=" + Arrays.toString(plural) +
                ", definitions=" + Arrays.toString(definitions) +
                '}';
    }
}
