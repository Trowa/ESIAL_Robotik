package ia.esialrobotik;

import api.hardware.GPIO;

public class Tirette {

	/**
	 * Le GPIO où est branché la tirette
	 */
	private GPIO gpio;
	
	public Tirette(GPIO gpio) {
		this.gpio = gpio;
	}

	public boolean isPulled() {
		return !gpio.getValue();
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
