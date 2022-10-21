import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;

public class Statistics {
	// An index we need in multiple functions so I made it static
	static int indexMaxWord = -1;
	
	// The total length of all words in the list
	static int totalLength = 0;

	// The total occurences of every letter in all the words
	static  int[] totalLetterOccurences = new int[26];

	// Class which contains the string (the actual word) 
	// and a vector containing the occurences of each letter
	static class Word implements Comparable {
		String word;
		int[] letterOccurences = new int[26];

		// Constructor that calculates occurances for the string as well as adding to the 
		// total occurences and length variables
		public Word(String word) {
			this.word = word;

			for (int i = 0; i < this.word.length(); i++) {
				letterOccurences[word.charAt(i) - 'a']++;
				totalLetterOccurences[word.charAt(i) - 'a']++;
			}
			totalLength = totalLength + this.word.length();
		}

		// Compare method for words
		@Override
		public int compareTo(Object o) {

			// Differences between character 'a' and characters that are not a in the words
			int diff1 = this.letterOccurences[indexMaxWord] 
						- (this.word.length() - this.letterOccurences[indexMaxWord]);
			int diff2 = ((Word) o).letterOccurences[indexMaxWord] 
						- (((Word) o).word.length() - ((Word) o).letterOccurences[indexMaxWord]);

			if (diff1 == diff2) {
				return this.letterOccurences[indexMaxWord] 
					- ((Word) o).letterOccurences[indexMaxWord];
			} else {
				return diff1 - diff2;
			}
		}
	}

	public static void main(String[] args) throws IOException {

		// Read from file
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

		for (int i = 0; i < n; i++) {
			line = br.readLine();
			words[i] = new Word(line);
		}

		// Vector which contains the result for every letter if it was dominant
		int[] maxNumberOfWords = new int[26];
		// Has the max of the value above, the final result
		int ultimateMax = -1;
		int temporaryMax;

		// Starts calculating the max number of words if letter 'a' + i is dominant
		for (int i = 0; i < 26; i++) {

			// Skip if the letter doesn't appear in any of the words
			if (totalLetterOccurences[i] == 0) {
				continue;
			}

			// Indicates to the compare method that we're considering letter 'a' + i dominant
			indexMaxWord = i;

			// Current max
			temporaryMax = n;
			Arrays.sort(words);

			// Calculates partial values for what has remained of the list
			float partialLength = (float) totalLength;
			float partialLetterOccurence = (float) totalLetterOccurences[i];
			int k = 0;

			// While the ratio is still bad, continues to eliminate most disavantageous words
			while (partialLetterOccurence / partialLength <= 0.5 && temporaryMax > 0) {

				// If we already are left with a smaller list than one before, 
				// there's no reason to continue
				if (temporaryMax < ultimateMax) {
					break;
				}

				partialLength = partialLength - (float)words[k].word.length();
				partialLetterOccurence = partialLetterOccurence 
							- (float)words[k].letterOccurences[i];
				temporaryMax--;
				k++;

			}

			// If the letter cant be dominant, we make it's max value in the vector -1 so
			// we could easily recognize if there cant be any dominant letter at the end
			if (temporaryMax == 0) {
				temporaryMax--;
			}
			maxNumberOfWords[i] = temporaryMax;
			
			// Compares to find the final result
			if (temporaryMax > ultimateMax) {
				ultimateMax = temporaryMax;
			}
		}

		// Writes result
		myWriter.write(ultimateMax + "\n");
		myWriter.close();
	}
}