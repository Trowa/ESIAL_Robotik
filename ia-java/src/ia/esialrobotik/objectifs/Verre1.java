package ia.esialrobotik.objectifs;

import api.asserv.actions.Go;
import api.asserv.actions.Goto;
import ia.common.Objectif;
import ia.esialrobotik.Ia;
import navigation.Point;

/**
 * Created by mickael on 13/05/15.
 */
public class Verre1 extends Objectif {
	protected Verre1(Ia ia) {
		super(ia);
	}

	@Override
	public boolean isCancellable() {
		return false;
	}

	@Override
	public Point getPosition() {
		return new Point(
				850,
				ia.ymult * (2000-830));
	}

	@Override
	public void run() throws InterruptedException {
		queue.addAction(new Go(100));
		queue.addAction(new Goto(950, ia.ymult * 1100));
		queue.addAction(new Goto(600, ia.ymult * 1100));
		queue.addAction(new Go(-100));
		queue.waitEnd();
	}
}
