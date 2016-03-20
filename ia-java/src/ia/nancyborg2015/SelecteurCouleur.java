package ia.nancyborg2015;

import org.mbed.RPC.DigitalIn;
import org.mbed.RPC.DigitalOut;

/**
 * Created by mickael on 12/05/15.
 */
public class SelecteurCouleur {
	private final DigitalIn in;
	private final DigitalOut out;

	public SelecteurCouleur(DigitalIn in, DigitalOut out) {
		this.in = in;
		this.out = out;
		this.out.write(1);
	}

	public Ia.TeamColor getTeamColor() {
		return in.read() == 0 ? Ia.TeamColor.GREEN : Ia.TeamColor.YELLOW;
	}
}
