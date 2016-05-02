package ia.common;

import api.asserv.Asserv;
import ia.esialrobotik.Ia;
import navigation.Point;

import java.util.ArrayList;

public class DeplacementTask extends Thread {

	private Asserv asserv;
	private ArrayList<Point> points;
	private Ia ia;

	public DeplacementTask(Ia ia, ArrayList<Point> points) {
		this.asserv = ia.asserv;
		this.points = points;
		this.ia = ia;
	}
	
	private void waitForFinish() {
		while(true) {
			ia.sleep(20);
			// todo lol
//			if(asserv.lastCommandFinished()) {
//				return;
//			}
		}
	}
	
	@Override
	public void run() {
		System.out.println("C'est parti !");
		for (Point p : points) {
			// todo lol
//			this.asserv.gotoPosition(p.getX(), p.getY(), false);
			this.waitForFinish();
		}
		//ia.objectifAtteint(points.get(points.size()-1));
	}
}
