package navigation.astar;

/**
 * Un noeud du graphe A*
 * @author jb
 *
 */
public class Node {
	
	/*
	 * Tous les attributs en public, parce qu'on a pas envie de passer
	 * son temps à appeler des méthodes pour ça
	 */
	
	/**
	 * Position
	 */
	public int x;
	public int y;
	
	/**
	 * Cout du noeud
	 * convention: cout = -1 indique un noeud inaccessible
	 */
	public int cout;
	
	/**
	 * Heuristique
	 */
	public int heuristique;
	
	/**
	 * Le noeud précédent du chemin
	 */
	public Node parent;
	
	/**
	 * Le noeud est-il ouvert ?
	 */
	public boolean ouvert;
	
	/**
	 * Constructeur
	 */
	public Node(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	@Override
	public String toString() {
		return "[" + x + ", " + y + "]";
	}
	
	@Override
	public boolean equals(Object n) {
		return this.x == ((Node)n).x && this.y == ((Node)n).y;
	}

}
