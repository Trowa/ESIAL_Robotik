package ia.esialrobotik;

import api.asserv.Asserv;
import api.asserv.AsservMbed;
import api.asserv.actions.*;
import api.chrono.Chrono;
import api.hardware.RaspberryPiGPIO;
import api.sensors.DetectionExternalSRF04Thread;
import api.sensors.SRF04JNI;
import ia.common.AsservQueue;
import navigation.Navigation.TeamColor;
import navigation.Point;

import java.io.IOException;
import java.util.TimerTask;

public class Ia {
	public Asserv asserv;

	public Tirette tirette;
    public SelecteurCouleur selecteurCouleur;
	public AsservQueue asservQueue;

	public TeamColor teamColor;
	public int ymult;
	public DetectionExternalSRF04Thread detection;

	public boolean debugMode = false;
	public String iaMode = "";

	public Ia() {
        try {
            // On initialise l'asservissement
            System.out.println("****** Init asserv");
            // TODO vérifier le fichier de périphérique
            asserv = new AsservMbed("/dev/serial/by-id/usb-mbed_Microcontroller_101000000000000000000002F7F2854A-if01");
            System.out.println("COUCOUCOUCOU");

            System.out.println("****** Init GPIO");
            // TODO bien définir les GPIOs
            tirette = new Tirette(new RaspberryPiGPIO(24, true));
            selecteurCouleur  = new SelecteurCouleur(new RaspberryPiGPIO(23, true));

            System.out.println("******* ALL INIT DONE");

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
		if (args.length > 0) {
			for (String arg : args) {
				switch (arg) {
					case "debug" :
						System.out.println("Debug mode POWAAAAAAAAAAAAAAAA !!! Donc l'init on s'en bas les couilles");
						ia.setDebugMode(true);
						break;

					case "color-green" :
						System.out.println("On joue vert, comme Hulk");
						ia.teamColor = TeamColor.GREEN;
						ia.ymult = 1;
						break;

					case "color-violet" :
						System.out.println("On joue violet, comme... je sais pas qui");
						ia.teamColor = TeamColor.VIOLET;
						ia.ymult = -1;
						break;

					case "iaTest" :
						ia.iaMode = "iaTest";
						break;

					case "iaTestQueue" :
						ia.iaMode = "iaTestQueue";
						break;
				}
			}
		}
		ia.start();
	}

	public void start() throws Exception {
		// On initialise le chrono
		Chrono chrono_stop = new Chrono((int) (89 * 1000));

		// On initialise l'IA (calage bordure, tirette, couleur, etc.)
		if (!this.debugMode) {
			this.initialisationIA();
		}

		System.out.println("Gooo");

		chrono_stop.startChrono(new TimerTask() {
			@Override
			public void run() {

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

		switch (this.iaMode) {
			case "iaTest" :
				iaTest();
				break;
			case "iaTestQueue" :
				iaTestAsservQueue();
				break;
			case "":
			default:
				//iaHomologation();
				iaHomologationAsservQueue();
				break;
		}
		//iaPrincipale();
		//nettoyage();
	}

	private void initialisationIA() throws IOException {
		System.out.println("Attente tirette présente");
		tirette.wait(false);

		System.out.println("Attente enlevage tirette");
		// On attend de virer la tirette
		tirette.wait(true);
		this.teamColor = selecteurCouleur.getTeamColor();
		this.ymult = this.teamColor == TeamColor.VIOLET ? -1 : 1;

		System.out.println("IA initialisée. Couleur : " + this.teamColor);

		// On lance le callage bordure
		System.out.println("Calage bordure");

		this.asserv.calageBordure(this.teamColor != TeamColor.VIOLET);

		// Code pour se remettre dans la zone de départ
		this.goPositionDepart();

		System.out.println("Attente remise tirette");

		// On attend de remettre la tirette
		tirette.wait(false);

		// On attend de virer la tirette
		System.out.println("Attente enlevage tirette pour départ");
		tirette.wait(true);
	}
	
	private void goPositionDepart() throws IOException {
		asserv.gotoPosition(250, 915 * ymult, true);
		asserv.face(1000, 915 * ymult, true);
		asserv.go(-100, true);
	}

	private void nettoyage() throws IOException {
		/* 
		 * actions à effectuer à la fin du match (genre ouvrir des pinces
		 * pour larguer des éléments de jeu
		 */
		this.detection.stop();
		this.asservQueue.stop();
	}

	private void iaTest() {
		/*
		 * IA de test pour savoir si l'asserv marche
		 */
		//this.createAndLaunchDetection();
		this.asserv.go(200, false);
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("J'ai timeout chef");
		this.asserv.halt();
		this.asserv.resetHalt();
	}

	private void iaTestAsservQueue() throws IOException {
		/*
		 * IA d'homologation, simple : marquer un point tout en évitant l'adversaire
		 */
		this.createAndLaunchDetection(100);
		this.asservQueue = new AsservQueue(this.asserv, this);
		this.asservQueue.start();

		// On force contre la bordure avec timeout
		Action go = new Go(200);
		go.setBlockingCommand(false);
		this.asservQueue.addAction(go);
		this.asservQueue.addAction(new WaitAndCancel(1000));
		this.asservQueue.addAction(new Goto(420,420));
		this.asservQueue.addAction(new Goto(500,500));
	}

	private void iaHomologationAsservQueue() throws IOException {
		/*
		 * IA d'homologation, simple : marquer un point tout en évitant l'adversaire
		 */
		this.createAndLaunchDetection(300);
		this.asservQueue = new AsservQueue(this.asserv, this);
		this.asservQueue.start();

		// On sort de a zone de départ et on pousse le premier chateau dans la zone de construction
		this.asservQueue.addAction(new Goto(450, this.ymult*920));
		this.asservQueue.addAction(new Goto(1200, this.ymult*1000));
		// On quitte la zone de construction
		this.asservQueue.addAction(new Go(-700));
		// On va en face des cabines et on s'alignes
		this.asservQueue.addAction(new Goto(450, this.ymult*280));
		this.asservQueue.addAction(new Face(450, this.ymult*0));
		// On force contre la bordure avec timeout
		Action go = new Go(200);
		go.setBlockingCommand(false);
		this.asservQueue.addAction(go);
		this.asservQueue.addAction(new WaitAndCancel(1000));
		// Suite au timeout, on recule
		this.asservQueue.addAction(new Go(-200));
		// On se positionne entre les deux coquillages les plus haut et on fait une boucle pour en chopper un max
		this.asservQueue.addAction(new Goto(425, 1250));
		this.asservQueue.addAction(new Goto(1050, 1250));
		this.asservQueue.addAction(new Goto(1050, 1735));
		this.asservQueue.addAction(new Goto(450, 1735));
		this.asservQueue.addAction(new Goto(200, 1650));
		this.asservQueue.addAction(new Goto(200, 900));
		this.asservQueue.addAction(new Go(-550));
		this.asservQueue.addAction(new Goto(425, 1250));
		this.asservQueue.addAction(new Goto(200, 900));
		this.asservQueue.addAction(new Go(-100));
	}

	private void iaHomologation() throws IOException {
		/*
		 * IA d'homologation, simple : marquer un point tout en évitant l'adversaire
		 */
		this.createAndLaunchDetection(300);

		// On sort de a zone de départ et on pousse le premier chateau dans la zone de construction
		this.asserv.gotoPosition(450, this.ymult*920, true);
		this.asserv.gotoPosition(900, this.ymult*970, true);
		// On quitte la zone de construction
		this.asserv.go(-400, true);
		// On va en face des cabines et on s'alignes
		this.asserv.gotoPosition(450, this.ymult*280, true);
		this.asserv.face(450, this.ymult*0, true);
		// On force contre la bordure avec timeout
		this.asserv.go(200, false);
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		this.asserv.halt();
		this.asserv.resetHalt();
		// Suite au timeout, on recule
		this.asserv.go(-200, true);
		// On se positionne entre les deux coquillages les plus haut et on fait une boucle pour en chopper un max
		this.asserv.gotoPosition(425, 1250, true);
		this.asserv.gotoPosition(1050, 1250, true);
		this.asserv.gotoPosition(1050, 1735, true);
		this.asserv.gotoPosition(450, 1735, true);
		this.asserv.gotoPosition(200, 1650, true);
		this.asserv.gotoPosition(200, 900, true);
		this.asserv.go(-550, true);
		this.asserv.gotoPosition(425, 1250, true);
		this.asserv.gotoPosition(200, 900, true);
		this.asserv.go(-100, true);
	}

	private void iaPrincipale() throws IOException {
		/*
		 * IA qui roxxe du poney et marque plein de points
		 */
		this.createAndLaunchDetection(300);
		// TODO effectuer quelques actions
	}
	
	public void sleep(int msec) {
		try {
			Thread.sleep(msec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public void createAndLaunchDetection(int seuil) {
		System.out.println("******* Init detection");
		SRF04JNI srf04AvantDroit = new SRF04JNI(17, 27);
		SRF04JNI srf04AvantMilieu = new SRF04JNI(10, 9);
		SRF04JNI srf04AvantGauche = new SRF04JNI(5, 6);
		SRF04JNI srf04Arriere = new SRF04JNI(19, 26);
		this.detection = new DetectionExternalSRF04Thread(srf04AvantGauche, srf04AvantMilieu, srf04AvantDroit, srf04Arriere, seuil, this);
		this.detection.start();
	}

	public void detectionAdversaire(Point p) {
		this.asserv.halt();
		this.asservQueue.abort();
	}

	public void pasdadversaire() {
		if (this.asservQueue.isAborted()) {
			this.asserv.resetHalt();
			this.asservQueue.reset();
		}
	}

	public boolean isDebugMode() {
		return debugMode;
	}

	public void setDebugMode(boolean debugMode) {
		this.debugMode = debugMode;
	}
}
