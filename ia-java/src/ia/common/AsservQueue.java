package ia.common;

import api.asserv.Asserv;
import api.asserv.actions.Action;
import ia.esialrobotik.Ia;

import java.util.ArrayList;

/**
 * Created by mickael on 13/05/15.
 */
public class AsservQueue extends Thread {

	private Action currentAction = null;
	private final ArrayList<Action> queue = new ArrayList<>();
	private Asserv asserv;
	private Ia ia;
	private boolean aborted = false;
	private boolean rerunCommand = false;

	public AsservQueue(Asserv asserv, Ia ia) {
		this.asserv = asserv;
		this.ia = ia;
	}

	/**
	 * Appelée lorsque l'exécution doit être interrompue (par une détection)
	 */
	public void abort() {
		synchronized (queue) {
			this.aborted = true;
		}
	}

	public void addAction(Action action) {
		synchronized (queue) {
			queue.add(action);
		}
	}

	public void reset() {
		synchronized (queue) {
			this.aborted = false;
		}
	}

	@Override
	public void run() {
		while (true) {
			if (!this.aborted) {
				System.out.println("not aborted");
				if (!this.rerunCommand) {
					synchronized (queue) {
						if (queue.size() > 0) {
							this.currentAction = queue.remove(0);
						} else {
							try {
								Thread.sleep(100);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
					}
				} else {
					System.out.println("rerun");
					this.rerunCommand = false;
				}

				if (this.currentAction != null) {
					System.out.println(this.currentAction.getSerialCommand());
					asserv.addAction(this.currentAction);
					asserv.setLastCommandFinished(false);
					if (this.currentAction.isBlockingCommand()) {
						System.out.println("wait for finish (with detect)");
						while (!asserv.isLastCommandFinished() && !this.aborted) {
							try {
								Thread.sleep(10);
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
						if (this.aborted) {
							this.rerunCommand = true;
						}
						ia.detection.setDetect(true);
						System.out.println("queue : finished");
					} else {
						System.out.println("no wait, no detection before next end of wait for finish");
						ia.detection.setDetect(false);
					}
				} else {
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			} else {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}

	public boolean isAborted() {
		return aborted;
	}

	public void setAborted(boolean aborted) {
		this.aborted = aborted;
	}
}
