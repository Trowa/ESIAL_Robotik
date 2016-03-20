package api.sensors;

import java.io.IOException;
import java.util.ArrayList;

import api.hardware.RaspberryPi;

public class DetectionSRF {
	private static int NB_ECH = 3;
	
	private SRF srf;
	private int seuilArret;
	private int seuilDepart;
	private ArrayList<Integer> echantillons = new ArrayList<Integer>();

	public DetectionSRF(int i2caddr, int maxRange, int maxGain, int seuilArret, int seuilDepart) throws IOException {
		this(i2caddr, seuilArret, seuilDepart);

		this.srf.setMaxGain(maxGain);
		this.srf.setMaxRange(maxRange);
	}

	public DetectionSRF(int i2caddr, int seuilArret, int seuilDepart) throws IOException {
		this.srf = new SRF(RaspberryPi.getMainI2CDev(), i2caddr);
		this.seuilArret = seuilArret;
		this.seuilDepart = seuilDepart;
	}
	
	public SRF getSRF() {
		return this.srf;
	}
	
	public int moyenne() {
		int somme = 0;
		
		for (int ech : echantillons) {
			somme += ech;
		}
		
		return somme / echantillons.size();
	}
	
	public int mesure() throws IOException {
		int mesure = srf.getCentimeters(80);
		
		if (mesure == 0)
			mesure = 100;
		
		echantillons.add(mesure);

		while (echantillons.size() > NB_ECH) {
			echantillons.remove(0);
		}
		return mesure;
	}

	public boolean peutRepartir() throws IOException {
		mesure();
		
		return moyenne() > this.seuilDepart;
	}

	public boolean doitStopper() throws IOException {
		return (mesure() < this.seuilArret);
	}
}
