/**
 * Description:
 * The Lab1 class reads tokens from a file "data.txt" and prints out
 * three values; the number of tokens that are numbers(doubles)
 * ,the number of tokens that are not numbers, and the sum of the
 * numbers.
 * 
 * Sample file input:
 * 1 2 3 one two three
 * 
 * Sample output:
 * 3 3 6
 *
 * @author Carla Ramirez (hua647)
 *
 */
import java.util.Scanner;
import java.io.File;

public class Lab1 {
    private static int count1 = 0; // counter for number of numbers
    private static int count2 = 0; // counter for number of words
    private static double sum = 0; // variable to return sum of all numbers

    /**
     * The main method handles the reading in file and parsing the
     * tokens it also handles error for failing to open file. The main method
     * calculates values and prints those to console.
     * 
     * 
     * @param args  variable for arguments input of type String
     *           
     */
    public static void main(String[] args) {
        Scanner input = null; // scanner object to read file
        Scanner tokeninput = null; // scanner object to parse tokens
        try {
            input = new Scanner(new File("data.txt"));
        } catch (Exception FileNotFoundException) {
            System.err.println("Failed to open data.txt");
            System.exit(1);
        }// continue reading in tokens until EOF
        while (input.hasNext()) {
            String token = null;
            token = input.next();
            // check if token is a double
            tokeninput = new Scanner(token);
            if (tokeninput.hasNextDouble()) {
                double d = Double.parseDouble(token);
                count1++;
                sum = sum + d;
            } else {
                count2++;
            }
        }
        System.out.printf("%d  %d  %.1f", count1, count2, sum);

        input.close(); // close scanner object
        tokeninput.close(); // close scanner object
    }
}
