package ia;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

import gpio.Interrupteur;
import chrono.Chrono;

public class IA {

	protected static Chrono chrono;
	protected static Interrupteur tirette;
	protected static Interrupteur selecteurCouleur;
	
	public static void main(String[] args) {
		
		// Redirection de la sortie vers un fichier de log
		File file  = new File("/root/sysout.log");
		PrintStream printStream;
	    try {
			printStream = new PrintStream(new FileOutputStream(file));
			System.setOut(printStream);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    
		
		
		// Initialisation
		System.out.println("Debut Initialisation");
		chrono = new Chrono();
		System.out.println("Chrono initialise");
		tirette = new Interrupteur(50, 51); // GPIO50 = JP6-25 et GPIO51 = JP3-27
		System.out.println("Tirette initalisee");
		selecteurCouleur = new Interrupteur(56, 59); // GPIO56 = JP3-26 et GPIO59 = JP3-29
		System.out.println("Selecteur couleur initialisee");
		
		
		chrono.startChrono();
		
		
		// Phase 1 : On attend le retrait de la tirette
		System.out.println("Attente tirette");
		while(!tirette.checkInterrupteur());
		
		// Phase 2
		System.out.println("Debut Match : tirette tiree");
		
		// Dernière phase : fin du match
		finDuMatch();
	}
	
	public static void finDuMatch() {
		tirette.closeGpios();
		System.out.println("Fin Tirette");
	}
	
}
