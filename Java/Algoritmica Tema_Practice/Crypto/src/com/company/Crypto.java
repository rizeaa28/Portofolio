package com.company;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Crypto {
	static long MOD = (long)Math.pow(10, 9) + 7;

	public static void main(String[] args) throws IOException {

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

		long[][]dp = new long[l][n];
		int numberOfQuestionMarks = 0;

		int numberOfUniqueLetters = 0;
		int[] numberOfLetters = new int[26];
		for(int i = 0; i < l; i++) {
			numberOfLetters[s.charAt(i) - 'a']++;
			if (numberOfLetters[s.charAt(i) - 'a'] == 1)
				numberOfUniqueLetters++;
		}

		long numberOfExtraLines = 1;
		for(int j = 0; j < n; j++) {
			for (int i = 0; i < l; i++) {

				if (i > j)
					dp[i][j] = 0;

				if (k.charAt(j) == s.charAt(i)) {
					if (j == 0 && i == 0) {
						dp[i][j] = 1;
					}

					if (j > 0 && i == 0) {
						dp[i][j] = ((numberOfExtraLines + (dp[i][j-1] % MOD)) % MOD);
					}

					if (j > 0 && i > 0) {
						dp[i][j] = (((dp[i-1][j-1] % MOD) + (dp[i][j-1] % MOD)) % MOD);
					}
				} else {
					if(k.charAt(j) != '?') {
						if (j == 0 && i == 0) {
							dp[i][j] = 0;
						}

						if (j > 0 && i == 0) {
							dp[i][j] = dp[i][j-1];
						}

						if (j > 0 && i > 0) {
							dp[i][j] = dp[i][j-1];
						}
					} else {
						if (j == 0 && i == 0) {
							dp[i][j] = 1;
							//numberOfQuestionMarks++;
						}

						if (j > 0 && i == 0) {
							dp[i][j] = (((((dp[i][j-1] % MOD) * (numberOfUniqueLetters % MOD)) % MOD) + numberOfExtraLines) % MOD);
							//numberOfQuestionMarks++;
						}

						if (j > 0 && i > 0) {
							dp[i][j] = (((((dp[i][j-1] % MOD) * (numberOfUniqueLetters % MOD)) % MOD) + (dp[i-1][j-1] % MOD)) % MOD);
							//numberOfQuestionMarks++;
						}
					}
				}

			}
			if(k.charAt(j) == '?') {
				numberOfQuestionMarks++;
				numberOfExtraLines = (numberOfExtraLines * numberOfUniqueLetters) % MOD;
			}
		}


		for(int i = 0; i < l; i++) {
			for(int j = 0; j < n; j++)
				System.out.print(dp[i][j] + " ");
			System.out.println();
		}



		myWriter.write(dp[l-1][n-1] + "\n");
		myWriter.close();
		//System.out.println(MOD + "\n");
		//System.out.println(numberOfExtraLines + " " + numberOfUniqueLetters + "\n");
	}
}
