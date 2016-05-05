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

	Tirette tirette;
    SelecteurCouleur selecteurCouleur;

	public TeamColor teamColor;
	public int ymult;
	public AsservQueue queue;
	public DetectionExternalSRF04Thread detection;

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

            queue = new AsservQueue(asserv);
            queue.start();
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
		Chrono chrono_stop = new Chrono((int) (89 * 1000));

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

		//iaTest();
		iaHomologation();
		//iaPrincipale();
		//nettoyage();
	}
	
	private void goPositionDepart() throws IOException {
		try {
			asserv.gotoPosition(250, 915 * ymult, true);
			asserv.face(1000, 915 * ymult, true);
			asserv.go(-100, true);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	private void nettoyage() throws IOException {
		/* 
		 * actions à effectuer à la fin du match (genre ouvrir des pinces
		 * pour larguer des éléments de jeu
		 */
		
		// TODO à remplir si nécessaire
		this.detection.stop();
		this.queue.stop();
	}

	private void iaTest() {
		/*
		 * IA de test pour savoir si l'asserv marche
		 */
		try {
			asserv.gotoPosition(850, (2000 - 1770) * ymult, true);
			asserv.gotoPosition(1010, (2000 - 1770) * ymult, true);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		asserv.halt();
	}

	private void iaHomologation() throws IOException {
		/*
		 * IA d'homologation, simple : marquer un point tout en évitant l'adversaire
		 */
		this.createAndLaunchDetection();

		// On sort de a zone de départ et on pousse le premier chateau dans la zone de construction
		this.queue.addAction(new Goto(450, this.ymult*920));
		this.queue.addAction(new Goto(900, this.ymult*920));
		// On quitte la zone de construction
		this.queue.addAction(new Go(-400));
		// On va en face des cabines et on s'alignes
		this.queue.addAction(new Goto(450, this.ymult*280));
		this.queue.addAction(new Face(450, this.ymult*0));
		// On force contre la bordure avec timeout
		this.queue.addAction(new Go(200));
		this.queue.addAction(new WaitAndCancel(1000));
		// Suite au timeout, on recule
		this.queue.addAction(new Go(-200));
		// On se positionne entre les deux coquillages les plus haut et on fait une boucle pour en chopper un max
		this.queue.addAction(new Goto(425, 1250));
		this.queue.addAction(new Goto(1050, 1250));
		this.queue.addAction(new Goto(1050, 1735));
		this.queue.addAction(new Goto(450, 1735));
		this.queue.addAction(new Goto(200, 1650));
		this.queue.addAction(new Goto(200, 900));
		this.queue.addAction(new Go(-550));
		this.queue.addAction(new Goto(425, 1250));
		this.queue.addAction(new Goto(200, 900));
		this.queue.addAction(new Go(-100));
	}

	private void iaPrincipale() throws IOException {
		/*
		 * IA qui roxxe du poney et marque plein de points
		 */
		this.createAndLaunchDetection();
		// TODO effectuer quelques actions
	}
	
	public void sleep(int msec) {
		try {
			Thread.sleep(msec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public void createAndLaunchDetection() {
		System.out.println("******* Init detection");
		SRF04JNI srf04AvantDroit = new SRF04JNI(17, 27);
		SRF04JNI srf04AvantMilieu = new SRF04JNI(10, 9);
		SRF04JNI srf04AvantGauche = new SRF04JNI(5, 6);
		SRF04JNI srf04Arriere = new SRF04JNI(19, 26);
		this.detection = new DetectionExternalSRF04Thread(srf04AvantGauche, srf04AvantMilieu, srf04AvantDroit, srf04Arriere, 400, this);
		this.detection.start();
	}

	public void detectionAdversaire(Point p) {
		this.asserv.halt();
		this.queue.abort();
	}

}
