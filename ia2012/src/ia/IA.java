package ia;

import gpio.Interrupteur;

import java.io.IOException;

import servomoteur.Pince;
import asserv.Asserv;
import chrono.Chrono;

public class IA {

	protected static Chrono chrono;
	protected static Interrupteur tirette;
	protected static Interrupteur selecteurCouleur;
	protected static Asserv asservissement;
	protected static DeplacementTask ordresDeplacement;
	protected static boolean sens;
	protected static int robotDetecte;
	protected static Pince pince;
	protected static boolean esquive = false;
	protected static int seuil_detection = 58;
	protected static int direction_detection = 1;

	protected static int telemetre_arriere = 0;
	protected static int telemetre_avantD = 0;
	protected static int telemetre_avantG = 0;
	protected static int telemetreNombreMoyenne = 4;
	protected static String[] commandArray;
	protected static int tab_avD[] = new int[telemetreNombreMoyenne];
	protected static int tab_avG[] = new int[telemetreNombreMoyenne];
	protected static int tab_ar[] = new int[telemetreNombreMoyenne];

	protected static int idx_avG = 0, idx_avD = 0, idx_ar = 0;

	public static void main(String[] args) throws InterruptedException,
			IOException {

		// Redirection de la sortie vers un fichier de log
		/*
		 * File file = new File("/root/sysout.log"); PrintStream printStream;
		 * try { printStream = new PrintStream(new FileOutputStream(file));
		 * System.setOut(printStream); } catch (FileNotFoundException e) {
		 * e.printStackTrace(); }
		 */

		// Initialisation du chrono
		System.out.println("Début Initialisation");
		chrono = new Chrono(88 * 1000);
		System.out.println("Chrono initialise");

		// Initialisation de la tirette
		tirette = new Interrupteur(50, 51); // GPIO50 = JP6-25 et GPIO51 =
											// JP3-27
		System.out.println("Tirette initalise");

		// Initialisation du sélecteur de couleur
		selecteurCouleur = new Interrupteur(55, 35); // GPIO55 = JP3-23 et
														// GPIO35 = JP3-24
		System.out.println("Sélecteur couleur initialise");

		// Initialisation de l'asserv
		asservissement = new Asserv();
		System.out.println("Asservissement initialise");

		// Initialisation de la pince
		pince = new Pince();

		// Phase 1 : On attend que la tirette soit mise
		System.out.println("Veuillez mettre la tirette");
		while (tirette.checkInterrupteur())
			;

		Thread.sleep(1000); // On attend pour etre sur que la tirette soit bien
							// mise

		// Phase 2 : On attend le retrait de la tirette pour le calage bordure
		System.out.println("Attente tirette pour calage bordure");
		while (!tirette.checkInterrupteur())
			;

		// On lit la couleur du match
		System.out.print("Couleur de depart : ");
		if (selecteurCouleur.checkInterrupteur()) {
			System.out.println("violet");
			sens = true;
		} else {
			System.out.println("rouge");
			sens = false;
		}
		ordresDeplacement = new DeplacementTask(asservissement, sens, pince);
		System.out.println("Tâche ordres de déplacement initialise");

		// on lance le calage bordure
		asservissement.calageBordure(sens);
		System.out.println("calage bordure lance");

		Thread.sleep(1000); // On attend pour etre sur que la tirette soit bien
							// enlevee

		// Phase 3 : On attend la remise de la tirette avant le départ
		System.out.println("Veuillez mettre la tirette");
		while (tirette.checkInterrupteur())
			;
		System.out.println("Tirette remise : PRET POUR MATCH !");
		// asservissement.emptyReceptionBuffer(); //On vide le buffer de la mbed
		Thread.sleep(1000); // On attend pour etre sur que la tirette soit bien
							// mise

		// Phase 4 : LE MATCH !
		while (!tirette.checkInterrupteur())
			; // on attend le retrait de la tirette
		System.out.println("Debut Match : tirette tire");
		chrono.startChrono(); // On lance le chrono

		/* ordreDeplacementThread = new Thread(ordresDeplacement); */
		ordresDeplacement.start();
		System.out.println("Thread deplacement initialise");
		// ordreDeplacementThread.start(); //On lance le thread qui gere les
		// deplacements
		System.out.println("Thread deplacement lance");

		tirette.closeGpios();
		selecteurCouleur.closeGpios();
		System.out.println("GPIOs ferme");

		commandArray = new String[3];
		commandArray[0] = "bash";
		commandArray[1] = "-c";
		commandArray[2] = "/root/SRF08/telemetre.sh " + direction_detection;

		while (true) {
			/*
			if(ordresDeplacement.isDetectionActive()) {
				try {
					detection();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			*/
		}

	}

