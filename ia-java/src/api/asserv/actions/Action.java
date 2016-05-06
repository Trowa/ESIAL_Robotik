package api.asserv.actions;

/**
 * Created by mickael on 13/05/15.
 */
public class Action {
	private final String command;
	private boolean blockingCommand;

	/**
	 * Par defaut, une commande est blaquante
	 * @param command
     */
	protected Action(String command) {
		this.command = command;
		this.blockingCommand = true;
	}

	protected Action(String command, boolean blockingCommand){
		this.command = command;
		this.blockingCommand = blockingCommand;
	}

	public String getSerialCommand() {
		System.out.println("getSerialCommand " + this.command + " : " + System.currentTimeMillis());
		return command;
	}

	public boolean isBlockingCommand() {
		return blockingCommand;
	}

	public void setBlockingCommand(boolean blockingCommand) {
		this.blockingCommand = blockingCommand;
	}
}
