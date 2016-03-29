package navigation.astar;

import java.util.ArrayList;
import java.util.Stack;

import navigation.Point;

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
			astar.setDefinitivelyAccessible(x, dimY/2 - dimY/10, false);			
			astar.setDefinitivelyAccessible(x, dimY/2 + dimY/10, false);
		}
		
		for(int x = 0; x < 3 * dimX / 4; x++) {
			astar.setDefinitivelyAccessible(x, dimY/2, false);
		}
		
		int nbInteration = 10;
		astar.updateVoisinageInfo();

		long elapsedTime = 0;
		long startTime = System.currentTimeMillis();
	
		chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));

		long stopTime = System.currentTimeMillis();
		elapsedTime = (stopTime - startTime);

		System.out.println("Chemin calculé en " + elapsedTime + "ms");
		
		if(chemin != null) {
			ArrayList<Point> cheminList = new ArrayList<Point>(chemin.size());
			while(!chemin.isEmpty()) {
				cheminList.add(chemin.pop());
			}
			
			int i = 0;
			for(Point p : cheminList) {
				System.out.println(" " + (i++) + " : " + p);
			}
		} else {
			System.out.println("Pas de chemin !");
		}

	}

}
