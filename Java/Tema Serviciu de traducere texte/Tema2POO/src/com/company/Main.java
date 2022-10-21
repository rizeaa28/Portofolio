package com.company;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;

import java.io.*;
import java.lang.reflect.Type;
import java.util.*;

public class Main {
    //The map that contains the dictionaries and their words
    public static Map<String, ArrayList<Word>> mapOfLanguages = new TreeMap<String, ArrayList<Word>>();

    //Method that prints the map which includes the dictionaries
    public static void printMap() {
        //iterates through languages
        for(Map.Entry<String, ArrayList<Word> > intrare : mapOfLanguages.entrySet() )
        {
            System.out.println("Language of dictionary: " + intrare.getKey() +
                                ", number of words: " + intrare.getValue().size());

            //iterates through words to print them one by one
            for(int i = 0; i < intrare.getValue().size(); i++) {
                System.out.println(intrare.getValue().get(i).toString());
            }
            System.out.println();
        }
    }

    //First method
    static boolean addWord(Word word, String language) {
        //The list of words from the respective language
        ArrayList<Word> list = mapOfLanguages.get(language);

        //If list isnt null checks if the word has to be replaces/added
        if(list != null) {
            int found = -1;
            //if it finds the word but with different definitions it replaces it
            for(int i = 0; i < list.size() && found < 0; i++) {
                if(list.get(i).getWord().equals(word))
                    found = i;
            }

            if(found >= 0) {
                list.set(found, word);
                return true;
            }

            //if the word isnt in the list altogether it adds it
            //(add the word to the list and then replaces the list in the map with the new list)
            if(!list.contains(word)) {
                list.add(word);
                mapOfLanguages.replace(language, list);
                return true;
            }
        }
        return false;
    }

    //Second Method
    static boolean removeWord(String word, String language) {
        //gets the words array from respective language
        ArrayList<Word> list = mapOfLanguages.get(language);

        int found = -1; //is positive if the word is found and memorizes its index
        for(int i = 0; i < list.size() && found < 0; i++) {
            if(list.get(i).getWord().equals(word))
                found = i;
        }

        //if the list isnt null (language doesn't exist) and word exists, it removes it
        if(list != null)
            if(found >= 0) {
                list.remove(found);
                mapOfLanguages.replace(language, list);
                return true;
            }
        return false;
    }

    //Method 3
    static boolean addDefinitionForWord(String word, String language, Definition definition) {
        ArrayList<Word> list = mapOfLanguages.get(language);

        //Tries to find word
        int found = -1;
        for(int i = 0; i < list.size() && found < 0; i++) {
            if(list.get(i).getWord().equals(word))
                found = i;
        }

        //If the word is found
        if(found > 0) {
            for(int i = 0; i < list.get(found).getDefinitions().length; i++) {
                if(list.get(found).getDefinitions()[i].equals(definition))
                    //if it has the definition already it doesnt add it
                    return false;
            }

            //adds the definition and returns true
            list.get(found).addDefinition(definition);
            return true;
        }
        //hasnt found the word, returns false
        return false;
    }

    //Method 4
    static boolean removeDefinition(String word, String language, String dictionary) {
        ArrayList<Word> list = mapOfLanguages.get(language);

        int found = -1;
        for(int i = 0; i < list.size() && found < 0; i++) {
            if(list.get(i).getWord().equals(word))
                found = i;
        }

        if(found == -1)
            return false;

        //looks through the definitions of the found word just like in method 3
        //but if it finds it, it deletes it with removeDefinition(int i)
        for(int i = 0; i < list.get(found).getDefinitions().length; i++) {
            if(list.get(found).getDefinitions()[i].getDict().equals(dictionary)) {
                list.get(found).removeDefinition(i);
                return true;
            }
        }
        return false;
    }

    //Method 5
    static String translateWord(String word, String fromLanguage, String toLanguage) {
        //Arrays of word for the initial and final languages from the map
        ArrayList<Word> initialLanguage = mapOfLanguages.get(fromLanguage);
        ArrayList<Word> finalLanguage = mapOfLanguages.get(toLanguage);

        //Iterates through the word from the initial language to find the word given in parameters
        for(int i = 0; i < initialLanguage.size(); i++) {
            if(initialLanguage.get(i).getWord().equals(word)) {
                //if the word was found, it retrieves its english translation
                String englishWord = initialLanguage.get(i).getWord_en();

                //tries to find a word with this english translation in the final language
                for(int j = 0; j < finalLanguage.size(); j++) {
                    if(finalLanguage.get(j).getWord_en().equals(englishWord)) {
                        //it found such word, and it returns the word
                        return finalLanguage.get(j).getWord();
                    }
                }
            }
        }
        return null;
    }

