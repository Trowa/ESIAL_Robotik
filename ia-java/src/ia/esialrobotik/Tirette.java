package ia.esialrobotik;


public class Tirette {

	/*
	 * JBH TODO
	 * Gérer directement les GPIOs de la raspberry pi
	 */
	
	public Tirette(/* GPIO gpio */) {
		// Initialiser un GPIO (prendre en paramètre un objet GPIO ?)
	}

	public boolean isPulled() {
		return /* Code pour lire le GPIO */ true;
	}

	public void wait(boolean pulled) {
		while (isPulled() != pulled) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
