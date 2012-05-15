package ia;

import servomoteur.Pince;
import asserv.Asserv;

public class DeplacementTask implements Runnable {
	
	private Asserv asservissement;
	private boolean sens;
	private boolean stopped;
	private Pince pince;
	
	public DeplacementTask(Asserv asservissement, boolean sens, Pince pince) {
		this.asservissement = asservissement;
		this.sens = sens;
		stopped = false;
	}

	@Override
	public void run() {
		
		System.out.println("C'est parti !");
		
		homologation();
		
		try {
			Thread.sleep(1000*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	private void homologation() {
		
		int mult = sens ? -1 : 1;
		
		asservissement.gotoPosition(518, mult*128); //Sortie zone de départ
		waitForFinish();
		asservissement.gotoPosition(518, mult*673); //On descend en suivant la ligne
		waitForFinish();
		asservissement.gotoPosition(278, mult*673); //On pousse le lingot
		waitForFinish();
		
	}
	
	private void matchBien() {
		
		int mult = sens ? 1 : -1;
		
		asservissement.gotoPosition(128, mult*518); //Sortie zone de départ
		waitForFinish();
		asservissement.gotoPosition(1428, mult*518); //On descend en suivant la ligne
		waitForFinish();
		asservissement.gotoPosition(1428, mult*1378); //On va au centre de la table
		waitForFinish();
		asservissement.gotoPosition(1203, mult*1378); //On remonte, toujours au centre
		waitForFinish();
		asservissement.gotoPosition(1428, mult*518); //On déblaye devant le bas du totem
		waitForFinish();
		asservissement.gotoPosition(1278, mult*278); //On ramène des trucs dans la cale
		waitForFinish();
		asservissement.gotoPosition(1378, mult*478); //On ramène des trucs dans la cale
		waitForFinish();
		
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
			asservissement.sendLastCommand();
		}
	}
	
	private void checkStopped() {
		while(stopped) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
