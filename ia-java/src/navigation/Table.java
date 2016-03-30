package navigation;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import navigation.astar.AStar;
import navigation.astar.LineSimplificator;

/**
 * Une table de jeu, avec des zones inaccessibles et un robot adverse.
 * Permet de calculer des chemins pour se déplacer sur cette table.
 * Toutes les méthodes accessibles publiquement causent en millimètres,
 * comme l'asserv. En interne, on s'autorise à utiliser un pas plus grand,
 * notamment pour les calculs de chemin.
 * 
 * La méthode {@link #getZonesInterditesStatiques()} est à modifier chaque année.
 * @author jb
 *
 */
public class Table {
	
	/**
	 * Le pas de calcul de chemin, en millimètre.
	 * La valeur est volontairement une constante, parce qu'on ne
	 * peut pas le changer sans changer la définition des zones inaccessibles.
	 */
	private static final int PAS_CALCUL = 10;
	
	/**
	 * Les dimensions de la table, en millimètre.
	 * Pareil, ce sont des constantes propres à la table
	 */
	public static final int dimX = 3000;
	public static final int dimY = 2000;
	
	// Un calculateur de chemin utilisant l'algorithme A*
	private AStar aStar;

	/**
	 * Définition de la table
	 */
	public Table() {
		// On instancie le calulateur de chemin
		aStar = new AStar(dimX / PAS_CALCUL, dimY / PAS_CALCUL);
		
		// On initialise les zones inaccessibles
		initZonesInaccessibles();
	}
	
	/**
	 * Calcule un chemin vers un point de la table. Toutes les coordonnées sont en mm, évidemment.
	 * @param nous Le point où l'on est
	 * @param cible Le point où l'on va
	 * @return Une liste de points à parcourir, commençant par le point
	 * où l'on est et finissant par le point à atteindre
	 */
	public List<Point> getCheminToPoint(Point nous, Point cible) {
		// On convertit les coordonnées en pas de la grille de calcul
		// Ajouter 0.5 avant de caster permet d'arrondir
		final int posXnous = (int) ((double) nous.x / PAS_CALCUL + 0.5);
		final int posYnous = (int) ((double) nous.y / PAS_CALCUL + 0.5);
		final int posXcible = (int) ((double) cible.x / PAS_CALCUL + 0.5);
		final int posYcible = (int) ((double) cible.y / PAS_CALCUL + 0.5);
		

		Stack<Point> pileChemin = aStar.getChemin(new Point(posXnous, posYnous), new Point(posXcible, posYcible));
		
		if(pileChemin != null) {
			List<Point> listeChemin = new ArrayList<Point>(pileChemin.size());
			for(Point p : LineSimplificator.getSimpleLines(pileChemin)) {
				listeChemin.add(0, new Point(p.x * PAS_CALCUL, p.y * PAS_CALCUL));
			}
			return listeChemin;
		} else {
			return null;
		}
		
	}
	
	/**
	 * Définit la position de l'adversaire, pour qu'on puisse l'éviter.
	 * On évitera un carré de dimension donnée, autour de la position donnée.
	 * @param p La position de l'adversaire. Si null, on considère qu'il n'y a plus d'adversaire
	 * @param dimension La largeur estimée du robot adverse, en millimètres.
	 * Ne pas oublier de prendre de la marge !
	 */
	public void setPositionAdversaire(Point p, int dimension) {
		// On reset les zones temporairement inaccessibles
		aStar.resetTemporaryAccessible();
		
		// Y a-t-il un adversaire dans les parages ?
		if(p != null) {
			// On convertit la position et la dimension en unités compréhensible par
			// le calculateur de chemin
			final int dimCarre = dimension / PAS_CALCUL + 1; // On prend volontairement une marge
			
			// Ajouter 0.5 avant de caster permet d'arrondir le résultat
			final int posX = (int) ((double) p.x / PAS_CALCUL + 0.5);
			final int posY = (int) ((double) p.y / PAS_CALCUL + 0.5);
			
			// On rend un carré inaccessible
			Area zoneAdversaire = new Carre(new Point(posX, posY), dimCarre);
			for(Point inaccessible : zoneAdversaire.getPoints()) {
				aStar.setTemporaryAccessible(inaccessible.x, inaccessible.y, false);
			}
		}
		
	}

