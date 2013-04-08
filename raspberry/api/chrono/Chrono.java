package api.chrono;

import java.util.Timer;
import java.util.TimerTask;

/*
 * Chrono du robot a l'aide d'un Timer java. A la fin du temps prevu dans le schedule (89 sec),
 * le run de ChronoTask se declenche automatiquement pour mettre fin au match
 */

public class Chrono {

	protected Timer timer;
	protected int tpsMatch; // Temps de declenchement du timer en millisecondes

	public Chrono(int tpsMatch) {
		System.out.println("Creation chrono");
		this.timer = new Timer();
		this.tpsMatch = tpsMatch;
	}

	// Demarrage du chrono en configurant la TimerTask
	public void startChrono(TimerTask task) {
		System.out.println("Demarrage chrono");
		this.timer.schedule(task, tpsMatch);
	}

}
