package com.company;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;


public class Prinel {

    static int max(int a, int b) {
        if(a > b)
            return a;
        else
            return b;
    }

    static ArrayList<Integer> divisorVector(int n) {
        ArrayList<Integer> v = new ArrayList<>();
        for (int i = 1; i <= Math.sqrt(n); i++) {
            if (n % i == 0) {
                    v.add(i);
                    if(n/i != i)
                        v.add(n/i);
                }
            }

        return v;
    }


    public static void main(String[] args) throws IOException {

        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader("prinel.in"));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        FileWriter myWriter = new FileWriter("prinel.out");

        String line = br.readLine();
        int n = Integer.parseInt(line.split(" ")[0]);
        int k = Integer.parseInt(line.split(" ")[1]);

        line = br.readLine();
        int[] target = new int[n];
        int[] a = new int[n];
        int[] p = new int[n];
        int[] steps = new int[n];

        for (int i = 0; i < n; i++) {
            target[i] = Integer.parseInt(line.split(" ")[i]);
            a[i] = 1;
        }

        line = br.readLine();
        for (int i = 0; i < n; i++) {
            p[i] = Integer.parseInt(line.split(" ")[i]);
        }

        int maxTarget = target[0];
        for (int i = 1; i < n; i++) {
            maxTarget = max(maxTarget, target[i]);
        }


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

        int maxiSteps = stepsVector[target[0]];
        int minSteps = stepsVector[target[0]];
        for(int i = 0; i < n; i++) {
            steps[i] = stepsVector[target[i]];
            if(minSteps > steps[i]) {
                minSteps = steps[i];
            }

            if(maxiSteps < steps[i]) {
                maxiSteps = steps[i];
            }
        }


        System.out.println();
        for(int i = 0; i < n; i++) {
            System.out.print(steps[i] + " ");
        }
        System.out.println();

        System.out.println();
        for(int i = 0; i < n; i++) {
            System.out.print(target[i] + " ");
        }
        System.out.println();

        System.out.println();
        for(int i = 0; i < n; i++) {
            System.out.print(p[i] + " ");
        }
        System.out.println();
        System.out.println();



        int[][] dp = new int[n+1][k+1];

        for (int i = 0; i <= k; i++) {
            dp[0][i] = 0;
        }

        int opt = 1;
        int dif_min = -1;
        minSteps = steps[0];
        for (int i = 1; i <= n; i++) {
            System.out.print(dif_min + " ");

            //if(dif_min > p[i-1] && opt == 1) {
                //opt++;
                //continue;
            //}

            dif_min = -1;
            if(k < steps[i-1]) {
                opt++;
                continue;
            }

            k = Math.min(k, 12*n);

            for (int maxSteps = k; maxSteps >= minSteps; maxSteps--) {
                if(maxSteps < steps[i-1] && dp[i-opt][maxSteps] == 0)
                    break;

                if (maxSteps - steps[i - 1] >= 0) {
                    dp[i][maxSteps] = max( dp[i-opt][maxSteps], dp[i-opt][maxSteps - steps[i - 1]] + p[i-1]);
                } else {
                    dp[i][maxSteps] = dp[i-opt][maxSteps];
                }

                if(i < n && maxSteps + steps[i] <= k) {
                    if (dif_min == -1) {
                        dif_min = dp[i][maxSteps + steps[i]] - dp[i][maxSteps];
                    }
                    else {
                        if(dif_min > (dp[i][maxSteps + steps[i]] - dp[i][maxSteps]))
                            dif_min = dp[i][maxSteps + steps[i]] - dp[i][maxSteps];
                    }
                }

                if(steps[i-1] < minSteps)
                    minSteps = steps[i-1];
            }
            opt = 1;
        }

        System.out.println();


        /*
        for(int i = 0; i <=n; i++) {
            for(int j = 0; j <= k; j++)
                System.out.print(dp[i][j] + " ");
            System.out.println();
        }

         */



        myWriter.write(dp[n - opt + 1][k] + "\n");
        System.out.println(dp[n - opt + 1][k]);
        myWriter.close();
        System.out.println(maxiSteps);
    }
}
