package api.controllers.gc;

import api.controllers.gc.GameCubeRemote.AnalogButton;
import api.controllers.gc.GameCubeRemote.Button;

public class GameCubeAdapter implements GameCubeListener {
	@Override
	public void dataUpdated() {	}

	@Override
	public void buttonStateChanged(Button button, boolean pressed) { }

	@Override
	public void buttonAnalogValueChanged(AnalogButton button, int value) { }
}
