package ia.esialrobotik;

import api.asserv.Asserv;
import api.chrono.Chrono;
import api.communication.Serial;
import api.controllers.PololuMaestro;
import ia.common.AsservQueue;
import ia.common.DetectionRPLidar;
import ia.common.Pince;
import navigation.Navigation2014;
import navigation.Point;
import org.mbed.RPC.DigitalIn;
import org.mbed.RPC.DigitalOut;
import org.mbed.RPC.MbedRPC;
import org.mbed.RPC.SerialMbedRPC;

import java.io.IOException;
import java.nio.file.FileSystems;
import java.util.TimerTask;

public class Ia {
	public Asserv asserv;
	Navigation2014 nav;

	MbedRPC rpc;
	Tirette tirette;
    SelecteurCouleur selecteurCouleur;
	PololuMaestro maestro;
	Etage etage;

	public DetectionRPLidar rplidar;
	Pince bras, tube, mainGauche, mainDroite, pinceGaucheGobelet, pinceGauchePied, pinceDroiteGobelet, pinceDroitePied;

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

            System.out.println("****** Init MBED IO");
            // On initialise la MBED pour les IO et l'étage
            rpc = new SerialMbedRPC(FileSystems.getDefault().getPath("/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_1010b17ca0c1d1b67081b01c87816f0123b1-if01").toRealPath().toString(), 115200);

            System.out.println("****** Init maestro");
            maestro = new PololuMaestro(new Serial("/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00046907-if00", 9600)); // if02
            bras = new Pince(maestro, 1, 1488, 2304);
            tube = new Pince(maestro, 0, 1856, 992);

            pinceGaucheGobelet = new Pince(maestro, 4, 2400, 1443);
	        pinceGauchePied = new Pince(maestro, 4, 2400, 1359);

            pinceDroiteGobelet = new Pince(maestro, 3, 704, 1645);
	        pinceDroitePied = new Pince(maestro, 3, 704, 1807);

            mainGauche = new Pince(maestro, 2, 1408, 608);
            mainDroite = new Pince(maestro, 5, 1616, 2400);


            System.out.println("COUCOUCOUCOU");

            System.out.println("****** Init GPIO");
            tirette = new Tirette(new DigitalIn(rpc, MbedRPC.p27), new DigitalOut(rpc, MbedRPC.p28));
            selecteurCouleur  = new SelecteurCouleur(new DigitalIn(rpc, MbedRPC.p29), new DigitalOut(rpc, MbedRPC.p30));

            System.out.println("****** Init moteur etage");
	        etage = new Etage(rpc, 10000);
	        etage.takeOrigin();

            System.out.println("****** Init RPLidar");

