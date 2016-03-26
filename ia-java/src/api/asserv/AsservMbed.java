package api.asserv;

import api.communication.Serial;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;

/**
 * Classe permettant de communiquer avec l'asservissement sur MBED et de lui
 * donner tout les ordres nécessaires une fois qu'il est bien réglé.
 * 
 * @author GaG <francois.prugniel@esial.net>
 * @author mickael9
 */

public class AsservMbed extends Asserv {
	/**
	 * Liaison série avec la MBED
	 */
	private Serial mbed;

	/**
	 * Constructeur
	 * 
	 * @param serie Nom de la liaison serie
	 * @throws UnsupportedCommOperationException 
	 * @throws NoSuchPortException 
	 * @throws PortInUseException 
	 * @throws IOException 
	 */
	public AsservMbed(String serie) throws IOException, NoSuchPortException, PortInUseException, UnsupportedCommOperationException {
		System.out.println("Connexion à l'asserv...");
		mbed = new Serial(serie, /*115200*/230400);
		mbed.sendBreak();
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("Serie ok");
		asservInit();
	}
	
	/**
	 * Envoie un ordre à la mbed
	 */
	@Override
	protected synchronized void sendCommand(String commande) {
		System.out.println("sending : " + commande);
		try {
			mbed.write(commande);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	/**
	 * Lit une ligne depuis la mbed
	 */
	@Override
	protected String readAsservOutput() {
		try {
			return mbed.readLine();
		} catch (IOException e) {
			e.printStackTrace();
			return "";
		}
	}

}
