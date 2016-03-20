package api.asserv.actions;

/**
 * Created by mickael on 13/05/15.
 */
public class Goto extends Action {
	public Goto(int x, int y) {
		super("g" + x + "#" + y);
	}
}
