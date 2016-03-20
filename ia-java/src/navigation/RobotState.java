package navigation;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by mickael on 14/05/15.
 */
public abstract class RobotState {
	private final int radius;
	private List<Area> forbiddenAreas = new ArrayList<>();

	protected RobotState(int radius) {
		this.radius = radius;
	}

	public List<Area> getForbiddenAreas() {
		return forbiddenAreas;
	}

	public int getRadius() {
		return radius;
	}
}
