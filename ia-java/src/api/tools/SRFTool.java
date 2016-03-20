package api.tools;

import java.io.IOException;

import api.hardware.RaspberryPi;
import api.sensors.SRF;

/**
 * Outil permettant d'interroger un SRF (en réglant éventuellement la portée et
 * le gain)
 */
public class SRFTool {

	public static void main(String[] args) throws IOException {
		System.out.println("Utilisation : java srf.jar <adresse hexa> [<max range> [<max gain>]]");
		System.out.println();

		int addr = Integer.parseInt(args[0], 16);

		SRF srf = new SRF(RaspberryPi.getMainI2CDev(), addr);

		if (args.length > 1) {
			srf.setMaxRange(Integer.parseInt(args[1]));
		}

		if (args.length > 2) {
			srf.setMaxGain(Integer.parseInt(args[2]));
		}

		while (true) {
			System.out.println(srf.getCentimeters(100));
		}
	}
}
