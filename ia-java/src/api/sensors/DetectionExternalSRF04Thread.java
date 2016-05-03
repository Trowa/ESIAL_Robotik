package api.sensors;

import ia.esialrobotik.Ia;
import navigation.Navigation;
import navigation.Point;

import java.io.IOException;
import java.util.NoSuchElementException;

public class DetectionExternalSRF04Thread extends Thread {

	protected ExternalSRF04 capteurs;
	protected int seuil;
	protected Ia ia;
	protected boolean detect;
	
	public DetectionExternalSRF04Thread(int[] gpioIn, int[] gpioOut, int seuil, Ia ia) {
		// gpioIn : gpio d'entrée des SRF04 ==> Avant Droit, Avant milieu, Avant gauche, Arrière
		// gpioIn : gpio de sortie des SRF004 ==> Avant Droit, Avant milieu, Avant gauche, Arrière
		try {
			String home = System.getenv().get("HOME");

			boolean detectorInitialised = false;
			while (!detectorInitialised) {
				try {
					this.capteurs = new ExternalSRF04(home + "/srf04/srf04", gpioIn, gpioOut);
					int[] mesures = capteurs.getMesures();
					detectorInitialised = true;
				} catch (NoSuchElementException e) {
					// Fail, on recommence
				}
			}

			this.seuil = seuil;
			this.ia = ia;
			this.detect = true;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void run() {
		super.run();
		while (true) {
			// mesures[0] = Avant droit
			// mesures[1] = Avant milieu
			// mesures[2] = Avant gauche
			// mesures[3] = Arrière
			int[] mesures = capteurs.getMesures();
			int droite = mesures[0];
			int milieu = mesures[1];
			int gauche = mesures[2];
			int arriere = mesures[3];

			System.out.println("Detect : " + gauche);

			if (this.detect) {

				// Est qu'on regarde hors de la table ?
				Point nous = ia.getPosition();
				double angle = nous.getCap();
				boolean detected = false;

				if (this.ia.asserv.isBackward()) { // En marche arrière
					if (arriere < seuil) {
						// Position de l'adversaire en mm
						int x = (int) (nous.getX() - 130 - Math.cos(angle) * arriere); // 130mm = décallage du X  entre point du robot et capteur
						int y = (int) (nous.getY() - Math.sin(angle) * arriere);
						System.out.println("Detection arrière : " + x + "-" + y);
						detected = this.iDontGiveAFuckOfDetection(x, y);
					}
				} else { // En marche avant
					if (droite < seuil) {
						int x = (int) (nous.getX() + 130 + Math.cos(angle - Math.PI/6) * droite); // 130mm = décallage du X  entre point du robot et capteur
						int y = (int) (nous.getY() - 140 + Math.sin(angle - Math.PI/6) * droite); // 140mm = décallage du Y  entre point du robot et capteur
						System.out.println("Detection avant droite : " + x + "-" + y);
						detected = this.iDontGiveAFuckOfDetection(x, y);
					}
					if (milieu < seuil) {
						int x = (int) (nous.getX() + 130 + Math.cos(angle) * milieu);  // 130mm = décallage du X  entre point du robot et capteur
						int y = (int) (nous.getY() + Math.sin(angle) * milieu);
						System.out.println("Detection avant milieu : " + x + "-" + y);
						detected = this.iDontGiveAFuckOfDetection(x, y);
					}
					if (gauche < seuil) {
						int x = (int) (nous.getX() + 130 + Math.cos(angle + Math.PI/6) * gauche); // 130mm = décallage du X  entre point du robot et capteur
						System.out.println("x = " + nous.getX() + "+" + 130 + "+" + (Math.cos(angle + Math.PI/6) * gauche));
						int y = (int) (nous.getY() + 140 + Math.sin(angle + Math.PI/6) * gauche); // 140mm = décallage du Y  entre point du robot et capteur
						System.out.println("y = " + nous.getY() + "+" + 140 + "+" + (Math.sin(angle + Math.PI/6) * gauche));
						System.out.println("Detection avant gauche : " + x + "-" + y);
						detected = this.iDontGiveAFuckOfDetection(x, y);
					}

				}

				if (detected) {
					System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
					//ia.detectionAdversaire(new Point(x, y));
					// TODO: oops
				}
			}
		}
	}

	private boolean iDontGiveAFuckOfDetection(int x, int y) {
		return x < 50  || x > 2950 || Math.abs(y) > 1950
				|| (ia.teamColor == Navigation.TeamColor.GREEN && y > 20) || (ia.teamColor != Navigation.TeamColor.GREEN && y < 20) // faut faire attention si on voit du -100 avec des y positif par  exemple
				|| (x > 850 && x < 2150 && Math.abs(y) > 700 && Math.abs(y) < 1400);
	}

	public int getSeuil() {
		return seuil;
	}

	public void setSeuil(int seuil) {
		this.seuil = seuil;
	}

	public Ia getIa() {
		return ia;
	}

	public void setIa(Ia ia) {
		this.ia = ia;
	}

	public boolean isDetect() {
		return detect;
	}

	public void setDetect(boolean detect) {
		this.detect = detect;
	}
	
}
