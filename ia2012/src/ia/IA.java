package ia;

import asserv.Asserv;
import gpio.Interrupteur;
import chrono.Chrono;

public class IA {

	protected static Chrono chrono;
	protected static Interrupteur tirette;
	protected static Interrupteur selecteurCouleur;
	protected static Asserv asservissement;
	protected static DeplacementTask ordresDeplacement;
	protected static Thread ordreDeplacementThread;
	protected static boolean sens;
	
	public static void main(String[] args) throws InterruptedException {
		
		// Redirection de la sortie vers un fichier de log
		/*File file  = new File("/root/sysout.log");
		PrintStream printStream;
	    try {
			printStream = new PrintStream(new FileOutputStream(file));
			System.setOut(printStream);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}*/
		
		// Initialisation
		System.out.println("Debut Initialisation");
		//chrono = new Chrono(89*1000);
		chrono = new Chrono(10 * 1000);
		System.out.println("Chrono initialise");
		tirette = new Interrupteur(50, 51); // GPIO50 = JP6-25 et GPIO51 = JP3-27
		System.out.println("Tirette initalisee");
		selecteurCouleur = new Interrupteur(55, 35); // GPIO55 = JP3-23 et GPIO35 = JP3-24
		System.out.println("Selecteur couleur initialisee");
		
		asservissement = new Asserv();
		System.out.println("Asservissement initialisee");
		
		System.out.print("Couleur de départ : ");
		if(selecteurCouleur.checkInterrupteur()) {
			System.out.println("violet");
			sens = true;
		} else {
			System.out.println("rouge");
			sens = false;
		}
		
		ordresDeplacement = new DeplacementTask(asservissement, sens);
		System.out.println("Tache ordres de deplacement initialisee");
		
		ordreDeplacementThread = new Thread(ordresDeplacement);
		System.out.println("Thread deplacement initialisee");
		
		//Phase 1 : On attend que la tirette soit mise
		System.out.println("Veuillez mettre la tirette");
		while(tirette.checkInterrupteur());
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien mise
		
		// Phase 2 : On attend le retrait de la tirette pour le calage bordure
		System.out.println("Attente tirette pour calage bordure");
		while(!tirette.checkInterrupteur());
		//on lance le calage bordure
		asservissement.calageBordure(sens);
		System.out.println("calage bordure lance");
		
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien enlevee
		
		//Phase 3 : On attend la remise de la tirette avant le départ
		System.out.println("Veuillez mettre la tirette");
		while(tirette.checkInterrupteur());
		System.out.println("Tirette remise : PRET POUR MATCH !");
		Thread.sleep(1000); //On attend pour etre sur que la tirette soit bien mise
		
		// Phase 4 : LE MATCH !
		while(!tirette.checkInterrupteur()); //on attend le retrait de la tirette
		System.out.println("Debut Match : tirette tiree");
		chrono.startChrono(); //On lance le chrono
		ordreDeplacementThread.start(); //On lance le thread qui gere les deplacements
		System.out.println("Thread deplacement lancee");
		
		tirette.closeGpios();
		selecteurCouleur.closeGpios();
		System.out.println("GPIOs fermes");
		
		
		while(true) {
			
		}
	}
	
	public static void finDuMatch() throws InterruptedException {
		System.out.println("Fin du match !");
		asservissement.halt();
		try {
			ordreDeplacementThread.wait();
		} catch(Exception e) {
			e.printStackTrace();
		}
		System.exit(0);
	}
	
}
