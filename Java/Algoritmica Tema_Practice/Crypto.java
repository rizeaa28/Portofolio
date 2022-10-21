
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Crypto {
	static long MOD = (long) Math.pow(10, 9) + 7;

	public static void main(String[] args) throws IOException {

		// Reading from file
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader("crypto.in"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		FileWriter myWriter = new FileWriter("crypto.out");

		String line = br.readLine();
		int n = Integer.parseInt(line.split(" ")[0]);
		int l = Integer.parseInt(line.split(" ")[1]);

		String k = br.readLine();
		String s = br.readLine();

		// Initialise dp matrix 
		long[][]dp = new long[l][n];

		// Count the number of unique letters in string s
		int numberOfUniqueLetters = 0;
		int[] numberOfLetters = new int[26];
		for (int i = 0; i < l; i++) {
			numberOfLetters[s.charAt(i) - 'a']++;
			if (numberOfLetters[s.charAt(i) - 'a'] == 1) {
				numberOfUniqueLetters++;
			}
		}

		// Initialise the number of extra lines (explained in README)
		long numberOfExtraLines = 1;

		// Start iterating in dp
		for (int j = 0; j < n; j++) {
			for (int i = 0; i < l; i++) {

				// If the string s or it's substring would not fit 
				//(i.e. were at index 5 in string k but we're looking for a substring with length 6)
				if (i > j) {
					dp[i][j] = 0;
				}
					
				// If the k[j] is the same as s[i]
				if (k.charAt(j) == s.charAt(i)) {
					// Top-left initialisation
					if (j == 0 && i == 0) {
						dp[i][j] = 1;
					}

					// First line
					if (j > 0 && i == 0) {
						dp[i][j] = ((numberOfExtraLines + (dp[i][j - 1] % MOD)) % MOD);
					}

					// Rest of lines
					if (j > 0 && i > 0) {
						dp[i][j] = (((dp[i - 1][j - 1] % MOD) + (dp[i][j - 1] % MOD)) % MOD);
					}
				} else {
					// If the k[j] is not s[i] and neither '?'
					if (k.charAt(j) != '?') {
						// Top-left initialisation
						if (j == 0 && i == 0) {
							dp[i][j] = 0;
						}

						// First line
						if (j > 0 && i == 0) {
							dp[i][j] = dp[i][j - 1];
						}

						// Rest of lines
						if (j > 0 && i > 0) {
							dp[i][j] = dp[i][j - 1];
						}
					} else {
						// If k[j] is '?'
						// Top-left initialisation
						if (j == 0 && i == 0) {
							dp[i][j] = 1;
						}

						// First line
						if (j > 0 && i == 0) {
							dp[i][j] = (((((dp[i][j - 1]) * (numberOfUniqueLetters)) % MOD) 
										+ numberOfExtraLines) % MOD);
						}

						// Rest of lines
						if (j > 0 && i > 0) {
							dp[i][j] = (((((dp[i][j - 1]) * (numberOfUniqueLetters)) % MOD) 
										+ (dp[i - 1][j - 1])) % MOD);
						}
					}
				}

			}

			// Count the number of extra lines (explained in README)
			if (k.charAt(j) == '?') {
				numberOfExtraLines = (numberOfExtraLines * numberOfUniqueLetters) % MOD;
			}
		}

		// Write the answer
		myWriter.write(dp[l - 1][n - 1] + "\n");
		myWriter.close();
	}
}
