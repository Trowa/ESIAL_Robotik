package navigation;

import java.util.List;

/**
 * Classe pour tester le calcul de chemin
 * @author jb
 *
 */
public class AStarTest {

	public static void main(String[] args) {
		
		// Une table
		Table table = new Table();
		
		Point robot = new Point(250, 250); // Notre position
		Point cible = new Point(2000, 1000); // Le point à atteindre

		// On démarre le chrono
		long elapsedTime = 0;
		long startTime = System.currentTimeMillis(); 
		
		// On met un adversaire sur la table
		table.setPositionAdversaire(new Point(1500, 400), 400);
	
		// On calcule le chemin
		List<Point> cheminList = table.getCheminToPoint(robot, cible);
		
		// Temps écoulé ?
		long stopTime = System.currentTimeMillis();
		elapsedTime = (stopTime - startTime);

		System.out.println("Chemin calculé en " + elapsedTime + "ms");
		
		if(cheminList != null) {
			int i = 0;
			for(Point p : cheminList) {
				System.out.println(" " + (i++) + " : " + p);
			}
		} else {
			System.out.println("Pas de chemin !");
		}

	}

}