    //Method 6
    static String translateSentence(String sentence, String fromLanguage, String toLanguage) {
        //we divide the sentence we got as a parameter into separate strings
        //which go into a string array
        String[] wordsToTranslate = sentence.split("[ ]");

        //initialising the output sentence
        String translatedSentence = "";

        //iterates through the array of strings and translates each word one by one
        //using translateWord method and adds the translated word to the output sentence
        for(int i = 0; i < wordsToTranslate.length; i++) {
            if(translateWord(wordsToTranslate[i], fromLanguage, toLanguage) == null)
                //if the word doesnt have a translation, it will put a "?" in its place
                translatedSentence = translatedSentence + "?";
            else
                translatedSentence = translatedSentence + translateWord(wordsToTranslate[i], fromLanguage, toLanguage);
            if(i != wordsToTranslate.length - 1)
                translatedSentence = translatedSentence + " ";
        }
        return translatedSentence;
    }

    //Method 7
    static ArrayList<String> translateSentences(String sentence, String fromLanguage, String toLanguage) {
        //initialises the output
        ArrayList<String> translatedSentences = new ArrayList<String>();
        //assigns the first translation the one resulted from the translateSentence method
        translatedSentences.add(translateSentence(sentence, fromLanguage, toLanguage));

        ArrayList<Word> finalLanguage = mapOfLanguages.get(toLanguage);

        //splits the already translated sentence and looks through
        //its words to find if they have synonyms
        String[] wordsToFindSynonyms = translatedSentences.get(0).split("[ ]");
        int k = 0; //retains the number of translations in the ArrayList (max 3 translations)

        String newTranslation = ""; //starts building the alternative translations
        for(int i = 0; i < wordsToFindSynonyms.length && k < 2; i++) {
            for(int j = 0; j < finalLanguage.size() && k < 2; j++) {
                if(finalLanguage.get(j).getWord().equals(wordsToFindSynonyms[i])) {
                    //if it finds the word were looking to have a synonym for, it
                    //puts its synonyms into the ArrayList below
                    ArrayList<String> wordSynonyms = finalLanguage.get(j).getSynonyms();

                    //iterates through the synonyms found for the word, doesn't
                    //start iterating if there are no synonyms
                    for(int p = 0; p < wordSynonyms.size() && k < 2; p++) {
                        //with the synonym found it constructs the new translated sentence
                        //by putting the synonym in the place of the initial word we found the synonym for
                        String constructTranslation = newTranslation;
                        constructTranslation = constructTranslation + " " + wordSynonyms.get(p);

                        //adds the word after the word we found the translation for
                        for(int n = i + 1; n < wordsToFindSynonyms.length; n++)
                            constructTranslation = constructTranslation + " " + wordsToFindSynonyms[n];

                        //adds the translation to the output and changes k
                        translatedSentences.add(constructTranslation);
                        k++;
                    }
                }
            }
                //newTranslation parameter will contain the part of the translated sentence
                //before the word that were looking to have a synonym for
                newTranslation = newTranslation + " " + wordsToFindSynonyms[i];
        }
        return translatedSentences;
    }

    //Method 8
    static ArrayList<Definition> getDefinitionsForWord(String word, String language) {
        ArrayList<Word> wordOfLanguage = mapOfLanguages.get(language);
        //initialises the output
        ArrayList<Definition> definitionsList = new ArrayList<Definition>();

        //tries to find the word that we need the definitions for
        Word wordToDefine = null;
        for(int i = 0; i < wordOfLanguage.size(); i++)
            if(wordOfLanguage.get(i).getWord().equals(word))
                wordToDefine = wordOfLanguage.get(i);

        if(wordToDefine != null) {
            //if we found the word, we save the definitions in the ArrayList output
            Definition[] definitions = wordToDefine.getDefinitions();
            for(int j = 0; j < definitions.length; j++)
                definitionsList.add(definitions[j]);
        }
        else {
            //if word couldnt be found
            System.out.println("Word doesn't exist");
            return null;
        }

        if(definitionsList.size() == 0) {
            //if the word doesnt have any definitons
            System.out.println("The word doesn't have any definitions");
            return null;
        }

        //sorts the definitions
        Collections.sort(definitionsList);
        return definitionsList;
    }

