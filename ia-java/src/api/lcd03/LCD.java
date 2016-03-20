package api.lcd03;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import api.communication.I2C;

public class LCD {
	private static final byte REG_CMD = 0;

	public static final byte CURSOR_HOME = 1;
	public static final byte SET_CURSOR = 2;
	public static final byte SET_CURSOR_2 = 3;
	public static final byte HIDE_CURSOR = 4;
	public static final byte UNDERLINE_CURSOR = 5;
	public static final byte BLINK_CURSOR = 6;
	public static final byte BACKSPACE = 8;
	public static final byte VTAB = 11;
	public static final byte CLEAR_SCREEN = 12;
	public static final byte CLEAR_COLUMN = 17;
	public static final byte TAB_SET = 18;
	public static final byte BACKLIGHT_ON = 19;
	public static final byte BACKLIGHT_OFF = 20;
	public static final byte DISABLE_STARTUP_MSG = 21;
	public static final byte ENABLE_STARTUP_MSG = 22;
	public static final byte CUSTOM_CHAR = 27;

	private I2C i2c;

	LCD(I2C i2c) {
		this.i2c = i2c;
	}

	public void cursorHome() throws IOException {
		i2c.writeRegister(REG_CMD, CURSOR_HOME);
	}

	public void cursorUp() throws IOException {
		i2c.writeRegister(REG_CMD, VTAB);
	}

	public void cursorDown() throws IOException {
		write("\n");
	}

	public void tab() throws IOException {
		write("\t");
	}

	public void setCursor(int pos) throws IOException {
		i2c.writeRegister(REG_CMD, SET_CURSOR, (byte) pos);
	}

	public void setCursor(int lig, int col) throws IOException {
		i2c.writeRegister(REG_CMD, SET_CURSOR_2, (byte) lig, (byte) col);
	}

	public void hideCursor() throws IOException {
		i2c.writeRegister(REG_CMD, HIDE_CURSOR);
	}

	public void underlineCursor() throws IOException {
		i2c.writeRegister(REG_CMD, UNDERLINE_CURSOR);
	}

	public void blinkCursor() throws IOException {
		i2c.writeRegister(REG_CMD, BLINK_CURSOR);
	}

	public void backspace() throws IOException {
		i2c.writeRegister(REG_CMD, BACKSPACE);
	}

	public void clearScreen() throws IOException {
		i2c.writeRegister(REG_CMD, CLEAR_SCREEN);
	}

	public void clearColumn() throws IOException {
		i2c.writeRegister(REG_CMD, CLEAR_COLUMN);
	}

	public void tabSet(int length) throws IOException {
		i2c.writeRegister(REG_CMD, TAB_SET, (byte) length);
	}

	public void setBacklight(boolean on) throws IOException {
		i2c.writeRegister(REG_CMD, on ? BACKLIGHT_ON : BACKLIGHT_OFF);
	}

	public void setStartupMessage(boolean on) throws IOException {
		i2c.writeRegister(REG_CMD, on ? ENABLE_STARTUP_MSG : DISABLE_STARTUP_MSG);
	}

	public void createCustomChar(int num, byte[] rows) throws IOException {
		if (rows.length != 8) {
			throw new IllegalArgumentException("rows must contains 8 elements");
		}

		byte[] arr = new byte[10];
		arr[0] = CUSTOM_CHAR;
		arr[1] = (byte) num;
		System.arraycopy(rows, 0, arr, 2, rows.length);

		i2c.writeRegister(REG_CMD, arr);
	}

	public void write(byte... vals) throws IOException {
		i2c.writeRegister(REG_CMD, vals);
	}

	public void write(String str) throws IOException {
		try {
			write(str.getBytes("US-ASCII"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
	}
}
