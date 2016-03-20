package api.asserv.actions;

/**
 * Created by mickael on 13/05/15.
 */
public class Go extends Action {
	public Go(int dist) {
		super("v" + dist);
	}
}
