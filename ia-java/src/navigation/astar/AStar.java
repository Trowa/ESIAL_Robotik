package navigation.astar;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.PriorityQueue;
import java.util.Stack;

import navigation.Point;

/**
 * Une classe qui fait un calcul de trajectoire avec l'algorithme A*
 * @author jb
 *
 */
public class AStar {
	
	/**
	 * La distance des points. On utilise des entiers, donc pour la distance des diagonales,
	 * on arrondit sqrt(2) à 1.4 et on multiplie tout par 10
	 */
	private static final int DIST_DIAGONALE = 14;
	private static final int DIST_H_V = 10;

	/**
	 * La dimension de la grille
	 */
	private int dimX;
	private int dimY;
	
	/**
	 * La grille
	 */
	private Node[][] grille;
	
	/**
	 * Les noeuds à examiner: une file ordonnée
	 */
	private PriorityQueue<Node> ouverts;
	
	/**
	 * Crée un calculateur de chemin basé sur l'algo Astar
	 * @param dimX la taille de la grille en X
	 * @param dimY la taille de la grille en Y
	 */
	public AStar(int dimX, int dimY) {
		
		System.out.println("Création objet AStar");
		
		this.dimX = dimX;
		this.dimY = dimY;
		
		grille = new Node[dimX][dimY];
		for(int x = 0; x < dimX; x++) {
			for(int y = 0; y < dimY; y++) {
				grille[x][y] = new Node(x, y);
			}
		}
		
		System.out.println("Grille initialisée !");
		
		ouverts = new PriorityQueue<Node>(dimX * dimY);
		
		System.out.println("Priority queue initialisée !");
	}
	
	/**
	 * Controle l'accessibilité d'une case de la grille
	 * Cette méthode est "définitive": à utiliser à l'init pour définir
	 * les zones physiquements inaccessibles sur la table, genre bordures et cie.
	 * @param x 
	 * @param y
	 * @param accessible
	 */
	public void setDefinitivelyAccessible(int x, int y, boolean accessible) {
		
		//  Si une case n'est pas accessible, on met le noeud correspondant à null
		if(accessible && grille[x][y] == null) {
			grille[x][y] = new Node(x, y);
		} else if(!accessible) {
			grille[x][y] = null;
		}
	}
	
	/**
	 * Controle l'accès à un noeud de manière temporaire
	 * A utiliser juste avant de calculer un chemin, pour définir une zone inaccessible
	 * parce qu'un robot adverse y est, par exemple...
	 * @param x
	 * @param y
	 * @param accessible
	 */
	public void setTemporaryAccessible(int x, int y, boolean accessible) {
		if(grille[x][y] != null) {
			grille[x][y].accessible = accessible;
		}
	}
	
	private Node getNode(int x, int y) {
		if(x >= 0 && x < dimX && y >= 0 && y < dimY) {
			return grille[x][y];
		}
		return null;
	}
	
	/**
	 * Cette fonction est à appeler INDISPENSABLEMENT APRÈS avoir défini les zones
	 * définitivement inaccessible !
	 * Si c'est appelé avant ou pas appelé, ça fera N'IMPORTE QUOI !
	 */
	public void updateVoisinageInfo() {
		for(int x = 0; x < dimX; x++) {
			for(int y = 0; y < dimY; y++) {
				
				Node node = grille[x][y];
				
				if(node == null) {
					continue;
				}
				
				/*
				 * Comme on est des oufs, on a une méthode qui renvoie null si le noeud
				 * n'existe pas, donc on ne vérifie pas les bornes ici.
				 */
				node.voisin_n = getNode(x-1,y);
				node.voisin_s = getNode(x+1,y);
				node.voisin_e = getNode(x,y-1);
				node.voisin_w = getNode(x,y+1);
				node.voisin_ne = getNode(x-1,y-1);
				node.voisin_nw = getNode(x-1,y+1);
				node.voisin_se = getNode(x+1,y-1);
				node.voisin_sw = getNode(x+1,y+1);
				
			}
		}
	}
	
	private void updateCout(Node courant, Node suivant, int cout) {
		
		if(suivant == null || suivant.ferme || !suivant.accessible) {
			// Rien à faire
			return;
		}
		
		int nouveauCout = suivant.heuristique + cout;
		
		// Si on n'a pas examiné le point, ou que le chemin améliore le cout...
		if(!suivant.ouvert || nouveauCout < suivant.cout) {
			// ... on met à jour le cout...
			suivant.cout = nouveauCout;
			
			// ... on ajoute aux ouverts...
			ouverts.add(suivant);
			suivant.ouvert = true;
			
			// ... et on met à jour le chemin
			suivant.parent = courant;
		}
	}
	
