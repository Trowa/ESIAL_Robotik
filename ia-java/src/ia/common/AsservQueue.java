package ia.common;

import api.asserv.Asserv;
import api.asserv.actions.Action;

import java.util.ArrayList;

/**
 * Created by mickael on 13/05/15.
 */
public class AsservQueue extends Thread {
	private ArrayList<Action> queue = new ArrayList<>();
	private Asserv asserv;
	private boolean aborted = false;

	public AsservQueue(Asserv asserv) {
		this.asserv = asserv;
	}

	/**
	 * Appelée lorsque l'exécution doit être interrompue (par une détection)
	 */
	public void abort() {
		synchronized (queue) {
			queue.clear();
			this.aborted = true;
			// todo un petit return de la queue pour la reprendre ce serait cool non ? enfin avec Astar on s'en bas les couilles
		}
	}

	public void addAction(Action action) {
		synchronized (queue) {
			queue.add(action);
		}
	}

	public void reset() {
		synchronized (queue) {
			queue.clear();
			this.aborted = false;
		}
	}

	public void waitEnd() throws InterruptedException {
		while (true) {
			synchronized (queue) {
				if (queue.isEmpty())
					return;

				if (aborted) {
					throw new InterruptedException();
				}
			}

			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public void run() {
		while (true) {
			Action action = null;

			synchronized (queue) {
				if (queue.size() > 0) {
					action = queue.remove(0);
				}
			}

			if (action != null) {
				asserv.addAction(action);

				while (!asserv.isLastCommandFinished() && !this.aborted) {
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}

			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
