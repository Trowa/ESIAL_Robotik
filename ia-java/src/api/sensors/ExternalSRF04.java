package api.sensors;

import java.io.IOException;
import java.util.Scanner;

/**
 * Une classe pour gérer des télémetres SRF04
 * Un programme externe est nécessaire pour faire fonctionner les capteurs,
 * la classe va se contenter de communiquer avec le programme externe.
 * @author jb
 *
 */
public class ExternalSRF04 {
	
	private int nbCapteurs;
	private Process externalProcess;
	private Scanner externalProcessScanner;
	
	/**
	 * Constructeur. On définit les GPIO utilisés par les capteurs.
	 * Les deux tableaux doivent avoir la même taille
	 * @param GPIOin
	 * @param GPIOout
	 * @throws IOException Si on arrive pas à exécuter le programme externe
	 */
	public ExternalSRF04(String pathToExternalProgram, int[] GPIOin, int[] GPIOout) throws IOException
	{
		nbCapteurs = GPIOin.length;
		String command = pathToExternalProgram;
		
		if(GPIOin.length != GPIOout.length) {
			throw new ArrayIndexOutOfBoundsException("Les dimensions des tableaux sont différentes !");
		}
		
		// On va ajouter les numéros de GPIO à la ligne de commande
		for(int capteurIndex = 0; capteurIndex < nbCapteurs; capteurIndex++) {
			command = command + " " + GPIOin[capteurIndex] + " " + GPIOout[capteurIndex];
		}
		
		System.out.println("SRF04 commande : \"" + command + "\"");
		
		// On lance le programme externe
		externalProcess = Runtime.getRuntime().exec(command);
		
		// On instancie le scanner pour parser la sortie
		externalProcessScanner = new Scanner(externalProcess.getInputStream());
		
		// Quand on termine le programme java, on s'assure que le programme externe est arrêté
		Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
			
			@Override
			public void run() {
				stop();
			}
		}));
		
	}
	
	/**
	 * Une fonction pour récupérer les données des capteurs
	 * Devrait être appelée en boucle dans un thread séparé
	 * @return
	 */
	private int[] getMesures() {
		int[] mesures = new int[nbCapteurs];
		
		// On récupère les données des capteurs et on les stocke
		for(int capteurIndex = 0; capteurIndex < nbCapteurs; capteurIndex++) {
			mesures[capteurIndex] = externalProcessScanner.nextInt();
		}
		
		return mesures;
	}

	
	public void stop() {
		boolean terminated = false;
		
		externalProcess.destroyForcibly();
		
		while(!terminated) {
			try {
				externalProcess.waitFor();
				terminated = true;
			} catch (InterruptedException e) {
			}
		}
	}
	
	/**
	 * Un main pour tester
	 * @param args
	 * @throws IOException
	 */
	public static void main(String args[]) throws IOException {
		int[] GPIOin = {17, 10, 5, 19};
		int[] GPIOout = {27, 9, 6, 26};
		
		String home = System.getenv().get("HOME");
		
		ExternalSRF04 srf04 = new ExternalSRF04(home + "/srf04/srf04", GPIOin, GPIOout);
		
		while(true) {
			int[] mesures = srf04.getMesures();
			System.out.print("Mesures: ");
			for(int mesure : mesures) {
				System.out.print(mesure + " ");
			}
			System.out.print("\n");
		}
	}
}
