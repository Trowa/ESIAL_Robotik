package navigation;

/**
 * Représentation un point via 2 coordonnées
 * @author Jean-Baptiste Guéry
 *
 */
public class Point {

	public int x;
	public int y;
	private double cap;
	
	/**
	 * Représente une direction dans laquelle il faut aller pour aller d'un point à un autre
	 * On suppose l'origine en haut à gauche d'un écran
	 * @author Jean-Baptiste Guéry
	 *
	 */
	public enum DIRECTION{
		
		NO(false), N(true), NE(false), E(true), SE(false), S(true), SO(false), O(true), NULL(false);
		
		
		public final boolean isPureCardinalPoint;
		
		DIRECTION(boolean isCardinalPoint){
			this.isPureCardinalPoint = isCardinalPoint;
		}
	}
	
	public Point(int x, int y){
		this.x = x;
		this.y = y;
	}
	
	public void setX(int x) {
		this.x = x;
	}
	
	public void setY(int y) {
		this.y = y;
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}
	
	public double getCap() {
		return cap;
	}

	public void setCap(double cap) {
		this.cap = cap;
	}

	public double getCapDegree() {
		return Math.toDegrees(cap);
	}

	/**
	 * Indique si un point est voisin directe d'un autre
	 * Si les points sont supperpos�s ils ne sont pas consid�r�s comme voisins
	 * @param p
	 * @return
	 */
	public boolean isNear(Point p){
		return x-1 == p.x || x+1 == p.x || y-1 == p.y || y+1 == p.y;
	}
	
	/**
	 * Retourne la direction qu'il faut suivre pour aller de ce point au suivant
	 * @param p
	 * @return
	 */
	public DIRECTION getDirectionToGoTo(Point p){
		if(this.x == p.x){
			if(this.y == p.y)
				return DIRECTION.NULL;
			else if(this.y < p.y)
				return DIRECTION.S;
			else
				return DIRECTION.N;
		}else if(this.x < p.x){
			if(this.y == p.y)
				return DIRECTION.E;
			else if(this.y > p.y)
				return DIRECTION.SE;
			else
				return DIRECTION.NE;			
		}else{
			if(this.y == p.y)
				return DIRECTION.O;
			else if(this.y > p.y)
				return DIRECTION.SO;
			else
				return DIRECTION.NO;
		}
	}
	
	public String toString(){
		return "("+x+";"+y+"; " + getCapDegree() + "°)";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + x;
		result = prime * result + y;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Point other = (Point) obj;
		if (x != other.x)
			return false;
		if (y != other.y)
			return false;
		return true;
	}
}
