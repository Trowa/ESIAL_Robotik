package navigation;

import ia.common.Objectif;
import ia.esialrobotik.objectifs.Cabine;

import java.util.ArrayList;
import java.util.List;

/**
 * Classe gérant la navigation du robot.
 * Les coordonnées des points sur la table sont en millimètres, comme dans l'asserv
 * @author fprugnie
 *
 */
public class Navigation {
	
	private Table table; // La table de jeu
	private List<Objectif> objectifs; // Les objectifs à atteindre
	private TeamColor couleur; // La couleur de l'équipe
	private int yMult;
	
	public enum TeamColor {
		GREEN, // Coté droit
		VIOLET // Coté gauche
	}
	
	/**
	 * Constructeur
	 */
	public Navigation() {
		
		// On instancie la table
		table = new Table();
		
		yMult = 0;
	}
	
	/**
	 * Renvoie le multiplicateur pour les coordoonées en Y
	 * @return 1 ou -1
	 */
	public int getYmult() {
		return yMult;
	}
	
	/**
	 * Définit la couleur de l'équipe
	 * @param couleur
	 */
	public void setTeamColor(TeamColor couleur) {
		this.couleur = couleur;
		
		/* Le coté de la table dépend de la couleur, les coordonnées changent.
		 * Si on évolue du coté gauche de la table, les Y seront positifs, si c'est
		 * du côté droit, les Y seront négatifs. On gère ça avec un multiplicateur sur Y.
		 */
		yMult = (couleur == TeamColor.VIOLET) ? -1 : 1;
		
		// On reparamètre la liste des objectifs
		initListeObjectifs(couleur);
	}
	
	/**
	 * Récupère l'objectif à remplir le plus proche de la position donnée
	 * @param position
	 * @return un objectif, ou null s'il n'y a plus d'objectif à remplir
	 */
	public Objectif getObjectifProche(Point position) {
		Objectif proche = null;
		int distance_2 = Integer.MAX_VALUE;
		
		// On vérifie les objectifs
		for(Objectif o : objectifs) {
			
			// Si l'objectif est remplit, on s'en fiche
			if(o.isDone()) {
				continue;
			}
			
			// On calcule la distance jusqu'à cet objectif
			final Point positionObjectif = o.getPosition();
			final int newDistance_2 = (positionObjectif.x - position.x) * (positionObjectif.x - position.x)
					+ (positionObjectif.y - position.y) * (positionObjectif.y - position.y);
			
			// S'il est plus proche, on le sélectionne
			if(newDistance_2 < distance_2) {
				proche = o;
				distance_2 = newDistance_2;
			}
		}
		
		return proche;
	}
	
	public List<Point> getChemin(Point nous, Point cible) {
		
		Point nousTable = nous;
		Point cibleTable = cible;
		
		// Si on doit inverser les Y, il faut le faire maintenant
		if(yMult == -1) {
			nousTable = new Point(nous.x , -nous.y);
			cibleTable = new Point(cible.x, -cible.y);
		}
		
		// On récupère le chemin
		List<Point> chemin = table.getCheminToPoint(nousTable, cibleTable);
		
		if(chemin != null && yMult == -1) {
			// Il faut inverser Y dans le chemin obtenu
			for(Point p : chemin) {
				// On modifie directement les instances des points
				p.y = -p.y;
			}
		}
		
		return chemin;
	}
	
	
	/**
	 * Remplit la liste d'objectifs. Les objectifs peuvent dépendre de la couleur
	 * de l'équipe
	 * A modifier chaque année
	 */
	private void initListeObjectifs(TeamColor couleur) {
		objectifs = new ArrayList<Objectif>();
		
		// On utilise yMult pour définir les coordonnées des objectifs
		
		// Les deux cabines de plages
		Point cabine1 = new Point(300, 210 * yMult, -90 * yMult);
		Point cabine2 = new Point(600, 210 * yMult, -90 * yMult);
		
		objectifs.add(new Cabine(cabine1));
		objectifs.add(new Cabine(cabine2));
	}

	public Table getTable() {
		return this.table;
	}

}
