package ia;

import gpio.Interrupteur;

import java.io.IOException;

import mbed.SerialMbed;

import servomoteur.Pince;
import asserv.Asserv;
import chrono.Chrono;

public class IA {

	protected static Chrono chrono;
	protected static Interrupteur tirette;
	protected static Interrupteur selecteurCouleur;
	protected static Asserv asservissement;
	protected static DeplacementTask ordresDeplacement;
	protected static Thread ordreDeplacementThread;
	protected static boolean sens;
	protected static boolean robotDetecte;
	protected static Pince pince;
	
	public static void main(String[] args) throws InterruptedException, IOException {
		
		// Redirection de la sortie vers un fichier de log
		/*File file  = new File("/root/sysout.log");
		PrintStream printStream;
	    try {
			printStream = new PrintStream(new FileOutputStream(file));
			System.setOut(printStream);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}*/
		
		// Initialisation du chrono
		System.out.println("Début Initialisation");
		chrono = new Chrono(89*1000);
		System.out.println("Chrono initialisé");
		
		//  Initialisation de la tirette
		tirette = new Interrupteur(50, 51); // GPIO50 = JP6-25 et GPIO51 = JP3-27
		System.out.println("Tirette initalisée");
		
		// Initialisation du sélecteur de  couleur
		selecteurCouleur = new Interrupteur(55, 35); // GPIO55 = JP3-23 et GPIO35 = JP3-24
		System.out.println("Sélecteur couleur initialisée");
		
		//  Initialisation de l'asserv
		asservissement = new Asserv();
		System.out.println("Asservissement initialisée");
		
		// Initialisation de la pince
		pince = new Pince();
		
		//Phase 1 : On attend que la tirette soit mise
		System.out.println("Veuillez mettre la tirette");
		while(tirette.checkInterrupteur());
		
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien mise
		
		// Phase 2 : On attend le retrait de la tirette pour le calage bordure
		System.out.println("Attente tirette pour calage bordure");
		while(!tirette.checkInterrupteur());
		
		//  On lit la couleur du match
		System.out.print("Couleur de départ : ");
		if(selecteurCouleur.checkInterrupteur()) {
			System.out.println("violet");
			sens = true;
		} else {
			System.out.println("rouge");
			sens = false;
		}
		ordresDeplacement = new DeplacementTask(asservissement, sens, pince);
		System.out.println("Tâche ordres de déplacement initialisée");
		
		ordreDeplacementThread = new Thread(ordresDeplacement);
		System.out.println("Thread déplacement initialisé");
		
		//on lance le calage bordure
		asservissement.calageBordure(sens);
		System.out.println("calage bordure lancé");
		
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien enlevee
		
		//Phase 3 : On attend la remise de la tirette avant le départ
		System.out.println("Veuillez mettre la tirette");
		while(tirette.checkInterrupteur());
		System.out.println("Tirette remise : PRET POUR MATCH !");
		//asservissement.emptyReceptionBuffer(); //On vide le buffer de la mbed
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien mise
		
		// Phase 4 : LE MATCH !
		while(!tirette.checkInterrupteur()); //on attend le retrait de la tirette
		System.out.println("Début Match : tirette tirée");
		chrono.startChrono(); //On lance le chrono
		ordreDeplacementThread.start(); //On lance le thread qui gere les deplacements
		System.out.println("Thread déplacement lancé");
		
		tirette.closeGpios();
		selecteurCouleur.closeGpios();
		System.out.println("GPIOs fermés");

		String[] commandArray = new String[3];
		commandArray[0] = "bash";
		commandArray[1] = "-c";
		commandArray[2] = "/root/SRF08/telemetre.sh";
		
		Process checkTelemetreProcess = null;
		
		while(true) {
			checkTelemetreProcess = Runtime.getRuntime().exec(commandArray);
			checkTelemetreProcess.waitFor();
			int codeRetour = checkTelemetreProcess.exitValue();
			
			if(robotDetecte != (codeRetour!=0)) {
				if(codeRetour!=0) {
					System.out.println("Attention robot !");
				} else {
					System.out.println("On repart");
				}
			}
			if(codeRetour!=0) {
				System.out.println("code " + codeRetour);
			}
			
			robotDetecte = (codeRetour!=0);
			
			switch(codeRetour) {
			// TODO Gérer marche arriere face à l'adversaire
			case 0:
				
				ordresDeplacement.resumeDeplacement();
				break;
				
			case 1:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 2:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 3:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 4:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 5:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 6:
				ordresDeplacement.stopDeplacements();
				break;
				
			case 7:
				ordresDeplacement.stopDeplacements();
				break;
				
			}
		}
		
		
		
		
		/*
		try {
			mbed.send("g518#128\n");
			mbed.send("g518#673\n");
			mbed.send("g278#673\n");
		} catch (Exception e) {
			e.printStackTrace();
		}
		*/
		
		
		
	}
	
	public static void finDuMatch() throws InterruptedException {
		System.out.println("Fin du match !");
		asservissement.halt();
		ordresDeplacement.stopDeplacements();
		System.exit(0);
	}
	
}