	private void calculChemin(int startX, int startY, int objectifX, int objectifY) {
		
		System.out.println("On a lancé le bouzin !");
		
		// ON VIDE TOUT !!!
		for(int x = 0; x < dimX; x++) {
			final int distX = Math.abs(objectifX - x);
			
			for(int y = 0; y < dimY; y++) {
				final Node temp = grille[x][y];
				
				if(temp != null) {
					temp.cout = Integer.MAX_VALUE;
					temp.parent = null;
					temp.heuristique = (distX + Math.abs(objectifY - y)) * DIST_H_V;
					temp.ouvert = false;
					temp.ferme = false;
				}
			}
		}
		System.out.println("Astar initialisé, on lance le calcul");
		
		ouverts.clear();
		
		Node courant = null;
		grille[startX][startY].cout = 0;
		
		// On ajoute le noeud de départ à la liste des ouverts
		ouverts.add(grille[startX][startY]);
		grille[startX][startY].ouvert = true;
		
		// Boucle de calcul
		while(true) {
			
			// On prend le noeud le plus intéressant (cout le plus faible)
			courant = ouverts.poll();
			
			if(courant == null) {
				// Plus rien à faire
				return;
			}
			
			if(courant.ferme) {
				/*
				 * Attention, code moche ! Vu qu'on peut ajouter plus d'une fois un noeud
				 * lorsque que le cout s'améliore, il faut vérifier si on ne sort pas un noeud déjà
				 * traité. S'il a déjà été traité, il a été traité dans un cas où le cout est inférieur,
				 * donc on ignore.
				 */
				continue;
			}
			
			// On "ferme" le noeud
			courant.ferme = true;
			courant.ouvert = false;
			
			// On vérifie si on arrivé
			if(courant.equals(grille[objectifX][objectifY])) {
				return;
			}
			
			// on vérifie les noeuds adjacents
			final int cout_h_v = courant.cout + DIST_H_V;
			final int cout_diag = courant.cout + DIST_DIAGONALE;
			
			updateCout(courant, courant.voisin_n, cout_h_v);
			updateCout(courant, courant.voisin_s, cout_h_v);
			updateCout(courant, courant.voisin_e, cout_h_v);
			updateCout(courant, courant.voisin_w, cout_h_v);
			
			updateCout(courant, courant.voisin_ne, cout_diag);
			updateCout(courant, courant.voisin_nw, cout_diag);
			updateCout(courant, courant.voisin_se, cout_diag);
			updateCout(courant, courant.voisin_sw, cout_diag);			
		}
		
	}
	
	/**
	 * Récupère une pile de points à parcourir entre deux points
	 * @param start le point de départ
	 * @param objectif le point d'arrivé
	 * @return une pile de point, commençant par le point de départ
	 */
	public Stack<Point> getChemin(Point start, Point objectif) {
		
		// Le chemin calculé
		Stack<Point> leChemin = new Stack<Point>();
		
		// Test tout con, parce qu'on sait jamais...
		if(start.x == objectif.x && start.y == objectif.y) {
			// start = objectif, alors on s'emmerde pas !
			leChemin.add(start);
			return leChemin;
		}
		
		// On vérifie les index, parce qu'on sait jamais non plus...
		if(start.x < 0 || start.x >= dimX || start.y < 0 || start.y >= dimY
				|| objectif.x < 0 || objectif.x > dimX || objectif.y < 0 || objectif.y >= dimY) {
			throw new IndexOutOfBoundsException("Point en dehors de la table !");
		}
		
		// On lance le bouzin !
		calculChemin(start.x, start.y, objectif.x, objectif.y);
		
		// On remplie la pile avec les points
		Node courant = grille[objectif.x][objectif.y];
		
		// Si l'objectif n'a pas de parent, c'est qu'il n'y a pas de chemin !
		if(courant.parent == null) {
			return null;
		}
		
		while(courant != null) {
			leChemin.add(new Point(courant.x, courant.y));
			courant = courant.parent;
		}
		
		return leChemin;
	}
	
	
	public static void main(String args[]) {
		
		int dimX = Integer.parseInt(args[0]);
		int dimY = Integer.parseInt(args[1]);
		
		AStar astar = new AStar(dimX, dimY);
		
		Stack<Point> chemin = null;
		
		//astar.setDefinitivelyAccessible(49, 35, false);
		
		astar.updateVoisinageInfo();
		
		//while(true) {
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2, dimY/2));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2, dimY/2));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2, dimY/2));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2, dimY/2));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2, dimY/2));
		//}
		
		Point p;
		int i = 0;
		while(!chemin.isEmpty()) {
			System.out.println(" " + (i++) + " : " + chemin.pop());
		}

		System.out.println("Terminé, connard !");
		
	}
}
