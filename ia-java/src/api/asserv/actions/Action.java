package api.asserv.actions;

/**
 * Created by mickael on 13/05/15.
 */
public class Action {
	private final String command;

	protected Action(String command) {
		this.command = command;
	}

	public String getSerialCommand() {
		return command;
	}
}
