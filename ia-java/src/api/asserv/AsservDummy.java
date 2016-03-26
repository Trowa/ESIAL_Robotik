package api.asserv;

/**
 * Une asserv qui ne sert à rien
 * @author jb
 *
 */

public class AsservDummy extends Asserv {
	
	private boolean resetTriggered = false;
	
	public AsservDummy() {
		System.out.println("[AsservDummy] Initialisation...");
		resetTriggered = false;
		asservInit();
	}

	@Override
	protected void sendCommand(String commande) {
		System.out.println("[AsservDummy] Envoi de la commande: " + commande);
		
		if(commande.equals("R")) {
			// C'est une commande de reset, il faut envoyer "ok"
			resetTriggered = true;
		}
	}

	@Override
	protected String readAsservOutput() {
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("[AsservDummy] Lecture d'une ligne...");
		if(resetTriggered) {
			resetTriggered = false;
			return "Initialitation ok";
		} else {
			return "#x0.0y0.0a0.0d1\n";
		}
	}

}
