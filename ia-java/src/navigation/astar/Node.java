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
	 * Cout du noeud: le cout pour atteindre ce noeud depuis le départ
	 */
	public int cout;
	
	/**
	 * Cout heuristique: le cout total estimé du chemin si on passe par ce noeud
	 */
	public int coutHeuristique;
	
	/**
	 * Heuristique: le cout estimé entre ce noeud et l'arrivé
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

	/**
	 * Comparaison de noeuds: le noeud le plus "petit" est le noeud qui minimise
	 * le cout total estimé.
	 */
	@Override
	public int compareTo(Node o) {
		return this.coutHeuristique - o.coutHeuristique;
	}

}
