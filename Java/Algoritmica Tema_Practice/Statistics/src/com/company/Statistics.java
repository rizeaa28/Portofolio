package com.company;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;

public class Statistics {
    static int indexMaxWord = -1;
    static int totalLength = 0;
    static  int[] totalLetterOccurences = new int[26];

    static class Word implements Comparable {
        String word;
        int[] letterOccurences = new int[26];

        public Word(String word) {
            this.word = word;

            for(int i = 0; i < this.word.length(); i++) {
                letterOccurences[word.charAt(i) - 'a']++;
                totalLetterOccurences[word.charAt(i) - 'a']++;
            }
            totalLength = totalLength + this.word.length();
        }

        @Override
        public int compareTo(Object o) {

            int diff1 = this.letterOccurences[indexMaxWord] - (this.word.length() - this.letterOccurences[indexMaxWord]);
            int diff2 = ((Word) o).letterOccurences[indexMaxWord] - (((Word) o).word.length() - ((Word) o).letterOccurences[indexMaxWord]);

            if(diff1 == diff2)
                return this.letterOccurences[indexMaxWord] - ((Word) o).letterOccurences[indexMaxWord];
            else
                return diff1 - diff2;

            /*
            boolean ratio1 = this.letterOccurences[indexMaxWord] > ((float)this.word.length()/2);
            boolean ratio2 = ((Word) o).letterOccurences[indexMaxWord] > ((float)((Word) o).word.length()/2);

            if(ratio1 && ratio2) {
                if(this.letterOccurences[indexMaxWord] == ((Word) o).letterOccurences[indexMaxWord])
                    return (-1)*(this.word.length() - ((Word) o).word.length());
                else
                    return (this.letterOccurences[indexMaxWord] - ((Word) o).letterOccurences[indexMaxWord]) * (-1);
            }
            else
                if(ratio1 || ratio2) {
                    if(ratio1)
                        return 1;
                    else
                        return -1;
                }
            else
                if(this.word.length() - this.letterOccurences[indexMaxWord] == ((Word) o).word.length() - ((Word) o).letterOccurences[indexMaxWord])
                    return this.letterOccurences[indexMaxWord] - ((Word) o).letterOccurences[indexMaxWord];
                else
                    return (-1)*(this.word.length() - this.letterOccurences[indexMaxWord] - (((Word) o).word.length() - ((Word) o).letterOccurences[indexMaxWord]));
        */
        }

        @Override
        public String toString() {
            return "Word{" +
                    "word='" + word + '\'' +
                    '}';
        }
    }

    public static void main(String[] args) throws IOException {

        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader("statistics.in"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        FileWriter myWriter = new FileWriter("statistics.out");

        String line = br.readLine();
        int n = Integer.parseInt(line);
        Word[] words = new Word[n];

        for(int i = 0; i < n; i++) {
            line = br.readLine();
            words[i] = new Word(line);
        }

        int[] maxNumberOfWords = new int[26];
        int ultimateMax = -1;
        int temporaryMax;

        for(int i = 0; i < 26; i++) {
            if(totalLetterOccurences[i] == 0) {
                continue;
            }

            indexMaxWord = i;
            temporaryMax = n;
            Arrays.sort(words);

            for(int j = 0; j < n; j++)
                System.out.println(words[j]);

            float partialLength = (float) totalLength;
            float partialLetterOccurence = (float) totalLetterOccurences[i];
            int k = 0;

            System.out.println("Total Length: " + totalLength);

            while(partialLetterOccurence / partialLength <= 0.5 && temporaryMax > 0) {
                System.out.println("Partial letter occurence: " + partialLetterOccurence + ", partial length: " + partialLength);

                if(temporaryMax < ultimateMax)
                    break;
                partialLength = partialLength - (float)words[k].word.length();
                partialLetterOccurence = partialLetterOccurence - (float)words[k].letterOccurences[i];
                temporaryMax--;
                k++;

            }



            System.out.println(temporaryMax);
            if(temporaryMax == 0)
                temporaryMax--;
            maxNumberOfWords[i] = temporaryMax;
            if(temporaryMax > ultimateMax)
                ultimateMax = temporaryMax;
        }

        myWriter.write(ultimateMax+"\n");
        myWriter.close();
        System.out.println(ultimateMax);
    }
}
