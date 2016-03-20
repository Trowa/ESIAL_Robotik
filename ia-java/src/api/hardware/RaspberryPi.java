package api.hardware;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Permet d'obtenir des informations sur le matériel de la Raspberry Pi
 */
public abstract class RaspberryPi {
	/**
	 * Renvoie la révision matérielle de la Raspberry Pi
	 * 
	 * @throws IOException
	 */
	public static int getHardwareVersion() throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader("/proc/cpuinfo"));
		Pattern patt = Pattern.compile("^Revision\\s*:\\s*([0-9a-f]+)");
		String line;
		int val = -1;

		do {
			line = reader.readLine();
			Matcher match = patt.matcher(line);

			if (match.matches()) {
				val = Integer.parseInt(match.group(1), 16);
				break;
			}
		} while (line != null);

		reader.close();

		return val;
	}

	/**
	 * Renvoie le numéro de l'adaptateur I2C par défaut cablé sur les pins 3 et
	 * 5
	 * <p>
	 * Cela est utile à cause de l'inversion qui a été faite à partir de la
	 * révision 2.0 de la Raspberry Pi
	 */
	public static int getMainI2CDev() {
		try {
			return (getHardwareVersion() >= 4 ? 1 : 0);
		} catch (IOException e) {
			e.printStackTrace();
			return 1;
		}
	}
}
