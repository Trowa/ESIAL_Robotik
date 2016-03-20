package ia.common;

import api.controllers.PololuMaestro;

import java.io.IOException;

/**
 * Created by mickael on 12/05/15.
 */
public class Pince {
	private final PololuMaestro maestro;
	private final float closedPos;
	private final float openPos;
	private final int channel;

	public Pince(PololuMaestro maestro, int channel, float closedPos, float openPos) {
		this.maestro = maestro;
		this.channel = channel;
		this.closedPos = closedPos;
		this.openPos = openPos;
	}

	/**
	 * Change la position de la pince.
	 *
	 * @param pos position entre 0 et 1 (0 = fermée, 1 = ouverte)
	 */
	public void setPosition(float pos) throws IOException {
		System.out.println("Channel " + channel + " val " + ((int) (closedPos + (openPos - closedPos) * pos)));
		maestro.setTarget(this.channel, (int) (closedPos + (openPos - closedPos) * pos));
	}

	public void shutdown() throws IOException {
		System.out.println("Shutdown channel " + channel);
		maestro.setTarget(this.channel, 0);
		maestro.setTargetMs(this.channel, 0);
	}
}
