package ia.esialrobotik;


public class SelecteurCouleur {
	/*
	 * JBH TODO
	 * Gérer directement les GPIOs de la raspberry pi
	 */

	public SelecteurCouleur(/* GPIO gpio */) {
		// Initialiser un GPIO (prendre en paramètre un objet GPIO ?)
	}

	public Ia.TeamColor getTeamColor() {
		return /* Code pour lire le GPIO */ true ? Ia.TeamColor.GREEN : Ia.TeamColor.YELLOW;
	}
}
