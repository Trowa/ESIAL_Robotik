package ia.esialrobotik;

import api.hardware.GPIO;

public class SelecteurCouleur {
	
	/**
	 * Le GPIO où est branché le sélecteur
	 */
	private GPIO gpio;

	public SelecteurCouleur(GPIO gpio) {
		this.gpio = gpio;
	}

	public Ia.TeamColor getTeamColor() {
		return gpio.getValue() ? Ia.TeamColor.GREEN : Ia.TeamColor.YELLOW;
	}
}
