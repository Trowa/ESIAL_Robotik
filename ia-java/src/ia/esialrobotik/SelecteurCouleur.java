package ia.esialrobotik;

import navigation.Navigation.TeamColor;
import api.hardware.GPIO;

public class SelecteurCouleur {
	
	/**
	 * Le GPIO où est branché le sélecteur
	 */
	private GPIO gpio;

	public SelecteurCouleur(GPIO gpio) {
		this.gpio = gpio;
	}

	public TeamColor getTeamColor() {
		return gpio.getValue() ? TeamColor.GREEN : TeamColor.VIOLET;
	}
}
