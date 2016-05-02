package api.hardware;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

/**
 * Implémentation d'un GPIO pour la Raspberry PI
 * @author jb
 *
 */
public class RaspberryPiGPIO implements GPIO {
	
	private int gpioNum;
	private boolean input;
	
	/**
	 * Un GPIO sur Rasbperry Pi
	 * @param gpioNum Le numéro du GPIO
	 * @param input si à true, le GPIO est configuré en input, sinon en output
	 * @throws IOException 
	 */
	public RaspberryPiGPIO(int gpioNum, boolean input) throws IOException {
		
		// On exporte le GPIO
		try {
			OutputStream exportFile = Files.newOutputStream(Paths.get("/sys/class/gpio/export"), StandardOpenOption.WRITE);
			exportFile.write(("" + gpioNum + "\n").getBytes());
			exportFile.close();
		} catch(IOException e) {
			// On ignore: le GPIO est déja exporté
		}
		
		// On s'assure qu'aucune interruption n'est configuré
		OutputStream edgeFile = Files.newOutputStream(Paths.get("/sys/class/gpio/gpio" + gpioNum + "/edge"), StandardOpenOption.WRITE);
		edgeFile.write(("none\n").getBytes());
		edgeFile.close();
		
		// On configure le GPIO en input ou en output
		OutputStream directionFile = Files.newOutputStream(Paths.get("/sys/class/gpio/gpio" + gpioNum + "/direction"), StandardOpenOption.WRITE);
		directionFile.write(((input ? "in" : "out") + "\n").getBytes());
		directionFile.close();
		
		this.gpioNum = gpioNum;
		this.input = input;
	}

	@Override
	public boolean getValue() {

		try {
			InputStream valueFile = Files.newInputStream(Paths.get("/sys/class/gpio/gpio" + gpioNum + "/value"), StandardOpenOption.READ);
			char value = (char) valueFile.read();
			if(value == '1')
			{
				return true;
			} else {
				return false;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
	public void setValue(boolean value) {
		// TODO A implémenter
	}

}
