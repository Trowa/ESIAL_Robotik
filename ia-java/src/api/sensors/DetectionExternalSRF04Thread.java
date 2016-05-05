package api.sensors;

import ia.esialrobotik.Ia;
import navigation.Navigation;
import navigation.Point;

import java.io.IOException;
import java.util.NoSuchElementException;

public class DetectionExternalSRF04Thread extends Thread {

	protected SRF04JNI srf04AvantGauche, srf04AvantMilieu, srf04AvantDroit, srf04Arriere;
	protected int seuil;
	protected Ia ia;
	protected boolean detect;
	
	public DetectionExternalSRF04Thread(SRF04JNI srf04AvantGauche, SRF04JNI srf04AvantMilieu, SRF04JNI srf04AvantDroit, SRF04JNI srf04Arriere, int seuil, Ia ia) {
		this.srf04AvantGauche = srf04AvantGauche;
		this.srf04AvantMilieu = srf04AvantMilieu;
		this.srf04AvantDroit = srf04AvantDroit;
		this.srf04Arriere = srf04Arriere;
		this.seuil = seuil;
		this.ia = ia;
		this.detect = true;
	}
	
	@Override
	public void run() {
		super.run();
		while (true) {
			try {
				Thread.sleep(12);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			if (this.detect) {
				// Est qu'on regarde hors de la table ?
				Point nous = ia.getPosition();
				double angle = nous.getCap();
				boolean detectedArriere  = false;
				boolean detectedAvantDroit = false;
				boolean detectedAvantMilieu = false;
				boolean detectedAvantGauche = false;
				int x = 0, y = 0;

				if (false && this.ia.asserv.isBackward()) { // En marche arrière
					int arriere = (int)this.srf04Arriere.getMeasure();
					if (arriere < seuil) {
						// Position de l'adversaire en mm
						x = (int) (nous.getX() - 130 - Math.cos(angle) * arriere); // 130mm = décallage du X  entre point du robot et capteur
						y = (int) (nous.getY() - Math.sin(angle) * arriere);
						detectedArriere = !this.iDontGiveAFuckOfDetection(x, y);
						System.out.println("Detection arrière : " + x + "-" + y + ", osef ? " + !detectedArriere);
						if (detectedArriere) {
							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
							ia.detectionAdversaire(new Point(x, y));
						}
					}
				} else { // En marche avant
					int droite = (int)this.srf04AvantDroit.getMeasure();
					if (droite < seuil) {
						x = (int) (nous.getX() + 130 + Math.cos(angle - Math.PI/6) * droite); // 130mm = décallage du X  entre point du robot et capteur
						y = (int) (nous.getY() - 140 + Math.sin(angle - Math.PI/6) * droite); // 140mm = décallage du Y  entre point du robot et capteur
						detectedAvantDroit = !this.iDontGiveAFuckOfDetection(x, y);
						System.out.println("Detection avant droite : " + x + "-" + y + ", osef ? " + !detectedAvantDroit);
						if (detectedAvantDroit) {
							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
							ia.detectionAdversaire(new Point(x, y));
						}
					}
					try {
						Thread.sleep(24);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					int milieu = (int)this.srf04AvantMilieu.getMeasure();
					if (milieu < seuil) {
						x = (int) (nous.getX() + 130 + Math.cos(angle) * milieu);  // 130mm = décallage du X  entre point du robot et capteur
						y = (int) (nous.getY() + Math.sin(angle) * milieu);
						detectedAvantMilieu = !this.iDontGiveAFuckOfDetection(x, y);
						System.out.println("Detection avant milieu : " + x + "-" + y + ", osef ? " + !detectedAvantMilieu);
						if (detectedAvantMilieu) {
							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
							ia.detectionAdversaire(new Point(x, y));
						}
					}
					try {
						Thread.sleep(24);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
//					int gauche = (int)this.srf04AvantGauche.getMeasure();
//					if (gauche < seuil) {
//						x = (int) (nous.getX() + 130 + Math.cos(angle + Math.PI/6) * gauche); // 130mm = décallage du X  entre point du robot et capteur
//						y = (int) (nous.getY() + 140 + Math.sin(angle + Math.PI/6) * gauche); // 140mm = décallage du Y  entre point du robot et capteur
//						detectedAvantGauche = !this.iDontGiveAFuckOfDetection(x, y);
//						System.out.println("Detection avant gauche : " + x + "-" + y + ", osef ? " + !detectedAvantGauche);
//						if (detectedAvantGauche) {
//							System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
//							ia.detectionAdversaire(new Point(x, y));
//						}
//					}
//					try {
//						Thread.sleep(24);
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
					if (!detectedArriere && !detectedAvantDroit && !detectedAvantMilieu && !detectedAvantGauche) {
						ia.pasdadversaire();
					}
				}
			}
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
