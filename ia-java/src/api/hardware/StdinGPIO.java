package api.hardware;

import java.io.IOException;

/**
 * Un GPIO qui n'en est pas un : il demande sa valeur sur l'entrée standard
 * quand on le lit et la revoie sur la sortie standard quand on l'écrit.
 * A utiliser pour du débug
 * @author jb
 *
 */
public class StdinGPIO implements GPIO {
	
	/**
	 * Le nom du GPIO, pour avoir un prompt un tantinet explicite
	 */
	private String name;
	
	public StdinGPIO(String name) {
		this.name = name;
	}

	@Override
	public boolean getValue() {
		// On demande la valeur à l'utilisateur
		
		while(true) {
			System.out.println("GPIO " + name + " : Valeur (0/1)? >");

			int charRead;
			try {
				do {
					charRead = System.in.read();
				} while(charRead == '\n');
				
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}

			if(charRead == '0') {
				return false;
			} else if(charRead == '1') {
				return true;
			}
			
		}
	}

	@Override
	public void setValue(boolean value) {
		System.out.println("GPIO " + name + " : Valeur mise à " + (value ? 1 : 0));
	}

}
