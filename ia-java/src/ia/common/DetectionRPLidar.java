package ia.common;

import ia.nancyborg2015.Ia;
import navigation.Point;
import roboticinception.rplidar.RpLidarHighLevelDriver;
import roboticinception.rplidar.RpLidarScan;

import java.util.ArrayList;

/**
 * Created by mickael on 12/05/15.
 */
public class DetectionRPLidar implements Runnable {
	private final Ia ia;
	private String device;
	private int baudrate;
	private RpLidarHighLevelDriver rplidar;

	public DetectionRPLidar(Ia ia, String device, int baudrate) {
		this.ia = ia;
		this.device = device;
		this.baudrate = baudrate;
		rplidar = new RpLidarHighLevelDriver(ia);
		//rplidar.driver.setVerbose(true);
	}

	public void start() {
		rplidar.initialize(device, baudrate);
		new Thread(this).start();
	}

	@Override
	public void run() {
		ArrayList<Boolean> detection = new ArrayList<>();

		//detection.addAll(Arrays.asList(false, false));
		//detection.addAll(Arrays.asList(false));
		boolean stopped = false;

		for (;;) {

			RpLidarScan scan = new RpLidarScan();
			long start = System.currentTimeMillis();
			rplidar.blockCollectScan(scan, 0);
			//System.out.println("Temps scan: " + (System.currentTimeMillis() - start));

			double min = Double.MAX_VALUE;
			double min_angle = 0;
			int det = 0;

			for (int i = 0; i < scan.used.size(); i++) {
				int index = scan.used.get(i);
				float angle = (float) Math.toRadians(180f - (index / 64f));
				float distance = scan.distance[index];
				Point pos_asserv = scan.pos_asserv[index];


				if (distance > 1000 || distance <= 220) {
					continue;
				}

				if (distance > 500) { // 800
					continue;
				}


				if (Math.abs(Math.toDegrees(angle)) > 60) {
					continue;
				}

				//System.out.println("******************************************** Angle " + Math.toDegrees(angle) + ": " + distance);


				float xobs = (float) (pos_asserv.getX() + Math.cos(angle - pos_asserv.getCap()) * distance);
				float yobs = (float) (pos_asserv.getY() + Math.sin(angle - pos_asserv.getCap()) * distance);

				System.out.println("Asserv " + pos_asserv);
				System.out.println("Detection X=" + xobs + "  Y=" + yobs + " à distance=" + distance + " angle=" + Math.toDegrees(angle));


				//if (xobs < 100 || Math.abs(yobs) < 100 || xobs > (3000-100) || Math.abs(yobs) > (2000-100)) {
				if (xobs < 0 || xobs > (3000-100) || Math.abs(yobs) > (2000-100)) {
					//System.out.println("Detection hors table (" + xobs + ',' + yobs + ")");
					System.out.println("Hors table");
					continue;
				}

    			det++;

				if (distance < min) {
					min = distance;
					min_angle = angle;
				}
			}





			detection.add(det > 1);

			System.out.println(detection.toString());
			if (detection.stream().allMatch(e -> e)) {
				if (!stopped) {
					System.out.println("********* STOP ***********");
					long t = System.currentTimeMillis();
					ia.asserv.halt();
					System.out.println("apres halt : " + (System.currentTimeMillis() - t));
					stopped = true;
					System.exit(0);
				}
			} else if (detection.stream().allMatch(e -> !e)) {
				if (stopped) {
					System.out.println("******** REPART ***********");
					stopped = false;
				}
			}

			detection.remove(0);

			if (min == Double.MAX_VALUE) {
				//System.out.println("RIEN");
			}
			else {
				System.out.println("MIN : " + min + " at " + min_angle);
			}

			//Thread.sleep(10);
		}
	}

	public static void main(String[] args) throws Exception {
		Ia ia = new Ia();
		ia.rplidar.start();

		/*System.out.println("go");
		ia.asserv.go(1500, false);*/

/*			System.out.println("go 0 0 ");
		ia.asserv.gotoPosition(0, 0, true);

		System.out.println("turn 180");
		ia.asserv.turn(180, true);
*/
/*		while (true)
			ia.sleep(1000);*/

	}
}
