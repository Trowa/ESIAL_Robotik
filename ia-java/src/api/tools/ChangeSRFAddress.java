package api.tools;

import java.io.IOException;

import api.hardware.RaspberryPi;
import api.sensors.SRF;

/**
 * Outil permettant de changer l'adresse d'un SRF
 */
public class ChangeSRFAddress {

	public static void main(String[] args) throws IOException {
		System.out.println("Scan en cours...");

		SRF[] results = SRF.scan(RaspberryPi.getMainI2CDev());

		for (SRF srf : results) {
			System.out.printf("SRF trouvé à l'adresse %x (%x)\n", srf.getAddress(), 2 * srf.getAddress());
		}

		if (results.length == 0) {
			System.out.println("Aucun SRF trouvé.");
		} else if (results.length > 1) {
			System.out.println("Merci de ne connecter qu'un seul SRF !");
		} else if (args.length < 1) {
			System.out.println("Pour changer l'adresse, spécifier la nouvelle adresse en argument.");
		} else {
			int newaddr = Integer.parseInt(args[0], 16);
			results[0].changeAddress(newaddr);
			System.out.println("Adresse changée !");
		}
	}
}
