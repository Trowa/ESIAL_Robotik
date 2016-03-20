package ia.nancyborg2015;

import org.mbed.RPC.DigitalIn;
import org.mbed.RPC.DigitalOut;

/**
 * Created by mickael on 12/05/15.
 */
public class Tirette {
	private final DigitalIn in;
	private final DigitalOut out;

	public Tirette(DigitalIn in, DigitalOut out) {
		this.in = in;
		this.out = out;
		this.out.write(1);
	}

	public boolean isPulled() {
		return in.read() == 0;
	}

	public void wait(boolean pulled) {
		while (isPulled() != pulled) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
