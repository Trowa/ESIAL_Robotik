package navigation;

public class Cercle extends Area {
	
	/**
	 * Un cercle, défini par son centre et son rayon
	 * @param centre
	 * @param rayon
	 */
	public Cercle(Point centre, int rayon) {
		
		super();
		rayon = Math.abs(rayon); // On sait jamais...
		
		// On calcule les coordonnées du carré circonscrit au cercle
		final int minX = centre.x - rayon;
		final int maxX = centre.x + rayon;
		final int minY = centre.y - rayon;
		final int maxY = centre.y + rayon;
		
		// Une partie du carré ne fait pas partie du cercle, mais on n'a plus
		// beaucoup de points à examiner
		// On vérifie le carré de la distance: pas besoin de prendre la racine
		final int rayon_2 = rayon * rayon;
		
		for(int x = minX; x <= maxX; x++) {
			final int distX_2 = (centre.x - x) * (centre.x - x); 
			
			for(int y = minY; y <= maxY; y++) {
				// Si le carré de la distance est inférieur au rayon,
				if(distX_2 + ((centre.y - y) * (centre.y - y)) <= rayon_2) {
					// ... le point est dans le cercle
					listPoints.add(new Point(x, y));
				}
			}
		}
		
	}

}
