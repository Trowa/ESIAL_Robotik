package api.lcd03;

import java.io.IOException;

import api.communication.I2C;

/**
 * Gère un module LCD03 (Afficheur LCD et clavier matriciel)
 * <p>
 * <dl>
 * <dt>Lien vers la documentation :</dt>
 * <dd><a href="http://www.robot-electronics.co.uk/htm/Lcd03tech.htm">
 * http://www.robot-electronics.co.uk/htm/Lcd03tech.htm</a></dd>
 * </dl>
 * 
 * @author mickael
 * 
 */
public class LCDController {
	public static final byte CHANGE_ADDRESS = 25;

	private I2C i2c = new I2C();
	private LCD lcd;
	private Keypad keypad;

	private int i2caddr;
	private int i2cdev;
	private int version;

	I2C getI2C() {
		return i2c;
	}

	/**
	 * Retourne l'adresse 7 bits de ce périphérique
	 */
	public int getAddress() {
		return i2caddr;
	}

	public int getVersion() {
		return version;
	}

	public LCD getLCD() {
		return lcd;
	}

	public Keypad getKeypad() {
		return keypad;
	}

	public LCDController(int i2cdev, int i2caddr) throws IOException {
		this.i2cdev = i2cdev;
		this.i2caddr = i2caddr;

		i2c.open(i2cdev, i2caddr >> 1);
		i2c.writeBytes((byte) 3);
		version = i2c.readByte();

		this.lcd = new LCD(i2c);
		this.keypad = new Keypad(i2c);
	}

	public LCDController(int i2caddr) throws IOException {
		this(i2caddr, 0xC6);
	}

	/**
	 * Vérifie la validité de l'adresse (après l'avoir converti en 7 bits si
	 * nécessaire)
	 * 
	 * @param addr l'adresse à convertir
	 * @return l'adresse sur 7 bits
	 * @throws IllegalArgumentException
	 */
	public static int fixAddress(int addr) {
		return I2C.fixAddress(addr, 0x63, 0x67);
	}

	public void changeAddress(int newaddr) throws IOException {
		newaddr = fixAddress(newaddr);

		i2c.writeRegister(0, CHANGE_ADDRESS, (byte) 0xA0, (byte) 0xAA, (byte) 0xA5, (byte) (newaddr << 1));
		i2c.close();
		i2c.open(i2cdev, newaddr);
	}
}