	/**
	 * Initialise les zones inaccessibles
	 */
	private void initZonesInaccessibles() {
		// On récupère la liste des zones interdites
		List<Area> zonesInaccessibles = getZonesInterditesStatiques();
		
		// Pour chaque zone inaccessible
		for(Area a : zonesInaccessibles) {
			// Pour chaque point de la zone
			for(Point p : a.getPoints()) {
				// On indique au calculateur de chemin de ne pas y passer
				aStar.setDefinitivelyAccessible(p.x, p.y, false);
			}
		}
		
		// Il faut ensuite appeler cette méthode pour que l'A* fonctionne correctement
		aStar.updateVoisinageInfo();
	}


	/**
	 * Définition des zones statiques inaccessibles sur la table:
	 * bordures, éléments de jeu que l'on veut éviter, etc...
	 * Attention, prévoir de la marge en fonction de la taille du robot !
	 * Les tailles sont à exprimer en {@link #PAS_CALCUL}.
	 * 
	 * Cette méthode est à modifier chaque année suivant la disposition de la table.
	 * @return La liste des zones interdites statiques
	 */
	private List<Area> getZonesInterditesStatiques() {
		
		List<Area> listeZones = new ArrayList<Area>();
		
		// Le pas a été défini à 10mm = 1cm, donc on exprime les dimensions en cm.
		
		// Les bordures de la tables : ne devrait pas changer trop souvent
		final int margeBordures = 20; // 20 cm de marge
		listeZones.add(new Rectangle(0, 0, dimX / PAS_CALCUL, margeBordures));
		listeZones.add(new Rectangle(0, 0, margeBordures, dimY / PAS_CALCUL));
		listeZones.add(new Rectangle(0, dimY / PAS_CALCUL, dimX / PAS_CALCUL, dimY / PAS_CALCUL - margeBordures));
		listeZones.add(new Rectangle(dimX / PAS_CALCUL, 0, dimX / PAS_CALCUL - margeBordures, dimY / PAS_CALCUL));
		
		// Spécifique 2016
		
		// La dune: deux petites bordures en haut de la table, à 80cm des cotés
		// On prend 19cm de marge et pas 20cm, parce que la plus proche cabine est à 20cm
		final int posBordureDune = 80;
		final int longeurBordureDune = 20;
		final int largeurBordureDune = 2;
		final int margeBordureDune = 19;
		
		listeZones.add(new Rectangle(posBordureDune - margeBordureDune,
				0,
				posBordureDune + largeurBordureDune + margeBordureDune,
				longeurBordureDune + margeBordureDune));
		listeZones.add(new Rectangle(dimX / PAS_CALCUL - posBordureDune + margeBordureDune,
				0,
				dimX / PAS_CALCUL - posBordureDune - largeurBordureDune - margeBordureDune,
				longeurBordureDune + margeBordureDune));
		
		// La zone de construction, au centre
		// Une bordure dans la largeur de la table, au centre, de 120cm, à 75cm du haut:
		final int longeurBordureZoneConstruction = 120;
		final int positionBordureZoneConstruction = 75;
		
		listeZones.add(new Rectangle(
				(dimX / PAS_CALCUL - longeurBordureZoneConstruction) / 2 - margeBordures,
				positionBordureZoneConstruction - margeBordures,
				(dimX / PAS_CALCUL + longeurBordureZoneConstruction) / 2 + margeBordures,
				positionBordureZoneConstruction + margeBordures));
		
		// Le paravent, au milieu de la table, de 60cm de long
		// On prend 22 cm de marge parce qu'il est un peu large
		final int longueurParavent = 60;
		final int margeParavent = 22;
		listeZones.add(new Rectangle(
				(dimX / PAS_CALCUL / 2 ) - margeParavent,
				positionBordureZoneConstruction - margeBordures,
				(dimX / PAS_CALCUL / 2 ) + margeParavent,
				positionBordureZoneConstruction + longueurParavent + margeBordures));
		
		return listeZones;
	}
}