    //Method 9
    static void exportDictionary(String language) {

        //sorts the words ArrayList for needed language
        Collections.sort(mapOfLanguages.get(language));

        //sorts all the definitions for each word
        for(int i = 0; i < mapOfLanguages.get(language).size(); i++)
            Arrays.sort(mapOfLanguages.get(language).get(i).getDefinitions());

        //builds the file's name
        String nameOfFile = language;
        nameOfFile = nameOfFile + "_out.json";

        //builds and array with all the words from the dictionary
        //(easier to parse to json an array and not an ArrayList)
        Word[] wordToJSON = new Word[mapOfLanguages.get(language).size()];

        for(int i = 0; i < mapOfLanguages.get(language).size(); i++) {
            wordToJSON[i] = mapOfLanguages.get(language).get(i);
        }


        try (Writer writer = new FileWriter(nameOfFile)) {
            //parses the array to json and puts in the file
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            gson.toJson(wordToJSON, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public static void main(String[] args) throws IOException{
        Gson gson = new Gson();

        //Initialize folder to read list of json files from
        File dir = new File("input");
        File[] files = dir.listFiles();

        //Iterate in json files (languages)
        for (File file : files) {

            if (file.isFile()) {
                BufferedReader inputStream = null;
                try {
                    inputStream = new BufferedReader(new FileReader(file));

                    //Get the type of ArrayList<Word> and use that type to parse from json with gson
                    Type wordListType = new TypeToken<ArrayList<Word>>(){}.getType();
                    ArrayList<Word> list = gson.fromJson(inputStream, wordListType);

                    //Extract language from "language_dict.json" to use it as key in the map
                    String[] fileName = file.getName().split("[_]");
                    //Insert the value (ArrayList<Word>) to the key extracted above in the map
                    mapOfLanguages.put(fileName[0], list);

                } catch (IOException e) {
                    System.out.println(e);
                } finally {
                    if (inputStream != null)
                        inputStream.close();
                }
            }
        }
        //Print Map
        printMap();

        //Read different words and parse them to objects to use them for testing
        File word1 = new File("word1.json");
        BufferedReader inputWord1 = new BufferedReader(new FileReader(word1));
        Word newWord1 = gson.fromJson(inputWord1, Word.class);

        File word2 = new File("word2.json");
        BufferedReader inputWord2 = new BufferedReader(new FileReader(word2));
        Word newWord2 = gson.fromJson(inputWord2, Word.class);

        //Add Definitions and construct them for tests
        Definition definition1;
        String[] synonyms1 = new String[3];
        synonyms1[0] = "sfidător";
        synonyms1[1] = "fără scrupul";
        synonyms1[2] = "fără jenă";
        definition1 = new Definition("Dicționar de sinonime", "synonyms", 2001, synonyms1);

        Definition definition2;
        String[] synonyms2 = new String[3];
        synonyms2[0] = "se deplasează";
        synonyms2[1] = "se duce";
        synonyms2[2] = "se mișcă";
        definition2 = new Definition("Dicționar de sinonime", "synonyms", 2002, synonyms2);

        //Testing Method 1
        System.out.println("==========TESTING METHOD 1==========");
        System.out.println("**Testing adding word1: " + addWord(newWord1, "ro"));

        System.out.println("**Testing adding word2: " + addWord(newWord2, "ro"));

        System.out.println("**Testing adding existing word: " + addWord(mapOfLanguages.get("fr").get(1), "fr"));
        printMap();

        //Testing Method 2
        System.out.println("==========TESTING METHOD 2==========");
        System.out.println("**Testing removing word2: " + removeWord(newWord2.getWord(), "ro"));

        System.out.println("**Testing removing another existing word: " + removeWord("jeu", "fr"));

        System.out.println("**Testing removing unexistent word: " + removeWord("ana", "ro"));
        printMap();

        //Testing Method 3
        System.out.println("==========TESTING METHOD 3==========");
        System.out.println("**Testing adding definition1: " + addDefinitionForWord("cinic", "ro", definition1));
        System.out.println("**Testing adding definition2: " + addDefinitionForWord("merge", "ro", definition2));
        System.out.println("**Testing adding definition to non-existent word: " + addDefinitionForWord("ana", "ro", definition2));
        printMap();

        //Testing Method 4
        System.out.println("==========TESTING METHOD 4==========");
        System.out.println("**Testing removing definition 1: " + removeDefinition("merge", "ro", "Micul dicționar academic, ediția a II-a"));
        System.out.println("**Testing removing definition 2: " + removeDefinition("câine", "ro", "Dicționar universal al limbei române, ediția a VI-a)"));
        System.out.println("**Testing removing not existent definition : " + removeDefinition("câine", "ro", "Dicționarul ce nu exista"));
        printMap();

        //Testing Method 5
        System.out.println("==========TESTING METHOD 5==========");
        System.out.println("**Testing translating 'chat': " + translateWord("chat", "fr", "ro"));
        System.out.println("**Testing translating 'pisică': " + translateWord("pisică", "ro", "fr"));

        System.out.print("**Testing translating 'merge' (doesn't have translation): ");
        if(translateWord("merge", "ro", "fr") == null)
            System.out.println("Translation doesn't exist!");
        else
            System.out.println(translateWord("merge", "ro", "fr"));

        //Testing Method 6
        System.out.println("\n==========TESTING METHOD 6==========");
        System.out.println("**Testing translating 'chat manger': " + translateSentence("chat manger", "fr", "ro"));
        System.out.println("**Testing translating 'câine mănâncă': " + translateSentence("câine mănâncă", "ro", "fr"));
        System.out.println("**Testing translating 'câine mănâncă casa' -> last word won't be translated: "
                + translateSentence("câine mănâncă casa", "ro", "fr"));


        //Testing Method 7
        System.out.println("\n==========TESTING METHOD 7==========");
        System.out.print("**Testing translating 'chat manger': ");
        ArrayList<String> translatedSentencesTest1 = translateSentences("chat manger", "fr", "ro");

        for(int i = 0; i < translatedSentencesTest1.size(); i++)
            System.out.println(translatedSentencesTest1.get(i));

        System.out.print("**Testing translating 'câine mănâncă': ");
        ArrayList<String> translatedSentencesTest2 = translateSentences("câine mănâncă", "ro", "fr");

        for(int i = 0; i < translatedSentencesTest2.size(); i++)
            System.out.println(translatedSentencesTest2.get(i));

        removeDefinition("cinic", "ro", "Dicționar de sinonime");
        System.out.print("**Testing translating 'chien cinique' (will have only one translation): ");
        ArrayList<String> translatedSentencesTest3 = translateSentences("chien cinique", "fr", "ro");

        for(int i = 0; i < translatedSentencesTest3.size(); i++)
            System.out.println(translatedSentencesTest3.get(i));

        //Testing Method 8
        System.out.println("\n==========TESTING METHOD 8==========");
        System.out.println("**Testing getting definitions for 'câine': ");

        ArrayList<Definition> definitionsTest1 = getDefinitionsForWord("câine", "ro");
        if(definitionsTest1 != null)
            for(int i = 0; i < definitionsTest1.size(); i++)
                System.out.println(definitionsTest1.get(i).toString());

        System.out.println("\n**Testing getting definitions for 'pisică': ");

        ArrayList<Definition> definitionsTest2 = getDefinitionsForWord("pisică", "ro");
        if(definitionsTest2 != null)
            for(int i = 0; i < definitionsTest2.size(); i++)
                System.out.println(definitionsTest2.get(i).toString());

        System.out.println("\n**Testing getting definitions for notexistent word: ");

        ArrayList<Definition> definitionsTest3 = getDefinitionsForWord("ana", "fr");
        if(definitionsTest3 != null)
            for(int i = 0; i < definitionsTest3.size(); i++)
                System.out.println(definitionsTest3.get(i).toString());


        //Testing Method 9
        exportDictionary("ro");
        exportDictionary("fr");
    }


}
