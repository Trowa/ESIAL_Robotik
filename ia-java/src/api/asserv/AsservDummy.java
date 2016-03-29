package api.asserv;

/**
 * Une asserv qui ne sert à rien
 * @author jb
 *
 */

public class AsservDummy extends Asserv {
	
	private boolean resetTriggered = false;
	private boolean commandSent = false;
	
	public AsservDummy() {
		System.out.println("[AsservDummy] Initialisation...");
		resetTriggered = false;
		commandSent = false;
		asservInit();
	}

	@Override
	protected void sendCommand(String commande) {
		System.out.print("[AsservDummy] Envoi de la commande: " + commande);
		if(!commande.endsWith("\n")) {
			System.out.print("\n");
		}
		
		if(commande.equals("R")) {
			// C'est une commande de reset, il faut envoyer "ok"
			resetTriggered = true;
		}
		commandSent = true;
	}

	@Override
	protected String readAsservOutput() {
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		if(resetTriggered) {
			resetTriggered = false;
			return "Initialitation ok";
		} else {
			String toSend = "#x0.0y0.0a0.0d";
			if(commandSent == true) {
				toSend += "0\n";
				commandSent = false;
			} else {
				toSend += "2\n";
			}
			
			return toSend;
		}
	}

}
