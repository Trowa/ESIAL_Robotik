package api.controllers.gc;

import java.util.EventListener;

import api.controllers.gc.GameCubeRemote.AnalogButton;
import api.controllers.gc.GameCubeRemote.Button;

public interface GameCubeListener extends EventListener {
	public void dataUpdated();
	public void buttonStateChanged(Button button, boolean pressed);
	public void buttonAnalogValueChanged(AnalogButton button, int value);
}
