import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class Prinel {

	// Calculates max between 2 numbers
	static int max(int a, int b) {
		if (a > b) {
			return a;
		} else {
			return b;
		}
	}

	// Returns array with the divisors of a number
	static ArrayList<Integer> divisorVector(int n) {
		ArrayList<Integer> v = new ArrayList<Integer>();
		for (int i = 1; i <= Math.sqrt(n); i++) {
			if (n % i == 0) {
				v.add(i);
				if (n / i != i) {
					v.add(n / i);
				}
			}
		}
		return v;
	}


	public static void main(String[] args) throws IOException {

		// Reading from file
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader("prinel.in"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		String line = br.readLine();
		int n = Integer.parseInt(line.split(" ")[0]);
		int k = Integer.parseInt(line.split(" ")[1]);

		// Optimisation explained in README
		k = Math.min(k, 22 * n);

		line = br.readLine();
		int[] target = new int[n];
		int[] a = new int[n];
		int[] p = new int[n];
		int[] steps = new int[n];

		String[] splitLine = line.split(" ");
		for (int i = 0; i < n; i++) {
			target[i] = Integer.parseInt(splitLine[i]);
			a[i] = 1;
		}

		line = br.readLine();
		splitLine = line.split(" ");
		for (int i = 0; i < n; i++) {
			p[i] = Integer.parseInt(splitLine[i]);
		}

		// Calculating the max number in target
		int maxTarget = target[0];
		for (int i = 1; i < n; i++) {
			maxTarget = max(maxTarget, target[i]);
		}

		// Calculates the steps needed for every number up until the maxTarget
		int[] stepsVector = new int[maxTarget + 1];

		for (int i = 1; i <= maxTarget; i++) {
			ArrayList<Integer> divs = divisorVector(i);

			for (int j = 0; j < divs.size(); j++) {
				if (i + divs.get(j) <= maxTarget) {
					if (stepsVector[i + divs.get(j)] == 0) {
						stepsVector[i + divs.get(j)] = stepsVector[i] + 1;
					} else {
						if (stepsVector[i + divs.get(j)] > stepsVector[i] + 1) {
							stepsVector[i + divs.get(j)] = stepsVector[i] + 1;
						}
					}
				}
			}
		}

		// Puts in the steps vector the value of steps calculated before for 
		// every target[i] on the same index
		for (int i = 0; i < n; i++) {
			steps[i] = stepsVector[target[i]];
		}

		int[][] dp = new int[n + 1][k + 1];

		// DP type "Rucsac" like in the lab
		for (int i = 0; i <= k; i++) {
			dp[0][i] = 0;
		}

		for (int i = 1; i <= n; i++) {
			for (int maxSteps = k; maxSteps >= 0; maxSteps--) {

				if (maxSteps - steps[i - 1] >= 0) {
					dp[i][maxSteps] = max(dp[i - 1][maxSteps], 
								dp[i - 1][maxSteps - steps[i - 1]] + p[i - 1]);
				} else {
					dp[i][maxSteps] = dp[i - 1][maxSteps];
				}
			}
		}

		// Write output
		FileWriter myWriter = new FileWriter("prinel.out");
		myWriter.write(dp[n][k] + "\n");
		myWriter.close();
	}
}