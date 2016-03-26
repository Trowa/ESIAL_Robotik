package api.hardware;

/**
 * Une interface commune pour gérer des GPIO
 * @author jb
 *
 */
public interface GPIO {
	
	/**
	 * Lit la valeur d'entrée du GPIO
	 * @return La valeur du GPIO: true ou false
	 */
	public boolean getValue();
	
	/**
	 * Définit la valeur de sortie du GPIO
	 * @param value La valeur: true ou false
	 */
	public void setValue(boolean value);

}
