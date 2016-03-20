package api.communication;

import java.io.Closeable;
import java.io.IOException;

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
/**
 * Interface de communication I2C
 * 
 * @author mickael
 */
public class I2C implements Closeable {
	private I2CBus bus;
	private I2CDevice device;

	/**
	 * Vérifie la validité d'une adresse (après l'avoir converti en 7 bits si
	 * nécessaire)
	 * 
	 * @param addr l'adresse à convertir
	 * @param min_addr l'adresse minimale valide (sur 7 bits)
	 * @param max_addr l'adresse maximale valide (sur 7 bits)
	 * @return l'adresse sur 7 bits
	 * @throws IllegalArgumentException
	 */
	public static int fixAddress(int addr, int min_addr, int max_addr) {
		if (addr >= (min_addr << 1) && addr <= (max_addr << 1) && (addr % 2) == 0) {
			return addr >> 1;
		} else if (addr == 0 || (addr >= min_addr && addr <= max_addr)) {
			return addr;
		} else {
			throw new IllegalArgumentException("Adresse non valide");
		}
	}

	/**
	 * Établit une connexion I2C à l'adresse donnée
	 * 
	 * @param dev numéro de l'adaptateur I2C (0, 1, ...)
	 * @param devAddr adresse du périphérique sur <strong>7 bits</strong> : le
	 *            bit de poids faible indiquant si il s'agit d'une lecture ou
	 *            d'une écriture ne doit pas être inclus (souvent les
	 *            documentations donnent une adresse sur 8 bits !)
	 * @throws IOException 
	 */
	public void open(int dev, int devAddr) throws IOException {
		bus = I2CFactory.getInstance(dev);
		device = bus.getDevice(devAddr);
	}

	/**
	 * Lit un octet
	 * 
	 * @return octet lu
	 * @throws IOException 
	 */
	public int readByte() throws IOException {
		return device.read();
	}

	/**
	 * Écrit un ou plusieurs octets
	 * 
	 * @param bytes les octets à écrire
	 * @throws IOException 
	 */
	public void writeBytes(byte... bytes) throws IOException {
		device.write(bytes, 0, bytes.length);
	}

	/**
	 * Ferme la connexion I2C
	 * @throws IOException 
	 */
	public void close() throws IOException {
		bus.close();
	}

	/**
	 * Écrit des octets dans un registre
	 * 
	 * @param regaddr adresse du registre
	 * @param vals octets à écrire
	 * @throws IOException 
	 */
	public void writeRegister(int regaddr, byte... vals) throws IOException {
		device.write(regaddr, vals, 0, vals.length);
	}

	/**
	 * Lit un registre de 16 bits (2 octets)
	 * 
	 * @param msb_addr adresse de l'octet de poids fort
	 * @param lsb_addr adresse de l'octet de poids faible
	 * @throws IOException 
	 */
	public int readRegister16(int msb_addr, int lsb_addr) throws IOException {
		return readRegister(lsb_addr) | (readRegister(msb_addr) << 8);
	}

	/**
	 * Lit un registre
	 * 
	 * @param addr adresse du registre
	 * @return octet lu
	 * @throws IOException 
	 */
	public int readRegister(int addr) throws IOException {
		writeBytes((byte) addr);
		return readByte();
	}
}