            rplidar = new DetectionRPLidar(this,
		            FileSystems.getDefault().getPath("/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0").toRealPath().toString(), 115200);

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


/*		ia.pied1(false);
		ia.sleep(2000);
		ia.pied2(false);
		ia.sleep(2000);
		//ia.pied3(false);
		//ia.sleep(2000);
		*/
		ia.start();
	}


	private void pied1(boolean move) throws IOException {
		System.out.println("Atrappage pied 1");
		this.etage.setPosition(0.3f, true);

		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		this.sleep(500);

		if (move)
			asserv.go(-10, true);
		this.etage.setPosition(0, true);

		this.pinceGauchePied.setPosition(1);
		this.pinceDroitePied.setPosition(1);
		this.sleep(500);

		System.out.println("Lève pied 1");
		this.etage.setPosition(0.9f, true);
	}

	private void pied2(boolean move) throws IOException {
		System.out.println("Attrape pied 2");
		this.etage.setPosition(0.7f, true);

		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		this.sleep(500);
		if (move)
			asserv.go(-10, true);

		System.out.println("Calage pied 2");
		this.etage.setPosition(0.2f, true);
		this.pinceGauchePied.setPosition(1);
		this.pinceDroitePied.setPosition(1);
		this.sleep(500);
		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		this.sleep(500);

		System.out.println("On prend le pied 2");
		this.etage.setPosition(0f, true);
		this.pinceGauchePied.setPosition(1);
		this.pinceDroitePied.setPosition(1);
		this.sleep(500);

		System.out.println("Lève pied 2");
		this.etage.setPosition(1.4f, false);
		this.sleep(1500);
	}


	private void pied3(boolean move) throws IOException {
		System.out.println("Attrape pied 3");
		this.etage.setPosition(0.7f, true);

		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		this.sleep(500);
		if (move) {
			asserv.go(-10, true);
		}

		System.out.println("Calage pied 3");
		this.etage.setPosition(0.2f, true);
		this.pinceGauchePied.setPosition(1);
		this.pinceDroitePied.setPosition(1);
		this.sleep(500);
		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		this.sleep(500);

		System.out.println("On prend pied 3");
		this.etage.setPosition(0f, true);
		this.pinceGauchePied.setPosition(1);
		this.pinceDroitePied.setPosition(1);
		this.sleep(500);

		System.out.println("Blocage");
		this.etage.setPosition(0.85f, false);
		this.sleep(1500);
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

		asserv.gotoPosition(800, 500 * ymult, true);
		asserv.gotoPosition(800, 1000 * ymult, true);
		asserv.face(2000, 1000 * ymult, true);
		asserv.go(-390, true);

		System.out.println("Depart prechargement balle");
		this.pinceGaucheGobelet.setPosition(1);
		this.pinceDroiteGobelet.setPosition(1);
		this.etage.setPosition(0.8f, true);

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
				try {
					pinceDroiteGobelet.setPosition(0);
					pinceGaucheGobelet.setPosition(0);
				} catch (IOException e) {
					e.printStackTrace();
				}

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

		bras.setPosition(0);
		mainGauche.setPosition(0);
		mainDroite.setPosition(0);

		//iaTest();
		//iaHomologation();
		iaEmpilage();
		nettoyage();
	}

	private void nettoyage() throws IOException {
		etage.setEnabled(false);
		etage.setOutput(0);
		bras.shutdown();
		mainGauche.shutdown();
		mainDroite.shutdown();
		pinceGauchePied.shutdown();
		pinceDroitePied.shutdown();

	}

	private void iaTest() {
		//rplidar.start();

		asserv.gotoPosition(850, (2000 - 1770) * ymult, true);
		asserv.gotoPosition(1010, (2000 - 1770) * ymult, true);
		asserv.halt();
	}

	private void iaHomologation() throws IOException {
		rplidar.start();

		asserv.gotoPosition(650, 1000 * ymult, true);
		asserv.gotoPosition(650, (2000 - 830) * ymult, true);

		// Avance vers verre 1
		asserv.gotoPosition(910 - 200, (2000 - 830) * ymult, true);
		asserv.gotoPosition(910 - 110, (2000 - 830) * ymult, true);

		// On serre la pince
		pinceGaucheGobelet.setPosition(1);
		pinceDroiteGobelet.setPosition(1);

		sleep(300);

		System.out.println("Avance vers verre 2 avec pince fermée");
		asserv.gotoPosition(2000, (2000 - 830) * ymult, true);
		System.out.println("Pause en 2200 pour pas le perdre");

		System.out.println("On se prépare à tourner");
		asserv.gotoPosition(2300, (2000 - 830) * ymult, true);

		System.out.println("On va le déposer");
		asserv.gotoPosition(2300, (2000 - 600) * ymult, true);
		asserv.gotoPosition(2800, (2000 - 600) * ymult, true);

		System.out.println("Ouverture pinces");
		pinceGaucheGobelet.setPosition(0);
		pinceDroiteGobelet.setPosition(0);

		sleep(300);

		asserv.go(-100, true);
	}

	private void iaEmpilage() throws IOException {
		System.out.println("Alignement avec pied premier pied");
		asserv.gotoPosition(870, 900 * ymult, true);

		System.out.println("Avance vers pied");
		asserv.gotoPosition(870, (2000 - 1250) * ymult, true);
		asserv.gotoPosition(870, (2000 - 1355) * ymult, true);

		pied1(true);

		asserv.gotoPosition(1200, (2000 - 1400) * ymult, true);
		asserv.gotoPosition(1300, (2000 - 1400) * ymult, true);

		pied2(true);

		asserv.gotoPosition(850, (2000 - 1400) * ymult, true);
		asserv.gotoPosition(850, (2000 - 1770) * ymult, true);
		asserv.gotoPosition(1040, (2000 - 1770) * ymult, true);

		pied3(true);

		asserv.go(-30, true);
		asserv.turn(-10 * ymult, true);
		asserv.go(70, true);
		asserv.turn(-10 * ymult, true);

		System.out.println("On lache tout");
		this.etage.setPosition(0, true);
		this.pinceGauchePied.setPosition(0);
		this.pinceDroitePied.setPosition(0);
		sleep(500);

		asserv.go(-200, true);

		if (teamColor == TeamColor.GREEN) {
			System.out.println("On est vert; on se retourne");
			asserv.gotoPosition(860, (2000 - 1760) * ymult, true);
			asserv.face(0, (2000 - 1760) * ymult, true);

			System.out.println("Clap 1");
			bras.setPosition(1);
			sleep(500);
			asserv.go(-170, true);
			bras.setPosition(0);
			sleep(500);

			System.out.println("Gobelet");
			pinceGaucheGobelet.setPosition(0);
			pinceDroiteGobelet.setPosition(0);
			sleep(500);

			asserv.gotoPosition(450, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(390, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(370, (2000 - 1760) * ymult, true);

			pinceGaucheGobelet.setPosition(1);
			pinceDroiteGobelet.setPosition(1);
			sleep(500);

			this.etage.setPosition(1.3f, true);

			asserv.gotoPosition(350, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(300, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(250, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(200, (2000 - 1760) * ymult, true);

			asserv.face(0, (2000 - 1760) * ymult, true);
			bras.setPosition(1);
			sleep(500);

			System.out.println("Clap 2");
			asserv.go(-200, true);
			bras.setPosition(0);
			sleep(500);

			asserv.gotoPosition(800, 500 * ymult, true);
			asserv.gotoPosition(800, 1000 * ymult, true);
			asserv.face(0, 1000 * ymult, true);
			asserv.go(500, true);

			System.out.println("On range le gobelet");
			this.etage.setPosition(0.1f, true);
			pinceGaucheGobelet.setPosition(0);
			pinceDroiteGobelet.setPosition(0);
			sleep(500);

			asserv.go(-200, true);

			System.out.println("Fin du match vert");
		} else {
			System.out.println("On est jaune; c'est moins chiant");
			asserv.gotoPosition(870, (2000 - 1760) * ymult, true);
			asserv.face(2000, (2000 - 1760) * ymult, true);

			System.out.println("Clap 1");
			bras.setPosition(1);
			sleep(500);
			//asserv.go(200, true);
			bras.setPosition(0);
			sleep(500);

			System.out.println("Gobelet");
			pinceGaucheGobelet.setPosition(0);
			pinceDroiteGobelet.setPosition(0);
			sleep(500);

			asserv.gotoPosition(450, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(390, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(370, (2000 - 1760) * ymult, true);
			pinceGaucheGobelet.setPosition(1);
			pinceDroiteGobelet.setPosition(1);

			this.etage.setPosition(1.3f, true);

			asserv.gotoPosition(350, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(300, (2000 - 1760) * ymult, true);
			asserv.gotoPosition(200, (2000 - 1760) * ymult, true);
			asserv.face(2000, (2000 - 1760) * ymult, true);
			bras.setPosition(1);
			sleep(500);

			System.out.println("Clap 2");
			//asserv.go(200, true);
			bras.setPosition(0);
			sleep(500);

			asserv.gotoPosition(800, 500 * ymult, true);
			asserv.gotoPosition(800, 1000 * ymult, true);
			asserv.face(0, 1000 * ymult, true);
			asserv.go(500, true);

			System.out.println("On range le gobelet");
			this.etage.setPosition(0.1f, true);
			pinceGaucheGobelet.setPosition(0);
			pinceDroiteGobelet.setPosition(0);
			sleep(500);

			asserv.go(-200, true);

			System.out.println("Fin du match vert");
		}
	}

	public void sleep(int msec) {
		try {
			Thread.sleep(msec);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

}
