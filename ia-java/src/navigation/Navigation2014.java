package navigation;

public class Navigation2014 extends Navigation {

	@Override
	protected void initZonesInterdites() {
		// On interdit les bords de la table pour que le robot ne se coince pas sur une bordure comme un con
		for (int i = 2; i <= 28; i++) {
			this.zonesInterdites.add(new Point(i, 2));
			this.zonesInterdites.add(new Point(i, 18));
		}
		for (int i = 2; i <= 18; i++) {
			this.zonesInterdites.add(new Point(2, i));
			this.zonesInterdites.add(new Point(28, i));
		}
		
		// On bloque les quarts de cercle de dépose dans les coins (on bloque un carré, c'est moins chiant)
		for (int i = 0; i <= 5; i++) {
			this.zonesInterdites.add(new Point(i, 5));
			this.zonesInterdites.add(new Point(5, i));
			this.zonesInterdites.add(new Point(25+i, 5));
			this.zonesInterdites.add(new Point(25, i));
		}
		
		// On bloque le cercle central (encore un carré)
		for (int i = 0; i <= 8; i++) {
			this.zonesInterdites.add(new Point(11+i, 7));
			this.zonesInterdites.add(new Point(11+i, 14));
		}
		for (int i = 0; i <= 7; i++) {
			this.zonesInterdites.add(new Point(11, 7+i));
			this.zonesInterdites.add(new Point(19, 7+i));
		}
		
		// On bloque les dépôts de fruits
		for (int i = 0; i <= 11; i++) {
			this.zonesInterdites.add(new Point(2+i, 15));
			this.zonesInterdites.add(new Point(17+i, 15));
		}
		for (int i = 0; i <= 5; i++) {
			this.zonesInterdites.add(new Point(2, 20-i));
			this.zonesInterdites.add(new Point(13, 20-i));
			this.zonesInterdites.add(new Point(17, 20-i));
			this.zonesInterdites.add(new Point(28, 20-i));
		}
		
		// On remplit DStar
		for (Point p : this.zonesInterdites) {
			this.dStar.updateCell(p.getX(), p.getY(), -1);
		}
	}

	@Override
	protected void setZonesInterditesMobiles(int x, int y) {
		for (int i = 0; i <= 6; i++) {
			this.zonesInterditesMobiles.add(new Point(x+3, y+i));
			this.zonesInterditesMobiles.add(new Point(x-3, y+i));
			this.zonesInterditesMobiles.add(new Point(x-3-i, y));
			this.zonesInterditesMobiles.add(new Point(x-3+i, y+6));
		}
	}
	
	@Override
	public Point[] getExtremeZoneInterdite(Point adversaire) {
		Point[] res = new Point[2];
		res[0] = new Point(adversaire.getX()-4, adversaire.getY()-4);
		res[1] = new Point(adversaire.getX()+4, adversaire.getY()+4);
		return res;
	}

	@Override
	protected void initListeObjectifs() {
		// TODO à remplir
		this.objectifs.add(new Point(25, 10));
	}

	@Override
	protected void init() {
		this.pas = 100;
	}

}
