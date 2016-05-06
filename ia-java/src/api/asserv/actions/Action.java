package api.asserv.actions;

import java.text.SimpleDateFormat;
import java.util.Date;

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
		SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
		Date date = new Date(System.currentTimeMillis());
		System.out.println("getSerialCommand " + this.command + " : " + format.format(date));
		return command;
	}

	public boolean isBlockingCommand() {
		return blockingCommand;
	}

	public void setBlockingCommand(boolean blockingCommand) {
		this.blockingCommand = blockingCommand;
	}
}
