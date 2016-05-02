package api.sensors;

import ia.esialrobotik.Ia;
import navigation.Navigation;
import navigation.Point;

import java.io.IOException;

public class DetectionSRFThread extends Thread {

	protected DetectionSRF capteurGauche;
	protected DetectionSRF capteurDroit;
	protected int seuil;
	protected Ia ia;
	protected boolean detect = true;
	
	public DetectionSRFThread(int adresseCapteurGauche, int adresseCapteurDroit, int seuil, Ia ia) {
		try {
			this.capteurGauche = new DetectionSRF(adresseCapteurGauche, seuil, seuil);
			this.capteurDroit = new DetectionSRF(adresseCapteurDroit, seuil, seuil);
			this.seuil = seuil;
			this.ia = ia;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void run() {
		super.run();
		while (true) {
			try {
				int gauche = this.capteurGauche.mesure();
				int droite = this.capteurGauche.mesure();
				if (this.detect && (gauche < seuil || droite < seuil)) {
					// Est qu'on regarde hors de la table ?
					Point nous = ia.getPosition();
					
					double angle = nous.getCap();
					int min = Math.min(gauche, droite)*10;

					// Position de l'adversaire en mm
					int x = (int) (nous.getX() + Math.cos(angle) * min);
					int y = (int) (nous.getY() + Math.sin(angle) * min);
					if (x < 20  || x > 2980 || Math.abs(y) > 1980 || (ia.teamColor == Navigation.TeamColor.GREEN && y > 20) || (ia.teamColor != Navigation.TeamColor.GREEN && y < 20)) {
						System.out.println("Je vois des choses dont je me fou royalement : "+x+"-"+y);
					} else {
						System.out.println("STOOOOOOOOOOOOOOOOOOOOOOOOOOP");
						//ia.detectionAdversaire(new Point(x, y));
						// TODO: oops
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public int getSeuil() {
		return seuil;
	}

	public void setSeuil(int seuil) {
		this.seuil = seuil;
	}

	public DetectionSRF getCapteurGauche() {
		return capteurGauche;
	}

	public void setCapteurGauche(DetectionSRF capteurGauche) {
		this.capteurGauche = capteurGauche;
	}

	public DetectionSRF getCapteurDroit() {
		return capteurDroit;
	}

	public void setCapteurDroit(DetectionSRF capteurDroit) {
		this.capteurDroit = capteurDroit;
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
