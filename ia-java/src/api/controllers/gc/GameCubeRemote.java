package api.controllers.gc;

import api.communication.Serial;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.util.ArrayList;

/**
 * Implémente la communication avec une manette de GameCube par l'intermédiaire
 * d'une Arduino reliée en I2C
 * 
 * @author mickael
 */
public class GameCubeRemote {
	private byte[] data = new byte[8];
	private byte[] previousData = new byte[8];
	private ArrayList<GameCubeListener> listeners = new ArrayList<GameCubeListener>();
	private Serial serial;

	public enum Button {
		START(0, 4),
		X(0, 2),
		Y(0, 3),
		A(0, 0),
		B(0, 1),
		L(1, 6),
		R(1, 5),
		Z(1, 4),
		D_UP(1, 3),
		D_DOWN(1, 2),
		D_RIGHT(1, 1),
		D_LEFT(1, 0);

		private int byteNumber;
		private int bitNumber;

		private Button(int byteNumber, int bitNumber) {
			this.byteNumber = byteNumber;
			this.bitNumber = bitNumber;
		}

		public boolean getPressed(byte[] data) {
			return (data[byteNumber] & (1 << bitNumber)) != 0;
		}
	}

	public enum AnalogButton {
		STICK_X(2),
		STICK_Y(3),
		CSTICK_X(4),
		CSTICK_Y(5),
		L(6),
		R(7);

		private int byteNumber;

		private AnalogButton(int byteNumber) {
			this.byteNumber = byteNumber;
		}

		public int getAnalogValue(byte[] data) {
			return data[byteNumber] & 0xFF;
		}
	}

	public GameCubeRemote() throws IOException, NoSuchPortException, PortInUseException, UnsupportedCommOperationException {
		serial = new Serial("/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A700fioY-if00-port0", 115200);
		serial.write(0);
		try {
			Thread.sleep(1500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Met à jour l'état des boutons.
	 */
	public void update() {
		byte[] newdata;

		try {
			serial.write(0);
			newdata = serial.read(8);
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}

		if (newdata.length != 8) {
			System.out.println("Seulement " + data.length + " octets reçus, on ignore…");
			return;
		}

		if (newdata[0] == 0 || newdata[1] == 0 || newdata[2] == 0 || newdata[3] == 0 || newdata[4] == 0 || newdata[5] == 0) {
			System.out.println("Valeur erronnée (0), on ignore");
			return;
		}

		System.arraycopy(data, 0, previousData, 0, data.length);
		System.arraycopy(newdata, 0, data, 0, newdata.length);

		//System.out.println(Arrays.toString(data));

		fireDataUpdated();

		for (Button button : Button.values()) {
			if (hasChanged(button)) {
				fireButtonStateChanged(button, isPressed(button));
			}
		}

		for (AnalogButton button : AnalogButton.values()) {
			if (hasValueChanged(button)) {
				fireButtonAnalogValueChanged(button, getValue(button));
			}
		}
	}

	/**
	 * Renvoie <code>true</code> si le bouton est appuyé.
	 * 
	 * @param button le bouton à tester
	 */
	public boolean isPressed(Button button) {
		return button.getPressed(data);
	}

	/**
	 * Renvoie la valeur analogique d'un bouton/stick.
	 * 
	 * @param analogButton le bouton/stick
	 * @return valeur analogique entre -128 et 127
	 */
	public int getValue(AnalogButton analogButton) {
		return analogButton.getAnalogValue(data);
	}

	/**
	 * Renvoie <code>true</code> si la valeur du bouton a changé depuis la
	 * dernière mise à jour.
	 */
	public boolean hasChanged(Button button) {
		return button.getPressed(data) != button.getPressed(previousData);
	}

	/**
	 * Renvoie <code>true</code> si la valeur analogique du bouton a changé
	 * depuis la dernière mise à jour.
	 */
	public boolean hasValueChanged(AnalogButton button) {
		return button.getAnalogValue(data) != button.getAnalogValue(previousData);
	}

	public void addListener(GameCubeListener listener) {
		listeners.add(listener);
	}

	public void removeListener(GameCubeListener listener) {
		listeners.remove(listener);
	}

	private void fireButtonStateChanged(Button button, boolean pressed) {
		for (GameCubeListener listener : listeners) {
			listener.buttonStateChanged(button, pressed);
		}
	}

	private void fireButtonAnalogValueChanged(AnalogButton button, int value) {
		for (GameCubeListener listener : listeners) {
			listener.buttonAnalogValueChanged(button, value);
		}
	}

	private void fireDataUpdated() {
		for (GameCubeListener listener : listeners) {
			listener.dataUpdated();
		}
	}
}
