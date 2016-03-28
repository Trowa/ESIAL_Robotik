package navigation.astar;

import java.util.ArrayList;
import java.util.Stack;

import navigation.Point;
import navigation.RouteSimplificator;

/**
 * Classe pour tester l'Astar
 * @author jb
 *
 */
public class AStarTest {

	public static void main(String[] args) {
		
		int dimX = Integer.parseInt(args[0]);
		int dimY = Integer.parseInt(args[1]);
		
		AStar astar = new AStar(dimX, dimY);
		
		Stack<Point> chemin = null;
		
		for(int x = dimX / 4; x < dimX; x++) {
			astar.setDefinitivelyAccessible(x, dimY/2, false);			
		}
		
		astar.updateVoisinageInfo();
		
		int nbInteration = 10;
		long elapsedTime = 0;
		
		for(int i = 0; i < nbInteration; i++) {
			long startTime = System.currentTimeMillis();
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
			long stopTime = System.currentTimeMillis();
			elapsedTime += (stopTime - startTime);
		}
		elapsedTime /= nbInteration;
		
		System.out.println("Chemin calculé en " + elapsedTime + "ms (moyenne)");
			
		ArrayList<Point> cheminList = new ArrayList<Point>(chemin.size());
		while(!chemin.isEmpty()) {
			cheminList.add(chemin.pop());
		}
		
		int i = 0;
		for(Point p : cheminList) {
			System.out.println(" " + (i++) + " : " + p);
		}
		
		RouteSimplificator simplificator = new RouteSimplificator(2);
		
		ArrayList<Point> cheminListSimple = simplificator.getSimplifiedRoute(cheminList);

		i = 0;
		for(Point p : cheminListSimple) {
			System.out.println(" " + (i++) + " : " + p);
		}
		
		System.out.println("Terminé !");
		
	}

}
