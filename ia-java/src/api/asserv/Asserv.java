package api.asserv;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import api.asserv.actions.Action;
import navigation.Point;

/**
 * Interface pour discuter avec une asserv
 * Le protocole de communication est définie par cette classe, mais
 * la communication avec une vraie asserv doit être codée dans une classe fille
 * @author jb
 * 
 * TODO vérifier les sections critiques à protéger (mutex)
 *
 */

public abstract class Asserv {
	
	/**
	 * Booléen signalant l'exécution complète de la dernière commande
	 */
	protected boolean lastCommandFinished;

	/**
	 * Position courante du robot
	 */
	protected Point nous;
	
	/**
	 * Thread pour parser la sortie de l'asserv
	 */
	Thread checker = new Thread(new Runnable() {
		@Override
		public void run() {
			int lastD = 2;
			
			while (true) {
				String check = null;
				// Lit une ligne
				try {
					check = readAsservOutput();
				} catch (Exception e) {
					e.printStackTrace();
					return;
				}
				
				// La ligne contient-elle des trucs intéressants ?
				if (check.isEmpty() || check.charAt(0) != '#') {
					continue;
				}
				
				// On parse la ligne
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
	 * Initialise l'asserv
	 * Le constructeur de la classe fille doit initialiser
	 * tout ce qu'il faut pour communiquer avec l'asserv,
	 * puis appeler cette méthode
	 */
	protected void asservInit() {
		reset();
		checker.start();
	}
	
	/**
	 * Renvoit un booléen signalant si la dernière commande est totalement
	 * exécuté
	 * 
	 * @return lastCommandFinished
	 */
	public boolean isLastCommandFinished() {
		return lastCommandFinished;
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
	 * Le robot s'aligne avec la position (x,y) puis y va
	 * 
	 * @param x Abscisse en mm
	 * @param y Ordonnée en mm
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public void gotoPosition(double x, double y, boolean wait) {
		String commande = "g"+x+"#"+y+"\n";
		synchronized(this) {
			sendCommand(commande);
			lastCommandFinished = false;
		}
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
		String commande = "f"+x+"#"+y+"\n";
		synchronized(this) {
			sendCommand(commande);
			lastCommandFinished = false;
		}
		if (wait)
			waitForFinish();
	}

	/**
	 * Avance tout droit (en arrière si d négatif)
	 * @param d Distance à parcourir en mm
	 * @param wait Attendre que la commande soit terminée avant de retourner
	 */
	public void go(double d, boolean wait) {
		String commande = "v"+d+"\n";
		synchronized(this) {
			sendCommand(commande);
			lastCommandFinished = false;
		}
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
		String commande = "t"+a+"\n";
		synchronized(this) {
			sendCommand(commande);
			lastCommandFinished = false;
		}
		if (wait)
			waitForFinish();
	}
	
	/**
	 * Position de notre robot renvoyé par l'asserv
	 * @return La position du robot
	 */
	public Point getCurrentPosition() {
		return this.nous;
	}

	/**
	 * Met une action en file d'attente coté asserv
	 * @param action L'action à ajouter
	 */
	public void addAction(Action action) {
		sendCommand(action.getSerialCommand() + "\n");
	}
	
	/**
	 * Arrêt du robot (en cas de détection)
	 * Désactive l'asservissement
	 */
	public void halt() {
		System.out.println("JE ME HALT");
		sendCommand("h");
	}
	
	/**
	 * Redémarre l'asservissement après un halt()
	 */
	public void resetHalt() {
		sendCommand("r");
	}

	public void setEnabled(boolean enabled) {
		sendCommand("D" + (enabled ? "0" : "1"));
	}
	
	public void setMotorSpeed(char moteur, int val) {
		sendCommand("M" + moteur + val + "\n");
	}
	
	/**
	 * Reset complet de l'asserv
	 */
	public void reset() {
		sendCommand("R"); // autrefois il y avait un break...
		lastCommandFinished = true;
		while(true) {
			String blabla = readAsservOutput();
			System.out.println("blabla: " + blabla);
			if (blabla.endsWith("ok")) {
				System.out.println("Asserv ready (la salope)");
				return;
			}
		}
	}
	
	/**
	 * On lance le calage bordure
	 * Cette commande est bloquante
	 * @param sens Sens du selecteur de couleur
	 */
	public void calageBordure(boolean sens) {
		sendCommand("c" + (sens ? "1" : "0") + "g");
	}
	
	/**
	 * Envoie la commande, à implémenter dans une classe fille
	 * @param commande
	 */
	protected abstract void sendCommand(String commande);
	
	/**
	 * Lit une ligne renvoyé par l'asserv, à implémenter dans une classe fille
	 * @return la ligne lue
	 */
	protected abstract String readAsservOutput();
	
}
