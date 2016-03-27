package navigation.astar;

import java.util.ArrayList;
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
	 * Les noeuds déjà examiné: une grille de booléen pour aller plus vite
	 */
	private boolean[][] fermes;
	
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
		
		ouverts = new PriorityQueue<Node>(dimX * dimY, (Object o1, Object o2) -> {
			Node n1 = (Node) o1;
			Node n2 = (Node) o2;
			
			return n1.cout - n2.cout;
		});
		
		System.out.println("Priority queue initialisée !");
		
		fermes = new boolean[dimX][dimY];
		
		System.out.println("Fermés initialisés !");
	}
	
	/**
	 * Controle l'accessibilité d'une case de la grille
	 * @param x 
	 * @param y
	 * @param accessible
	 */
	public void setAccessible(int x, int y, boolean accessible) {
		
		//  Si une case n'est pas accessible, on met le noeud correspondant à null
		if(accessible && grille[x][y] == null) {
			grille[x][y] = new Node(x, y);
		} else if(!accessible) {
			grille[x][y] = null;
		}
	}
	
	private void updateCout(Node courant, Node suivant, int cout) {
		
		if(suivant == null || fermes[suivant.x][suivant.y]) {
			// Rien à faire
			return;
		}
		
		int nouveauCout = suivant.heuristique + cout;
		
		// Si on n'a pas examiné le point, ou que le chemin améliore le cout...
		if(!suivant.ouvert || nouveauCout < suivant.cout) {
			// ... on ajoute aux ouverts si besoin...
			if(!suivant.ouvert) {
				ouverts.add(suivant);
				suivant.ouvert = true;
			}
			// ... on met à jour le cout...
			suivant.cout = nouveauCout;
			
			// ... et on met à jour le chemin
			suivant.parent = courant;
		}
	}
	
	private void calculChemin(int startX, int startY, int objectifX, int objectifY) {
		
		System.out.println("On a lancé le bouzin !");
		
		// ON VIDE TOUT !!!
		for(int x = 0; x < dimX; x++) {
			for(int y = 0; y < dimY; y++) {
				final Node temp = grille[x][y];
				
				if(temp != null) {
					temp.cout = Integer.MAX_VALUE;
					temp.parent = null;
					temp.heuristique = (Math.abs(objectifX - x) + Math.abs(objectifY - y)) * DIST_H_V;
					temp.ouvert = false;
				}
				fermes[x][y] = false;
			}
		}
		System.out.println("Astar initialisé, on lance le calcul");
		
		ouverts.clear();
		
		Node courant = null;
		
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
			
			// On "ferme" le noeud
			fermes[courant.x][courant.y] = true;
			courant.ouvert = false;
			
			// On vérifie si on arrivé
			if(courant.equals(grille[objectifX][objectifY])) {
				return;
			}
			
			// on vérifie les noeuds adjacents
			if(courant.x - 1 >= 0) {
				updateCout(courant, grille[courant.x - 1][courant.y], courant.cout + DIST_H_V);
				
				if(courant.y - 1 >= 0) {
					updateCout(courant, grille[courant.x - 1][courant.y - 1], courant.cout + DIST_DIAGONALE);
				}

				if(courant.y + 1 < dimY) {
					updateCout(courant, grille[courant.x - 1][courant.y + 1], courant.cout + DIST_DIAGONALE);
				}
            }
			
			if(courant.y - 1 >= 0) {
				updateCout(courant, grille[courant.x][courant.y - 1], courant.cout + DIST_H_V);
			}

			if(courant.y + 1 < dimY) {
				updateCout(courant, grille[courant.x][courant.y + 1], courant.cout + DIST_H_V);
			}

			if(courant.x + 1 < dimX) {
				updateCout(courant, grille[courant.x + 1][courant.y], courant.cout + DIST_H_V);
				
				if(courant.y - 1 >= 0) {
					updateCout(courant, grille[courant.x + 1][courant.y - 1], courant.cout + DIST_DIAGONALE);
				}

				if(courant.y + 1 < dimY) {
					updateCout(courant, grille[courant.x + 1][courant.y + 1], courant.cout + DIST_DIAGONALE);
				}
            }
			
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
		
		//astar.setAccessible(2, 2, false);
		
		//while(true) {
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
			chemin = astar.getChemin(new Point(dimX/2 - dimX/3, dimY/2 - dimY/3), new Point(dimX/2 + dimX/3, dimY/2 + dimY/3));
		//}
		
		Point p;
		int i = 0;
		while(!chemin.isEmpty()) {
			System.out.println(" " + (i++) + " : " + chemin.pop());
		}

		System.out.println("Terminé, connard !");
		
	}
}
