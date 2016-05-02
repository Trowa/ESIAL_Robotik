package navigation;

import ia.common.Objectif;

import java.util.List;

import navigation.Navigation.TeamColor;

/**
 * Classe pour tester le calcul de chemin
 * @author jb
 *
 */
public class AStarTest {

	public static void main(String[] args) {
		
		// La navigation
		Navigation nav = new Navigation();
		
		nav.setTeamColor(TeamColor.GREEN);
		
		Point nous = new Point(210, nav.getYmult() * 800);
		
		Objectif o = null;
		
		do {
		
			o = nav.getObjectifProche(nous);
			
			if(o != null) {
		
				System.out.println(o.getClass().getName() + " : " + o.getPosition());
		
				List<Point> chemin = nav.getChemin(nous, o.getPosition());
		
				int i = 0;
				for(Point p : chemin) {
					System.out.println((++i) + " : " + p);
				}
				
				nous = chemin.get(chemin.size() - 1);
				o.setDone();
			}
		} while(o != null);
	}

}
