package navigation;

/**
 * Un carrée: cas particulier d'un rectangle, mais on va pouvoir
 * le définir par un centre et un côté
 * @author jb
 *
 */
public class Carre extends Rectangle {

	/**
	 * Une forme carré
	 * @param centre Centre du carré
	 * @param dimCote Dimension d'un coté du carré
	 */
	public Carre(Point centre, int dimCote) {
		super(centre.x - dimCote/2, centre.y - dimCote/2, centre.x + dimCote/2, centre.y + dimCote/2);
	}

}
