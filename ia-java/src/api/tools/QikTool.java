package api.tools;

import api.communication.Serial;
import api.controllers.qik.Qik2s9v1;

public class QikTool {
	public static void main(String[] args) throws Exception {
		System.out.println("--- Test Qik2s9v1 ---");
		System.out.println("Arguments : voie (0 ou 1) puis consignes à appliquer à M0 (entre -127 et 127)");
		System.out.println("Chaque consigne est maintenue pendant 1 seconde");
		System.out.println();

		System.out.println("Ouverture port série...");
		Serial serial = new Serial("/dev/ttyAMA0", 38400);

		System.out.println("Initialisation de la Qik2s9v1...");
		Qik2s9v1 qik2s9v1 = new Qik2s9v1(serial);

		System.out.println("Erreur courante (binaire) : " + Integer.toBinaryString(qik2s9v1.getErrors()));
		System.out.println("Version firmware : " + qik2s9v1.getFirmwareVersion());
		System.out.println("ID carte : " + qik2s9v1.getConfigurationParameter(Qik2s9v1.QIK_CONFIG_DEVICE_ID));
		System.out.println("Config PWM : " + qik2s9v1.getConfigurationParameter(Qik2s9v1.QIK_CONFIG_PWM_PARAMETER));
		System.out.println("Arrêt moteurs en cas d'erreur : " + qik2s9v1.getConfigurationParameter(Qik2s9v1.QIK_CONFIG_SHUT_DOWN_MOTORS_ON_ERROR));
		System.out.println("Timeout série : " + qik2s9v1.getConfigurationParameter(Qik2s9v1.QIK_CONFIG_SERIAL_TIMEOUT));

		if (args.length < 1) {
			return;
		}

		if (!args[0].equals("0") && !args[0].equals("1")) {
			System.out.println("Voie non valide");
			return;
		}

		try {
			for (int i = 1; i < args.length; i++) {
				int consigne = Integer.parseInt(args[i]);

				if (args[0].equals("0")) {
					qik2s9v1.setM0Speed(consigne);
				} else {
					qik2s9v1.setM1Speed(consigne);
				}

				System.out.println("Nouvelle consigne : " + consigne);
				Thread.sleep(1000);
			}
		} finally {
			System.out.println("Arrêt...");

			if (args[0].equals("0")) {
				qik2s9v1.setM0Speed(0);
			} else {
				qik2s9v1.setM1Speed(0);
			}
		}
	}
}
