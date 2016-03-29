package navigation;

import java.util.List;

/**
 * Interface utilisée pour définir des zones interdites sur la table 
 */
public interface Area {
	
	/**
	 * Récupère la liste des points contenus dans la forme
	 * @return la liste des points
	 */
	public List<Point> getPoints();
}
