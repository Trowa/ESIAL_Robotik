package api.asserv;

import api.asserv.actions.Action;
import api.communication.Serial;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;
import navigation.Point;

import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Classe permettant de communiquer avec l'asservissement sur MBED et de lui
 * donner tout les ordres nécessaires une fois qu'il est bien réglé.
 * 
 * @author GaG <francois.prugniel@esial.net>
 * @author mickael9
 */

public class Asserv {
	/**
	 * Liaison série avec la MBED
	 */
	private Serial mbed;
	/**
	 * Commande à executer
	 * TODO On doit pouvoir faire plus propre je pense
	 */
	private String commande; // Dernière commande

	public boolean isLastCommandFinished() {
		return lastCommandFinished;
	}

	/**
	 * Booléen signalant l'exécution complète de la dernière commande
	 */
	private boolean lastCommandFinished;
	private int lastD = 2;
	/**
	 * Position courante du robot
	 */
	private Point nous;
	
	/**
	 * Checker
	 */
	Thread checker = new Thread(new Runnable() {
		@Override
		public void run() {
			while (true) {
				String check = null;
				try {
					check = mbed.readLine();
				} catch (IOException e) {
					e.printStackTrace();
					return;
				}
				if (check.isEmpty() || check.charAt(0) != '#') {
					continue;
				}
				//nous = parseCurrentPosition(check);
				try {
					Pattern p = Pattern.compile("#x([0-9.-]+)y([0-9.-]+)a([0-9.-]+)d([0-2])");
					Matcher m = p.matcher(check);
					if (m.find()) {
						if (m.group(4).equals("1") || (lastD == 0 && m.group(4).equals("2"))) {
							System.out.println("finished !!");
							lastCommandFinished = true;
						}
						
						Point point = new Point((int)Double.parseDouble(m.group(1)), (int)Double.parseDouble(m.group(2)));
						point.setCap(Double.parseDouble(m.group(3)));
						nous = point;
						lastD = Integer.parseInt(m.group(4));
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	});

	/**
	 * Constructeur
	 * 
	 * @param serie Nom de la liaison serie
	 * @throws UnsupportedCommOperationException 
	 * @throws NoSuchPortException 
	 * @throws PortInUseException 
	 * @throws IOException 
	 */
	public Asserv(String serie) throws IOException, NoSuchPortException, PortInUseException, UnsupportedCommOperationException {
		System.out.println("Connexion à l'asserv...");
		commande = "";
		mbed = new Serial(serie, /*115200*/230400);
		mbed.sendBreak();
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("Serie ok");
		reset();
		checker.start();
	}
	
	public void reset() throws IOException {
		mbed.write('R'); // autrefois il y avait un break...
		lastCommandFinished = true;
		while(true) {
			String blabla = mbed.readLine();
			System.out.println("blabla: " + blabla);
			if (blabla.endsWith("ok")) {
				System.out.println("Asserv ready (la salope)");
				return;
			} else {
				System.out.println(blabla);
			}
		}
	}

	/**
	 * Le robot s'aligne avec la position (x,y) puis y va
	 * 
	 * @param x Abscisse en mm
	 * @param y Ordonnée en mm
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public void gotoPosition(double x, double y, boolean wait) {
		commande = "g"+x+"#"+y+"\n";
		sendCommand();
		if (wait)
			waitForFinish();
	}

	/**
	 * Le robot s'aligne avec la position (x,y) 
	 * 
	 * @param x Abscisse en mm
	 * @param y Ordonnée en mm
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public void face(double x, double y, boolean wait) {
		commande = "f"+x+"#"+y+"\n";
		sendCommand();
		if (wait)
			waitForFinish();
	}

	/**
	 * Avance tout droit (en arrière si d négatif)
	 * @param d Distance à parcourir en mm
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public void go(double d, boolean wait) {
		commande = "v"+d+"\n";
		sendCommand();
		if (wait)
			waitForFinish();
	}

	/**
	 * Tourne
	 * 
	 * @param a Angle à parcourir en degrés 
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public synchronized void turn(double a, boolean wait) {
		commande = "t"+a+"\n";
		sendCommand();
		if (wait)
			waitForFinish();
	}

	/**
	 * Arrêt du robot (en cas de détection)
	 * Désactive l'asservissement
	 */
	public void halt() {
		System.out.println("JE ME HALT");
		try {
			mbed.write("h");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Redémarre l'asservissement après un halt()
	 */
	public void resetHalt() {
		try {
			mbed.write("r");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Envoie un ordre à l'asservissement
	 */
	public synchronized void sendCommand() {
		System.out.println("sending : " + commande);
		try {
			synchronized (this) {
				mbed.write(commande);
				lastCommandFinished = false;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * On lance le calage bordure
	 * Cette commande est bloquante
	 * @param sens Sens du selecteur de couleur
	 */
	public void calageBordure(boolean sens) {
		try {
			mbed.write("c" + (sens ? "1" : "0") + "g");
			while (mbed.ready()) {
				mbed.read();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Attend que la dernière commande ait fini son exécution 
	 */
	public void waitForFinish() {
		while (!lastCommandFinished) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Renvoit un booléen signalant si la dernière commande est totalement
	 * exécuté
	 * 
	 * @return lastCommandFinished
	 */
	public boolean lastCommandFinished() {
		return lastCommandFinished;
	}
	
	public void setEnabled(boolean enabled) {
		try {
			mbed.write("D" + (enabled ? "0" : "1"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void setMotorSpeed(char moteur, int val) {
		try {
			mbed.write("M" + moteur + val + "\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public Point parseCurrentPosition(String str) {
		try {
			System.out.println(str);
			Pattern p = Pattern.compile("#x([0-9.-]+)y([0-9.-]+)a([0-9.-]+)d([0-2])");
			Matcher m = p.matcher(str);
			if (m.find()) {
				if (m.group(4).equals("1")/* || (!lastCommandFinished) && m.group(4).equals("2")*/) {
					System.out.println("finished !!");
					lastCommandFinished = true;
				}
				
				Point point = new Point((int)Double.parseDouble(m.group(1)), (int)Double.parseDouble(m.group(2)));
				point.setCap((int)Double.parseDouble(m.group(3)));
				return point;
			}
			return nous;
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Error on : " + str);
			return nous;
		}
	}
	
	public Point getCurrentPosition() {
		return this.nous;
	}

	public void addAction(Action action) {
		try {
			mbed.write(action.getSerialCommand() + "\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
