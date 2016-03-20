package api.controllers.qik;

import java.io.IOException;

import api.communication.Serial;

public class Qik2s12v10 extends Qik2s9v1 {
	public static final byte QIK_CONFIG_PWM_PARAMETER = 1;

	private static final int QIK_2S12V10_MOTOR_M0_BRAKE = 0x86;
	private static final int QIK_2S12V10_MOTOR_M1_BRAKE = 0x87;
	private static final int QIK_2S12V10_GET_MOTOR_M0_CURRENT = 0x90;
	private static final int QIK_2S12V10_GET_MOTOR_M1_CURRENT = 0x91;
	private static final int QIK_2S12V10_GET_MOTOR_M0_SPEED = 0x92;
	private static final int QIK_2S12V10_GET_MOTOR_M1_SPEED = 0x93;

	public static final byte QIK_CONFIG_MOTOR_M0_ACCELERATION = 4;
	public static final byte QIK_CONFIG_MOTOR_M1_ACCELERATION = 5;
	public static final byte QIK_CONFIG_MOTOR_M0_BRAKE_DURATION = 6;
	public static final byte QIK_CONFIG_MOTOR_M1_BRAKE_DURATION = 7;
	public static final byte QIK_CONFIG_MOTOR_M0_CURRENT_LIMIT_DIV_2 = 8;
	public static final byte QIK_CONFIG_MOTOR_M1_CURRENT_LIMIT_DIV_2 = 9;
	public static final byte QIK_CONFIG_MOTOR_M0_CURRENT_LIMIT_RESPONSE = 10;
	public static final byte QIK_CONFIG_MOTOR_M1_CURRENT_LIMIT_RESPONSE = 11;

	public Qik2s12v10(Serial serial) throws IOException {
		super(serial);
	}

	void setM0Brake(int brake) throws IOException {
		if (brake > 127) {
			brake = 127;
		}

		serial.write(QIK_2S12V10_MOTOR_M0_BRAKE, brake);
	}

	void setM1Brake(int brake) throws IOException {
		if (brake > 127) {
			brake = 127;
		}

		serial.write(QIK_2S12V10_MOTOR_M1_BRAKE, brake);
	}

	void setBrakes(int m0Brake, int m1Brake) throws IOException {
		setM0Brake(m0Brake);
		setM1Brake(m1Brake);
	}

	int getM0Current() throws IOException {
		serial.write(QIK_2S12V10_GET_MOTOR_M0_CURRENT);

		return serial.read();
	}

	int getM1Current() throws IOException {
		serial.write(QIK_2S12V10_GET_MOTOR_M1_CURRENT);

		return serial.read();
	}

	int getM0CurrentMilliamps() throws IOException {
		return getM0Current() * 150;
	}

	int getM1CurrentMilliamps() throws IOException {
		return getM1Current() * 150;
	}

	int getM0Speed() throws IOException {
		serial.write(QIK_2S12V10_GET_MOTOR_M0_SPEED);

		return serial.read();
	}

	int getM1Speed() throws IOException {
		serial.write(QIK_2S12V10_GET_MOTOR_M1_SPEED);

		return serial.read();
	}
}
