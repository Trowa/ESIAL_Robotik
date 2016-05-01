package api.asserv.actions;

/**
 * Une action qui attend un certain temps, puis annule toutes les commandes en cours
 * @author jb
 *
 */
public class WaitAndCancel extends Action {

	private int timeoutInMs;
	
	public WaitAndCancel(int timeoutInMs) {
		// La commande: un Halt suivi d'un Reset
		super("h\nr");
		
		this.timeoutInMs = timeoutInMs;
	}
	
	@Override
	public String getSerialCommand() {
		// Avant de retourner la commande, on attend un peu
		try {
			Thread.sleep(timeoutInMs);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		return super.getSerialCommand();
	}

}
