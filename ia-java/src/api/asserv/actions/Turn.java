package api.asserv.actions;

/**
 * Created by mickael on 13/05/15.
 */
public class Turn extends Action {
	public Turn(int angle) {
		super("t" + angle);
	}
}
