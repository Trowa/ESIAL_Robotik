package api.sensors;

import ia.esialrobotik.Ia;
import navigation.Navigation;
import navigation.Point;

import java.io.IOException;
import java.util.NoSuchElementException;

public class DetectionExternalSRF04Thread extends Thread {

	protected ExternalSRF04 capteurs;
	protected SRF04JNI test;
	protected int seuil;
	protected Ia ia;
	protected boolean detect;
	
	public DetectionExternalSRF04Thread(int[] gpioIn, int[] gpioOut, int seuil, Ia ia) {
		// gpioIn : gpio d'entrée des SRF04 ==> Avant Droit, Avant milieu, Avant gauche, Arrière
		// gpioIn : gpio de sortie des SRF004 ==> Avant Droit, Avant milieu, Avant gauche, Arrière
//		try {
			String home = System.getenv().get("HOME");
			//this.capteurs = new ExternalSRF04(home + "/srf04/srf04", gpioIn, gpioOut);
			this.test = new SRF04JNI(17, 27);
			this.seuil = seuil;
			this.ia = ia;
			this.detect = true;
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
	}
	
	@Override
	public void run() {
		super.run();
		while (true) {
			System.out.println("YOLOOOOOOOOOOOOOOOOOOOOOOOOO : " + this.test.getMeasure());
			try {
				Thread.sleep(12);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			// mesures[0] = Avant droit
			// mesures[1] = Avant milieu
			// mesures[2] = Avant gauche
			// mesures[3] = Arrière
//			int[] mesures = capteurs.getMesures();
//			int droite = mesures[0];
//			int milieu = mesures[1];
//			int gauche = mesures[2];
//			int arriere = mesures[3];
//
//			if (this.detect) {
//
//				// Est qu'on regarde hors de la table ?
//				Point nous = ia.getPosition();
//				double angle = nous.getCap();
//				boolean detected = false;
//				int x = 0, y = 0;
//
//				if (false && this.ia.asserv.isBackward()) { // En marche arrière
//					if (arriere < seuil) {
//						// Position de l'adversaire en mm
//						x = (int) (nous.getX() - 130 - Math.cos(angle) * arriere); // 130mm = décallage du X  entre point du robot et capteur
//						y = (int) (nous.getY() - Math.sin(angle) * arriere);
//						detected = !this.iDontGiveAFuckOfDetection(x, y);
//						System.out.println("Detection arrière : " + x + "-" + y + ", osef ? " + detected);
//					}
//				} else { // En marche avant
//					if (droite < seuil) {
//						x = (int) (nous.getX() + 130 + Math.cos(angle - Math.PI/6) * droite); // 130mm = décallage du X  entre point du robot et capteur
//						y = (int) (nous.getY() - 140 + Math.sin(angle - Math.PI/6) * droite); // 140mm = décallage du Y  entre point du robot et capteur
//						detected = !this.iDontGiveAFuckOfDetection(x, y);
//						System.out.println("Detection avant droite : " + x + "-" + y + ", osef ? " + detected);
//						if (detected) {
//							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
//							ia.detectionAdversaire(new Point(x, y));
//						}
//					}
//					if (milieu < seuil) {
//						x = (int) (nous.getX() + 130 + Math.cos(angle) * milieu);  // 130mm = décallage du X  entre point du robot et capteur
//						y = (int) (nous.getY() + Math.sin(angle) * milieu);
//						detected = !this.iDontGiveAFuckOfDetection(x, y);
//						System.out.println("Detection avant milieu : " + x + "-" + y + ", osef ? " + detected);
//						if (detected) {
//							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
//							ia.detectionAdversaire(new Point(x, y));
//						}
//					}
//					if (gauche < seuil) {
//						x = (int) (nous.getX() + 130 + Math.cos(angle + Math.PI/6) * gauche); // 130mm = décallage du X  entre point du robot et capteur
//						y = (int) (nous.getY() + 140 + Math.sin(angle + Math.PI/6) * gauche); // 140mm = décallage du Y  entre point du robot et capteur
//						detected = !this.iDontGiveAFuckOfDetection(x, y);
//						System.out.println("Detection avant gauche : " + x + "-" + y + ", osef ? " + detected);
//						if (detected) {
//							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
//							ia.detectionAdversaire(new Point(x, y));
//						}
//					}
//				}
//			}
		}
	}

	private boolean iDontGiveAFuckOfDetection(int x, int y) {
		return x < 100  || x > 2900 || Math.abs(y) > 1900
				|| (ia.ymult == -1 && y > -100) || (ia.ymult == 1 && y < 100) // faut faire attention si on voit du -100 avec des y positif par exemple
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
