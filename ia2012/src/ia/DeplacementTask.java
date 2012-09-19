package ia;

import servomoteur.Pince;
import asserv.Asserv;

public class DeplacementTask extends Thread /*implements Runnable*/ {
	
	private Asserv asservissement;
	private boolean sens;
	private boolean stopped;
	private Pince pince;
	private boolean marche_avant;
	private boolean detectionActive;
	
	public boolean isDetectionActive() {
		return detectionActive;
	}


	public DeplacementTask(Asserv asservissement, boolean sens, Pince pince) {
		this.asservissement = asservissement;
		this.sens = sens;
		this.pince = pince;
		stopped = false;
		this.marche_avant = true;
		detectionActive = true;
	}
	
	
	public boolean isMarcheAvant()
	{
		return this.marche_avant;
	}
	

	@Override
	public void run() {
		
		System.out.println("C'est parti !");
		
		//homologation();
		matchBien();
		//test();
		//testPince();
		
		try {
			Thread.sleep(1000*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	private void test()
	{
		int mult = sens ? -1 : 1;
		
		this.marche_avant = true;
		System.out.println("commande 1");
		asservissement.gotoPosition(2000,0);
		waitForFinish();
		
	}
	
	
	private void homologation() {
		
		int mult = sens ? -1 : 1;
		
		this.marche_avant = true;
		System.out.println("commande 1");
		asservissement.gotoPosition(538, mult*128); //Sortie zone de départ
		waitForFinish();
		System.out.println("commande 2");
		asservissement.gotoPosition(538, mult*673); //On descend en suivant la ligne
		waitForFinish();
		System.out.println("commande 3");
		
		asservissement.gotoPosition(278, mult*673); //On pousse le lingot
		waitForFinish();
		
		
		
		System.out.println("commande 4");
		asservissement.go(-260); //On recule sur la ligne noire
		this.marche_avant = false;
		waitForFinish();
		
		System.out.println("commande 5");
		asservissement.gotoPosition( 538, mult*1646 ); //On se place devant la bouteille
		this.marche_avant = true;
		waitForFinish();
		
		System.out.println("commande 6");
		asservissement.go( 110 ); //On tape la bouteille
		this.marche_avant = true;
		waitForFinish();
		
		System.out.println("commande 6");
		asservissement.go(-110); //On recule sur la ligne noire
		this.marche_avant = false;
		waitForFinish();
	}
	
	private void matchBien() {
		
		int mult = sens ? -1 : 1;
		
		asservissement.gotoPosition(878, mult*100); //On esquive le CD
		waitForFinish();
		asservissement.gotoPosition(1378, mult*200); //Avance vers le milieu haut de la table
		waitForFinish();
		asservissement.gotoPosition(1378, mult*540); //Prêt à râcler le haut du totem
		waitForFinish();
		asservissement.gotoPosition(578, mult*540); //On râcle
		waitForFinish();
		detectionActive = false;
		asservissement.gotoPosition(250, mult*750); //On pousse les CDs et le lingot
		waitForFinish();
		detectionActive = true;
		asservissement.go(-200); //On recule pour avoir la place de tourner
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(478, mult*1600); //On va en bas de la table
		marche_avant = true;
		waitForFinish();
		asservissement.face(478, mult*1800); //On vise la bouteille
		waitForFinish();
		detectionActive = false;
		asservissement.go(150); //On pousse la bouteille
		try {
			Thread.sleep(2000);
		} catch(Exception e) {
			e.printStackTrace();
		}
		asservissement.halt(); //on stoppe l'asserv
		pince.openPinceAv(); //Un coup de pince pour être sûr
		try {
			Thread.sleep(300);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.closePinceAv();
		asservissement.resetHalt(); //on réasservit
		detectionActive = true;
		asservissement.go(-150); //On recuele pour avoir la place de tourner
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(1700, mult*1478); //On se place en bas à gauche du lingot
		marche_avant = true;
		waitForFinish();
		asservissement.go(-100); //On recule pour pas se laisser emmerder par les CDs
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(1300, mult*1300); //On pousse le lingot près du totem
		marche_avant = true;
		waitForFinish();
		asservissement.go(-200); //On recule pour avoir la place de tourner
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(1378, mult*1210); //On se prépare à tout racler
		marche_avant = true;
		waitForFinish();
		asservissement.gotoPosition(700, mult*1210); //On râcle
		waitForFinish();
		detectionActive = false;
		asservissement.gotoPosition(350, mult*950); //On pousse tout en cale
		waitForFinish();
		detectionActive = true;
		asservissement.go(-200); //On prend de l'élan
		marche_avant = false;
		waitForFinish();
		detectionActive = false;
		asservissement.go(250); //On redonne un petit coup
		marche_avant = true;
		waitForFinish();
		detectionActive = true;
		asservissement.go(-250); //On recule pour avoir la place de tourner
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(478, mult*450); //On remonte un peu
		marche_avant = true;
		waitForFinish();
		asservissement.gotoPosition(978, mult*450); //On se place en haut du totem
		waitForFinish();
		asservissement.face(978, mult*878); //On se tourne vers le totem
		waitForFinish();
		pince.openPinceAv();
		try {
			Thread.sleep(500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		detectionActive = false;
		asservissement.go(280); //On se colle au totem
		try {
			Thread.sleep(1500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		asservissement.halt();
		asservissement.resetHalt();
		
		pince.closePinceAv();
		try {
			Thread.sleep(800);
		} catch(Exception e) {
			e.printStackTrace();
		}
		detectionActive = true;
		asservissement.go(-250); //On recule pour avoir la place de tourner
		marche_avant = false;
		waitForFinish();
		asservissement.gotoPosition(500, mult*678); //On reprend la ligne
		marche_avant = true;
		waitForFinish();
		asservissement.gotoPosition(500, mult*1178); //On descend
		waitForFinish();
		asservissement.gotoPosition(978, mult*1300); //On se place en bas du totem
		waitForFinish();
		asservissement.face(978, mult*1578); //On tourne le cul vers le totem
		waitForFinish();
		pince.openPinceAr();
		try {
			Thread.sleep(500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		detectionActive = false;
		asservissement.go(-300); //On se colle au totem
		marche_avant = false;

		try {
			Thread.sleep(1500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		asservissement.halt();
		asservissement.resetHalt();

		pince.closePinceAr();
		try {
			Thread.sleep(800);
		} catch(Exception e) {
			e.printStackTrace();
		}
		detectionActive = true;
		asservissement.gotoPosition(978, mult*1400); //On réavance
		waitForFinish();
		asservissement.gotoPosition(500, mult*1078); //On repart sur la ligne
		waitForFinish();
		asservissement.gotoPosition(620, mult*380); //On se met devant la cale
		waitForFinish();
		detectionActive = false;
		asservissement.gotoPosition(130, mult*750); //On entre en cale
		try {
			Thread.sleep(1500); //On met une tempo, parce qu'on est quasi sûr d'être bloqué
		} catch(Exception e) {
			e.printStackTrace();
		}
		asservissement.face(130, mult*950); //On s'aligne bien
		try {
			Thread.sleep(1000); //On met une tempo, parce qu'on est quasi sûr d'être bloqué
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		asservissement.go(-60); //On recule des éléments (peut-être)
		try {
			Thread.sleep(500); //On met une tempo, parce qu'on est quasi sûr d'être bloqué
		} catch(Exception e) {
			e.printStackTrace();
		}
		asservissement.go(30); //On avance dans une zone où on a supposément fait le ménage
		try {
			Thread.sleep(500); //On met une tempo, parce qu'on est quasi sûr d'être bloqué
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		pince.openPinceAvForEject();
		try {
			Thread.sleep(300);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.openPinceArForEject();
		try {
			Thread.sleep(500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		/*
		try {
			Thread.sleep(500);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.ejecter();
		try {
			Thread.sleep(100);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.fermerBenne();
		try {
			Thread.sleep(100);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.ejecter();
		try {
			Thread.sleep(100);
		} catch(Exception e) {
			e.printStackTrace();
		}
		pince.fermerBenne();
		try {
			Thread.sleep(100);
		} catch(Exception e) {
			e.printStackTrace();
		}
		*/
	

		while(true) {

						
			pince.ejecter();
			try {
				Thread.sleep(500);
			} catch(Exception e) {
				e.printStackTrace();
			}

			pince.fermerBenne();
			try {
				Thread.sleep(500);
			} catch(Exception e) {
				e.printStackTrace();
			}
	
			asservissement.go(-30); //On fait des va-et-vient pour tout bien décharger
			waitForFinish();
			asservissement.go(30); //On fait des va-et-vient pour tout bien décharger
			waitForFinish();

		}
		
		
	}

	private void testPince() {

		pince.openPinceAv();

	}

	private void waitForFinish() {
		boolean finished = false;
		while(!finished) {
			checkStopped();
			if(asservissement.lastCommandFinished()) {
				finished = true;
			}
		}
		
	}

	public void stopDeplacements() {
		if(!stopped) {
			stopped = true;
			asservissement.halt();
		}
	}
	
	public void resumeDeplacement() {
		if(stopped) {
			stopped = false;
			asservissement.resetHalt();
			asservissement.sendCommand();
		}
	}
	
	private void checkStopped() {
		while(stopped) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
