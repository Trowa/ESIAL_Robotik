package navigation;

import java.util.ArrayList;
import java.util.List;

/**
 * Un rectangle
 * @author jb
 *
 */
public class Rectangle implements Area {
	
	private List<Point> listPoints;
	
	/**
	 * Deux points suffisent à définir un rectangle
	 * @param a Un sommet du rectangle
	 * @param b Le sommet diagonalement opposé
	 */
	public Rectangle(Point a, Point b) {
		this(a.x, a.y, b.x, b.y);
	}
	
	/**
	 * Définit un rectangle avec ses coordonnées
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 */
	public Rectangle(int x1, int y1, int x2, int y2) {
		
		// On vérifie que les points donnés sont dans le bon ordre
		final int xMin = x1 < x2 ? x1 : x2;
		final int xMax = x1 > x2 ? x1 : x2;
		final int yMin = y1 < y2 ? y1 : y2;
		final int yMax = y1 > y2 ? y1 : y2;
		
		// On construit directement la liste de points
		// On connait le nombre de points à générer, donc on préalloue la liste
		listPoints = new ArrayList<Point>((xMax - xMin + 1) * (yMax - yMin + 1));
		
		for(int x = xMin; x <= xMax ; x++) {
			for(int y = yMin; y <= yMax ; y++) {
				listPoints.add(new Point(x, y));
			}
		}
	}

	@Override
	public List<Point> getPoints() {
		return listPoints;
	}

}
