package chrono;

import ia.IA;

import java.util.Timer;
import java.util.TimerTask;

/*
 * Chrono du robot a l'aide d'un Timer java. A la fin du temps prevu dans le schedule (89 sec),
 * le run de ChronoTask se declenche automatiquement pour mettre fin au match
 */

public class Chrono {

	protected Timer timer;
	protected int tpsMatch; // Temps de declenchement du timer en millisecond

	public Chrono(int tpsMatch) {
		System.out.println("Création chrono");
		this.timer = new Timer();
		this.tpsMatch = tpsMatch;
	}

	// Demarrage du chrono en configurant la TimerTask
	public void startChrono() {
		System.out.println("Démarrage chrono");
		this.timer.schedule(new ChronoTask(), tpsMatch);
	}

	private class ChronoTask extends TimerTask {

		@Override
		public void run() {
			// TODO Fin du chrono, on stop tout
			System.out.println("Fin du chrono");
			try {
				IA.finDuMatch();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.exit(0);
		}

	}
}
