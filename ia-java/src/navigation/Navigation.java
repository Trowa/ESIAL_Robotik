package navigation;

import java.util.ArrayList;
import java.util.List;

/**
 * Classe abstraite gérant la navigation du robot, il suffit de l'étendre chaque année en remplissant les méthodes qui vont bien
 * @author fprugnie
 *
 */
public abstract class Navigation {
	
	protected DStarLite dStar;
	protected ArrayList<Point> zonesInterdites;
	protected ArrayList<Point> zonesInterditesMobiles;
	protected Point goal;
	protected ArrayList<Point> objectifs;
	protected ArrayList<Point> commandes;
	protected int pas;
	
	public Navigation() {
		this.dStar = new DStarLite();
		this.zonesInterdites = new ArrayList<Point>();
		this.zonesInterditesMobiles = new ArrayList<Point>();
		this.objectifs = new ArrayList<Point>();
		this.commandes = new ArrayList<Point>();
		
		this.init();
		this.initZonesInterdites();
		this.initListeObjectifs();
	}
	
	protected abstract void init();
	
	/**
	 * Contient les zones interdites fixes
	 * Attention à bien prendre en compte la largeur du robot dans vos calculs !!
	 */
	protected abstract void initZonesInterdites();
	
	/**
	 * Remplit la zone interdites mobiles à partir de la position de l'adversaire
	 * @param x Position en X de l'obstacle (prendre en cm l'entier le plus proche)
	 * @param y Position en Y de l'obstacle (prendre en cm l'entier le plus proche)
	 */
	protected abstract void setZonesInterditesMobiles(int x, int y);
	
	/**
	 * Renvoit les extrêmes d'une zone interdites
	 * @param adversaire Position de l'adversaire (prendre en cm l'entier le plus proche)
	 * @return Un tableau de deux Point contenant (xmin, ymin) et (xmax, ymax) d'une zone interdite
	 */
	public abstract Point[] getExtremeZoneInterdite(Point adversaire);
	
	/**
	 * Remplit la liste d'objectifs
	 */
	protected abstract void initListeObjectifs();
	
	/**
	 * On vient de rencontrer un obstacle, on met à jours les zones interdites et on recalcule l'itinéraire
	 * @param obstacleX Position en X de l'obstacle (prendre en cm l'entier le plus proche)
	 * @param obstacleY Position en Y de l'obstacle (prendre en cm l'entier le plus proche)
	 * @param robotX Position en X du robot (prendre en cm l'entier le plus proche)
	 * @param robotY Position en Y du robot (prendre en cm l'entier le plus proche)
	 * @return true si l'objectif est encore atteignable, false sinon
	 */
	public boolean obstacleMobile(Point adversaire, Point robot) {
		// On nettoie les anciennes zones interdites
		this.resetObstacleMobile();
		
		// On calcule la nouvelle zone et on la stocke
		this.setZonesInterditesMobiles(adversaire.getX(), adversaire.getY());
		
		// On place la nouvelle zone interdite
		for (Point p : this.zonesInterditesMobiles) {
			this.dStar.updateCell(p.getX(), p.getY(), -1);
		}
		
		return calculItineraire(robot);
	}
	
	public void resetObstacleMobile() {
		for (Point p : this.zonesInterditesMobiles) {
			this.dStar.updateCell(p.getX(), p.getY(), 1);
		}
		this.zonesInterditesMobiles.clear();
	}
	
	/**
	 * On déclenche un calcul de l'itinaire à suivre
	 * @param startX Position en X du robot (prendre en cm l'entier le plus proche)
	 * @param startY Position en Y du robot (prendre en cm l'entier le plus proche)
	 * @return true si l'objectif est atteignable, false sinon
	 */
	public boolean calculItineraire(Point start) {
		long time = System.currentTimeMillis();
		System.out.println("DStar start");		
		this.dStar.updateStart((int)Math.rint((double)start.getX()/(double)pas), (int)Math.rint((double)start.getY()/(double)pas));
		System.out.println("updateStart ok : "+start+" - "+(int)Math.rint((double)start.getX()/(double)pas)+";"+(int)Math.rint((double)start.getY()/(double)pas));
		this.dStar.updateGoal(this.goal.getX(), this.goal.getY());
		System.out.println("updateGoal ok : "+this.goal);
		boolean res = this.dStar.replan();
		System.out.println("DStar end : "+(System.currentTimeMillis()-time)+"ms");
		return res;
	}
	
	/**
	 * Permet de récupérer la liste des positions à transmettre à l'asservissement
	 * @return ArrayList<Point> des positions du parcours
	 */
	public ArrayList<Point> getCommandeAsserv() {
		this.commandes = new ArrayList<Point>();
		List<State> path = this.dStar.getPathReduced();
		
		for (State i : path) {
			commandes.add(new Point(i.x,i.y));
		}
		System.out.println(commandes);
		return commandes;
		
		// Méthode JBG, semble moins legèrement moins bonne mais plus propre
		//return this.dStar.getRoute();
	}
	
	/**
	 * Permet de récupérer les points de passage sans tout recalculer
	 * @return ArrayList<Point> this.commandes
	 */
	public ArrayList<Point> getCachedCommandeAsserv() {
		return this.commandes;
	}
	
	public void debugZoneInterdites() {
		System.out.println("================= Debug zones interdites ===================");
		for (Point state : this.zonesInterdites) {
			System.out.println(state.getX()+";"+state.getY());
		}
		System.out.println("================= Fin debug zones interdites ===================");
	}
	
	public void debugZoneInterditesMobiles() {
		System.out.println("================= Debug zones interdites mobiles ===================");
		for (Point state : this.zonesInterditesMobiles) {
			System.out.println(state.getX()+";"+state.getY());
		}
		System.out.println("================= Fin debug zones interdites mobiles ===================");
	}
	
	public ArrayList<Point> getZonesInterdites() {
		return zonesInterdites;
	}

	public void setZonesInterdites(ArrayList<Point> zonesInterdites) {
		this.zonesInterdites = zonesInterdites;
	}

	public ArrayList<Point> getZonesInterditesMobiles() {
		return zonesInterditesMobiles;
	}

	public void setZonesInterditesMobiles(ArrayList<Point> zonesInterditesMobiles) {
		this.zonesInterditesMobiles = zonesInterditesMobiles;
	}

	public ArrayList<Point> getObjectifs() {
		return objectifs;
	}

	public void setObjectifs(ArrayList<Point> objectifs) {
		this.objectifs = objectifs;
	}
	
	public Point getGoal() {
		return this.goal;
	}
	
	public void setGoal(Point goal) {
		this.goal = goal;
	}

}
