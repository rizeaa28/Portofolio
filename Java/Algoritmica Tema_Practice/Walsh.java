import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Walsh {

	public static void main(String[] args) throws IOException {
		
		// Read from file
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader("walsh.in"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		FileWriter myWriter = new FileWriter("walsh.out");

		String line = br.readLine();
		String[] elementsInLine = line.split(" ");
		int n = Integer.parseInt(elementsInLine[0]);
		int k = Integer.parseInt(elementsInLine[1]);

		// (sx, sy) is the top left corner of the matrix
		// (dx, dy) is the bottom right corner of the matrix
		int sx, sy;
		int dx, dy;

		// Coordinates of final value
		int x, y;
		
		// Final value initialisaiton
		boolean value;

		for (int i = 1; i <= k; i++) {
			
			// Initialise value for every set of coordinates
			value = false;
			line = br.readLine();
			elementsInLine = line.split(" ");
			y = Integer.parseInt(elementsInLine[0]) - 1;
			x = Integer.parseInt(elementsInLine[1]) - 1;

			sx = 0;
			sy = 0;
			dx = n - 1;
			dy = n - 1;

			// While we haven't reached having just one box
			while (sx != dx) {

				// Determines middle
				int half_length = (dx - sx + 1) / 2;

				// Top-left matrix
				if (x < half_length && y < half_length) {
					dx = dx - half_length;
					dy = dy - half_length;
				}

				// Top-right matrix
				if (x >= half_length && y < half_length) {
					x = x - half_length;
					sx = sx + half_length;
					dy = dy - half_length;
				}

				// Bottom-left matrix
				if (x < half_length && y >= half_length) {
					y = y - half_length;
					sy = sy + half_length;
					dx = dx - half_length;
				}

				// Bottom-right matrix => negates!
				if (x >= half_length && y >= half_length) {
					x = x - half_length;
					y = y - half_length;
					sx = sx + half_length;
					sy = sy + half_length;
					value = !value;
				}
			}

			if (value == true) {
				myWriter.write("1\n");
			} else {
				myWriter.write("0\n");
			}
		}
		myWriter.close();
	}
}