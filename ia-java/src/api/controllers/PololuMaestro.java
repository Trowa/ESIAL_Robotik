package api.controllers;

import api.communication.Serial;

import java.io.IOException;

public class PololuMaestro {
	private static final int CMD_SET_TARGET = 0x84;
	private static final int CMD_SET_SPEED = 0x87;
	private static final int CMD_SET_ACCELERATION = 0x89;

	private Serial serial;

	public PololuMaestro(Serial serial) {
		this.serial = serial;
	}

	public void init() throws IOException {
		//serial.sendBreak(10); // reset
		serial.write(0xAA); // autodetect baud rate
	}

	public void setTarget(int channel, double usecs) throws IOException {
		int val = (int) (usecs * 4);

		serial.write(CMD_SET_TARGET, channel, val & 0x7F, (val >> 7) & 0x7F);
	}

	public void setTargetAngle(int channel, int angle) throws IOException {
		setTarget(channel, 1500 + 500 * angle / 90);
	}


	public void setTargetMs(int channel, int ms) throws IOException {
		byte[] buffer = new byte[6];
		buffer[0] = (byte) 0xAA;
		buffer[1] = 12;
		buffer[2] = (byte) 0x04;
		buffer[3] = (byte) channel;
		buffer[4] = (byte) ((4 * ms) % 128);
		buffer[5] = (byte) ((4 * ms) / 128);

		serial.write(buffer);
	}

	public static void main(String[] args) throws Exception {
		Serial serial = new Serial("/dev/ttyACM0", 115200);

		PololuMaestro maestro = new PololuMaestro(serial);
		maestro.init();

		for (String arg : args) {
			int ang = Integer.parseInt(arg);

			System.out.println(ang + "° ...");
			maestro.setTargetAngle(0, ang);
			Thread.sleep(1000);
		}

		System.out.println("0° ...");
		maestro.setTargetAngle(0, 0);
	}
}
