package roboticinception.rplidar;

import navigation.Point;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Storage for a complete scan
 *
 * @author Peter Abeles
 */
public class RpLidarScan {

	public static final int N = 360*64-1;

	public int quality[] = new int[N];
	public float distance[] = new float[N];
	/** System.currentTimeMillis() when a new observation arrived */
	public long time[] = new long[N];
	/** index of elements which were written to */
	public ArrayList<Integer> used = new ArrayList<>();
	public Point[] pos_asserv = new Point[N];


	/**
	 * Copies 'scan' into this scan.
	 *
	 * @param scan The scan which is to be copied.
	 */
	public void set( RpLidarScan scan ) {
		System.arraycopy(scan.quality,0,quality,0,N);
		System.arraycopy(scan.distance,0,distance,0,N);
		System.arraycopy(scan.time,0,time,0,N);
		System.arraycopy(scan.pos_asserv,0,pos_asserv,0,N);
		used.clear();
		used.addAll(scan.used);
	}


	public void reset() {
		// mark them all as invalid
		Arrays.fill(distance,0);
		used.clear();
	}

	public boolean isInvalid( int which ) {
		return distance[which] == 0;
	}
}
