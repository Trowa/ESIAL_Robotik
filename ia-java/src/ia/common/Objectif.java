package ia.common;

import api.asserv.Asserv;
import ia.nancyborg2015.Ia;
import navigation.Point;

/**
 * Created by mickael on 13/05/15.
 */
public abstract class Objectif {
	protected Ia ia;
	protected AsservQueue queue;
	protected Asserv asserv;
	protected boolean done;

	protected Objectif(Ia ia) {
		this.ia = ia;
		this.done = false;
		this.asserv = ia.asserv;
		this.queue = ia.queue;
	}

	public boolean isDone() {
		return done;
	}

	public boolean isReachable() {
		return true;
	}

	public float getWeight() {
		return 1f;
	}

	public abstract void run() throws InterruptedException;
	public abstract boolean isCancellable();
	public abstract Point getPosition();
}
