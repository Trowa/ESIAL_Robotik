package navigation;

import navigation.Point.DIRECTION;

/**
 * Utilis� pour v�rifier qu'une suite de points fait partie d'une diagonale ou d'une ligne droite
 * Une diagonale c'est un ensemble de tron�ons mis bout � bout qui sont d�cal�s les uns par rapport aux autre
 * d'un cran constant
 * @author Jean-Baptiste Gu�ry
 *
 */
public class CycleDetector {

	private DIRECTION troncon;
	private DIRECTION decalage;

	
	// Intervalle entre 2 d�calages (longueur d'un tron�on)
	private int interval;
	// Conserve l'interval minimum entre 2 d�calages (calcul� au d�but quand on ne connait pas encore eu deux d�calages)
	private int minInterval;
	
	// Conserve la longueur courante du tron�on
	private int currentInterval;
	
	// Utilis� au d�but pour d�termin� la direction du tron�on et du d�calage
	private DIRECTION dir1;
	private DIRECTION dir2;
	
	public CycleDetector(){
		reset();
	}
	
	/**
	 * Remet � z�ro le CycleDetector pour d�marrer une nouvelle s�quence de validation
	 */
	public void reset(){
		currentInterval = 0;
		minInterval = 0;
		interval = -1;
		dir1 = null;
		dir2 = null;
		troncon = null;
		decalage = null;
	}
	
	/**
	 * Ajoute une direction au d�tecteur
	 * Une fois que cette m�thode a retourn� false son comportement n'est plus assur�
	 * Il faut appeler reset() pour d�marrer une nouvelle s�quence de d�tection
	 * @param d
	 * @return vrai si la direction ajout�e est coh�rente
	 */
	public boolean add(DIRECTION d){
		// Si on a pas encore d�termin� la direction du tronc�on de la diagonale
		if(troncon == null){
			if(dir1 == null && dir2 == null){
				dir1 = d;
				return true;
			}else if(dir2 == null){
				if(dir1 == d){
					troncon = d;
					currentInterval = 1;
					minInterval = 1;
					return true;
				}else{
					dir2 = d;
					return true;
				}
			}else{
				if(dir1 == d){
					currentInterval = 1;
					minInterval = 0;
					troncon = d;
					decalage = dir2;
					return true;
				}else if(dir2 == d){
					currentInterval = 2;
					minInterval = 0;
					troncon = d;
					decalage = dir1;
					return true;
				}else
					return false;
			}
		}
		// Si la direction correspond � la direction du tron�on 
		else if(d == troncon){
			currentInterval++;
			// Si l'interval est d�j� fix�
			if(decalage == null){
				minInterval = currentInterval;
				return true;
			}else if(interval > -1){
				return currentInterval <= interval;
			}else{
				return true;
			}
		// Si on avait pas encore la direction du d�calage
		}else if(decalage == null){
			decalage = d;
			minInterval = currentInterval;
			currentInterval = 0;
			return true;
		}
		// Si c'est la direction du d�calage
		else if(decalage == d){
			// Si la longueur du tron�on n'avait pas encore �t� d�termin�
			if(interval == -1){
				// Si on avait constat� lors de la premi�re occurence du d�calage qu'on avait re�u un plus gros tron�on que l'actuel c'est incoh�rent
				if(currentInterval <= minInterval)
					return false;
				else{
					interval = currentInterval;
					currentInterval = 0;
					return true;
				}
			}else{
				if(currentInterval == interval){
					currentInterval = 0;
					return true;
				}else{
					return false;
				}
			}
		}
		// Si la direction ne correspond ni � un tron�on ni au d�calage
		else
			return false;	
		
	}
	
	/**
	 * R�cup�re la direction du d�calage
	 * @return
	 */
	public DIRECTION getDecalage(){
		return decalage;
	}
	
	/**
	 * R�cup�re la direction du tron�on
	 * @return
	 */
	public DIRECTION getTroncon(){
		return troncon;
	}
	
	/**
	 * Pour du d�bugs
	 * @return
	 */
	public String dump(){
		return "interval: "+interval+" / minInterval : "+minInterval+" currentInterval : "+currentInterval+" / main : "+troncon+" / shift : "+decalage;
	}
}
