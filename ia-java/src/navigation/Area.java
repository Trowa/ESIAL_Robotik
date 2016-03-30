package navigation;

import java.util.ArrayList;
import java.util.List;

/**
 * Classe abstraite utilisée pour définir des zones interdites sur la table 
 */
public abstract class Area {
	
	protected List<Point> listPoints;
	
	protected Area() {
		listPoints = new ArrayList<Point>();
	}
	
	/**
	 * Récupère la liste des points contenus dans la forme
	 * @return la liste des points
	 */
	public final List<Point> getPoints() {
		return listPoints;
	}
}
