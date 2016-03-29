package navigation.astar;

/**
 * Un noeud du graphe A*
 * @author jb
 *
 */
public class Node implements Comparable<Node> {
	
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
	 * Le noeud est-il fermé ?
	 */
	public boolean ferme;
	
	/**
	 * Le noeud est-il accessible ?
	 */
	public boolean accessible;
	
	/**
	 * Les noeuds voisins
	 */
	public Node voisin_n;
	public Node voisin_s;
	public Node voisin_e;
	public Node voisin_w;
	public Node voisin_ne;
	public Node voisin_nw;
	public Node voisin_se;
	public Node voisin_sw;
	
	/**
	 * Constructeur
	 */
	public Node(int x, int y) {
		this.x = x;
		this.y = y;
		accessible = true;
	}
	
	@Override
	public String toString() {
		return "[" + x + ", " + y + "]";
	}
	
	@Override
	public boolean equals(Object n) {
		return this.x == ((Node)n).x && this.y == ((Node)n).y;
	}

	@Override
	public int compareTo(Node o) {
		return this.cout - o.cout;
	}

}
