package api.lcd03;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import api.communication.I2C;

public class Keypad {
	private static final byte DOUBLE_KEYPAD_SCAN_RATE = 28;
	private static final byte SIMPLE_KEYPAD_SCAN_RATE = 29;

	private static final int REG_KEYPAD_LOW = 1;
	private static final int REG_KEYPAD_HIGH = 2;

	private static final int[] key_values = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };
	private static final char[] key_chars = { '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', '0', 'B' };

	private List<KeypadEventListener> listeners = new ArrayList<KeypadEventListener>();
	private I2C i2c;

	private volatile Thread poll_thread = null;

	Keypad(I2C i2c) {
		this.i2c = i2c;
	}

	public void setDoubleKeypadScanRate(boolean enable) throws IOException {
		i2c.writeRegister(0, enable ? DOUBLE_KEYPAD_SCAN_RATE : SIMPLE_KEYPAD_SCAN_RATE);
	}

	private void fireKeydownEvent(char key) {
		synchronized (listeners) {
			for (KeypadEventListener listener : listeners) {
				listener.keyDown(key);
			}
		}
	}

	private void fireKeyupEvent(char key) {
		synchronized (listeners) {
			for (KeypadEventListener listener : listeners) {
				listener.keyUp(key);
			}
		}
	}

	public void addKeypadEventListener(KeypadEventListener listener) {
		synchronized (listeners) {
			listeners.add(listener);
		}
	}

	public char pollKey() throws IOException {

		int keypad_state = i2c.readRegister16(REG_KEYPAD_HIGH, REG_KEYPAD_LOW);
		int index = Arrays.binarySearch(key_values, keypad_state);

		if (index < 0) {
			return '\0';
		}

		return key_chars[index];
	}

	public void startPolling() {
		poll_thread = new Thread(new Runnable() {
			@Override
			public void run() {
				char prev_key = '\0';
				Thread thisThread = Thread.currentThread();

				try {
					while (thisThread == poll_thread) {
						char pressed = pollKey();

						if (pressed != prev_key) {
							if (prev_key != '\0') {
								fireKeyupEvent(prev_key);
							}
							if (pressed != '\0') {
								fireKeydownEvent(pressed);
							}
						}

						prev_key = pressed;
						Thread.sleep(50);
					}
				} catch (InterruptedException e) {
				} catch (IOException e) {
				}
			}
		});

		poll_thread.setDaemon(true);
		poll_thread.start();
	}

	public void stopPolling() {
		if (poll_thread != null) {
			poll_thread.interrupt();
			poll_thread = null;
		}
	}
}
