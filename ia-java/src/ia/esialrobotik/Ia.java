package ia.esialrobotik;

import api.asserv.Asserv;
import api.chrono.Chrono;
import api.hardware.RaspberryPiGPIO;
import ia.common.AsservQueue;
import navigation.Navigation2014;
import navigation.Point;

import java.io.IOException;
import java.util.TimerTask;

public class Ia {
	public Asserv asserv;
	Navigation2014 nav;

	Tirette tirette;
    SelecteurCouleur selecteurCouleur;

	public TeamColor teamColor;
	public int ymult;
	public AsservQueue queue;

	public enum TeamColor {
		GREEN,
		YELLOW
	}

	public Ia() {
        try {
            // On initialise l'asservissement
            System.out.println("****** Init asserv");
            asserv = new Asserv("/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_101068a5cbdd92814f89f87e9a3fcdbac7ba-if01");
            System.out.println("COUCOUCOUCOU");

            System.out.println("****** Init GPIO");
            // TODO bien définir les GPIOs
            tirette = new Tirette(new RaspberryPiGPIO(42));
            selecteurCouleur  = new SelecteurCouleur(new RaspberryPiGPIO(42));

            System.out.println("******* ALL INIT DONE");

            queue = new AsservQueue(asserv);
            queue.start();
            return;
        } catch (Exception ex) {
            ex.printStackTrace();
	        System.exit(1);
        }
	}

	public Point getPosition() {
		return asserv.getCurrentPosition();
	}
	public static void main(String[] args) throws Exception {

		System.out.println("############################################################## IA #################################################");
		final Ia ia = new Ia();


		ia.start();
	}


	public void start() throws Exception {
		// On initialise le chrono
		Chrono chrono_stop = new Chrono(88 * 1000);

		System.out.println("Attente tirette présente");
		tirette.wait(false);

		System.out.println("Attente enlevage tirette");
		// On attend de virer la tirette
		tirette.wait(true);
		this.teamColor = selecteurCouleur.getTeamColor();
		this.ymult = this.teamColor == TeamColor.GREEN ? -1 : 1;

		System.out.println("IA initialisée. Couleur : " + (this.teamColor == TeamColor.GREEN ? "vert" : "jaune"));

		// On lance le callage bordure
		System.out.println("Calage bordure");

		this.asserv.calageBordure(this.teamColor != TeamColor.GREEN);

		// TODO Code pour se remettre dans la zone de départ
		asserv.gotoPosition(800, 500 * ymult, true);
		asserv.gotoPosition(800, 1000 * ymult, true);
		asserv.face(2000, 1000 * ymult, true);
		asserv.go(-390, true);

		System.out.println("Attente remise tirette");

		// On attend de remettre la tirette
		tirette.wait(false);

		// On attend de virer la tirette
		System.out.println("Attente enlevage tirette pour départ");
		tirette.wait(true);
		System.out.println("Gooo");

		chrono_stop.startChrono(new TimerTask() {
			@Override
			public void run() {

				System.out.println("Fin du match mais on tire un coup quand même");
				asserv.halt();
				asserv.resetHalt();
				asserv.go(-50, false);
				sleep(1000);
				asserv.halt();
				System.out.println("Fin");
				try {
					nettoyage();
				} catch (IOException e) {
					e.printStackTrace();
				}
				System.exit(0);
			}
		});

		//iaTest();
		//iaHomologation();
		iaPrincipale();
		nettoyage();
	}

	private void nettoyage() throws IOException {
		/* 
		 * actions à effectuer à la fin du match (genre ouvrir des pinces
		 * pour larguer des éléments de jeu
		 */
		
		// TODO à remplir si nécessaire
	}

	private void iaTest() {
		/*
		 * IA de test pour savoir si l'asserv marche
		 */
		asserv.gotoPosition(850, (2000 - 1770) * ymult, true);
		asserv.gotoPosition(1010, (2000 - 1770) * ymult, true);
		asserv.halt();
	}

	private void iaHomologation() throws IOException {
		/*
		 * IA d'homologation, simple : marquer un point tout en évitant l'adversaire
		 */
		
		// TODO initialiser l'évitement
		
		// TODO effectuer quelques actions
	}

	private void iaPrincipale() throws IOException {
		/*
		 * IA qui roxxe du poney et marque plein de points
		 */
		
		// TODO initialiser l'évitement
		
		// TODO effectuer quelques actions
	}
	
	public void sleep(int msec) {
		try {
			Thread.sleep(msec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

}
