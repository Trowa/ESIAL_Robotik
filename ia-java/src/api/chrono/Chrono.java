package api.chrono;

import java.util.Timer;
import java.util.TimerTask;

/**
 * <b>Chrono du robot a l'aide d'un Timer.</b>
 * <p>
 * A la fin du temps prévu, la mêhode {@link TimerTask#run()} se declenche
 * automatiquement pour mettre fin au match (ou faire autre chose)
 * </p>
 * 
 * @author GaG <francois.prugniel@esial.net>
 * @version 1.0
 */

public class Chrono {

	/**
	 * Timer java
	 */
	protected Timer timer;
	/**
	 * Temps de déclenchement du timer (en millisecondes)
	 */
	protected int tpsMatch;

	/**
	 * Constructeur du chrono
	 * 
	 * @param tpsMatch Temps au bout duquel le chrono déclenche sa méthode de
	 *            fin en ms
	 */
	public Chrono(int tpsMatch) {
		System.out.println("Creation chrono");
		this.timer = new Timer();
		this.tpsMatch = tpsMatch;
	}

	/**
	 * @return Renvoie le temps du chrono en millisecondes
	 */
	public int getTpsMatch() {
		return tpsMatch;
	}

	/**
	 * @param tpsMatch Temps du chrono en millisecondes à modifer
	 */
	public void setTpsMatch(int tpsMatch) {
		this.tpsMatch = tpsMatch;
	}

	/**
	 * Démarre le chrono. La méthode {@link TimerTask#run()} task sera appellée
	 * à la fin du chrono.
	 * 
	 * @param task La tâche {@link TimerTask#run()} qui sera effectuée à la fin
	 *            du chrono
	 */
	public void startChrono(TimerTask task) {
		System.out.println("Demarrage chrono");
		this.timer.schedule(task, tpsMatch);
	}
}