	public static void detection() throws Exception {
		if (esquive)
			direction_detection = 3;
		else if (ordresDeplacement.isMarcheAvant())
			direction_detection = 1;
		else
			direction_detection = 2;

		commandArray[2] = "/root/SRF08/telemetre_retour_unitaire.sh "
				+ direction_detection;

		Process checkTelemetreProcess = Runtime.getRuntime().exec(commandArray);
		checkTelemetreProcess.waitFor();

		byte buff[] = new byte[11];
		checkTelemetreProcess.getInputStream().read(buff);

		String[] tab_res = (new String(buff)).split("#");

		tab_ar[idx_ar++ % telemetreNombreMoyenne] = Integer
				.parseInt(tab_res[0]);
		tab_avD[idx_avD++ % telemetreNombreMoyenne] = Integer
				.parseInt(tab_res[1]);
		tab_avG[idx_avG++ % telemetreNombreMoyenne] = Integer
				.parseInt(tab_res[2]);

		int cur_val_avG = 0;
		int cur_val_avD = 0;
		int cur_val_ar = 0;

		for (int i = 0; i < telemetreNombreMoyenne; i++) {
			cur_val_avG += (tab_avG[i] > 0 ? tab_avG[i] : 65);
			cur_val_avD += (tab_avD[i] > 0 ? tab_avD[i] : 65);
			cur_val_ar += (tab_ar[i] > 0 ? tab_ar[i] : 65);

		}

		cur_val_avG = cur_val_avG / telemetreNombreMoyenne;
		cur_val_avD = cur_val_avD / telemetreNombreMoyenne;
		cur_val_ar = cur_val_ar / telemetreNombreMoyenne;

		// System.out.println("cur_val_avG=" + cur_val_avG + " cur_val_avD=" +
		// cur_val_avD + " cur_val_ar=" + cur_val_ar);

		if ((cur_val_avG < seuil_detection && cur_val_avG != 0 || cur_val_avD < seuil_detection
				&& cur_val_avD != 0)
				&& cur_val_ar < seuil_detection && cur_val_ar != 0) {
			robotDetecte = 3;
			System.out.println("Robot detecte partout! (DP time)");
		} else if (ordresDeplacement.isMarcheAvant()
				&& (cur_val_avG < seuil_detection && cur_val_avG != 0 || cur_val_avD < seuil_detection
						&& cur_val_avD != 0)) {
			robotDetecte = 1;
			System.out.println("Robot detecte devant");
		} else if (!ordresDeplacement.isMarcheAvant()
				&& cur_val_ar < seuil_detection && cur_val_ar != 0) {
			robotDetecte = 2;
			System.out.println("Robot detecte derriere");
		} else {
			robotDetecte = 0;
		}

		switch (robotDetecte) {

		case 0: // On voit rien

			if (esquive) // On kill les commandes d'evitements
				ordresDeplacement.stopDeplacements();

			esquive = false;
			ordresDeplacement.resumeDeplacement();
			break;

		case 1: // Devant
			if (ordresDeplacement.isMarcheAvant()) {
				if (!esquive) {
					ordresDeplacement.stopDeplacements();
					esquive = true;
				} else {
					asservissement.eviteAdversaireDevant();
					waitForFinish();

				}
			}

			break;

		case 2: // Derriere
			if (!ordresDeplacement.isMarcheAvant()) {
				if (!esquive) {
					ordresDeplacement.stopDeplacements();
					esquive = true;
				} else {
					asservissement.eviteAdversaireDerriere();
					waitForFinish();
				}
			}

			break;

		case 3: // DP
			ordresDeplacement.stopDeplacements();
			break;

		}

	}

	public static void waitForFinish() {
		/*
		 * boolean finished = false; while(!finished) {
		 * if(asservissement.lastCommandFinished()) { finished = true; } }
		 */
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();

		}
	}

	public static void finDuMatch() throws InterruptedException {
		System.out.println("Fin du match !");
		pince.openPinceAvForEject();
		Thread.sleep(300);
		pince.openPinceArForEject();
		Thread.sleep(300);
		pince.ejecter();
		asservissement.halt();
		ordresDeplacement.stopDeplacements();
		System.exit(0);
	}

}
