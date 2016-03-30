package navigation.astar;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import navigation.Point;
import navigation.Point.DIRECTION;

/**
 * Une classe pour simplifier des chemins calculés par l'A*
 * Fonctionne correctement si les chemins sont des droites de 0°, 45°, 90°, etc...
 * et si les points sont côtes à côtes (i.e. la différence de coordoonées entre deux
 * points successifs est de 1 maximum)
 * @author jb
 *
 */
public abstract class LineSimplificator {

	/**
	 * Simplifie un chemin en ne gardant que les points correspondants à
	 * des segments de droite.
	 * @param chemin Le chemin compliqué
	 * @return Un chemin plus simple
	 */
	public static List<Point> getSimpleLines(Collection<Point> chemin) {
		// S'il n'y a pas de chemin, on ne renvoie rien non plus !
		if(chemin == null) {
			return null;
		}
		
		// La liste des points pour le chemin simple
		List<Point> cheminSimple = new ArrayList<Point>();
		
		// Est-ce qu'il y a des points ?
		if(!chemin.isEmpty()) {
			// On ne connait pas la direction du segment
			DIRECTION direction = DIRECTION.NULL;
			
			// Le point précédent, parce qu'on en aura besoin
			Point precedent = null;
			
			// On parcourt tous les points
			for(Point courant : chemin) {
				
				if(precedent == null) {
					// Cas initial: on ajoute le premier point au chemin simple
					cheminSimple.add(courant);
					
				} else if(direction == DIRECTION.NULL) {
					// on ne connait pas encore la direction de la droite, c'est qu'on est au début,
					// on va la calculer
					direction = precedent.getDirectionToGoTo(courant);
					
				} else {
					// On récupère la nouvelle direction
					DIRECTION newDirection = precedent.getDirectionToGoTo(courant);
					
					// La direction change !
					if(newDirection != direction) {
						// Le point précédent est la fin d'un segment de droite, on l'ajoute au chemin simple
						cheminSimple.add(precedent);
						direction = newDirection; // On change de direction
					}
					
				}
				
				// On sauvegarde le point
				precedent = courant;
			}
			
			// Il faut encore ajouter le dernier point
			if(precedent != null && !cheminSimple.contains(precedent)) {
				cheminSimple.add(precedent);
			}
			
		}
		
		return cheminSimple;
	}
	
}
