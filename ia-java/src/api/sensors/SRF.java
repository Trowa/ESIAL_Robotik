package api.sensors;

import java.io.IOException;
import java.util.ArrayList;

import api.communication.I2C;

/**
 * Gère le capteur d'ultrasons SRF10.
 * <p>
 * Le SRF08 est également géré (protocole identique) mais les commandes
 * spécifiques ne sont pas implémentées (de toute manière elles nous sont
 * inutiles...)
 * <p>
 * <dl>
 * <dt>Documentation du SRF08 :</dt>
 * <dd><a href="http://www.robot-electronics.co.uk/htm/srf08tech.shtml">
 * http://www.robot-electronics.co.uk/htm/srf08tech.shtml</a></dd>
 * <dt>Documentation du SRF10 :</dt>
 * <dd><a href="http://www.robot-electronics.co.uk/htm/srf10tech.htm">
 * http://www.robot-electronics.co.uk/htm/srf10tech.htm</a></dd>
 * </dl>
 * 
 * @author mickael
 */
public class SRF {
	private static final byte REG_COMMAND = 0;
	private static final byte REG_MAXGAIN = 1;
	private static final byte REG_RANGE_HIGH = 2;
	private static final byte REG_RANGE_LOW = 3;

	private static final byte RANGE_INCHES = 0x50;
	private static final byte RANGE_CENTIMETERS = 0x51;
	private static final byte RANGE_MICROSECONDS = 0x52;

	private static final byte CHG_ADDR1 = (byte) 0xA0;
	private static final byte CHG_ADDR2 = (byte) 0xAA;
	private static final byte CHG_ADDR3 = (byte) 0xA5;

	private I2C i2c = new I2C();
	private int i2cdev;
	private int i2caddr;

	/**
	 * Recherche un capteur sans connaître son adresse.
	 * 
	 * @param i2cdev numéro de l'adaptateur I2C (0, 1, ...)
	 * @return le capteur trouvé, ou <code>null</code>
	 */
	public static SRF[] scan(int i2cdev) {
		ArrayList<SRF> found = new ArrayList<SRF>();

		for (int addr = 0x70; addr < 0x80; addr++) {
			try {
				SRF srf = new SRF(i2cdev, addr);

				if (srf.getVersion() > 0) {
					found.add(srf);
				}
			} catch (Exception ex) {
				continue;
			}
		}

		return found.toArray(new SRF[0]);
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
		return I2C.fixAddress(addr, 0x70, 0x7F);
	}

	/**
	 * Crée un nouveau capteur
	 * 
	 * @param i2cdev numéro de l'adapateur I2C (0, 1, ...)
	 * @param i2caddr adresse sur 7 ou 8 bits (entre 0xE0 et 0xFE ou entre 0x70
	 *            et 0x7F)
	 * @throws IOException 
	 */
	public SRF(int i2cdev, int i2caddr) throws IOException {
		this.i2cdev = i2cdev;
		this.i2caddr = fixAddress(i2caddr);
		init();
	}

	/**
	 * Crée un nouveau capteur à l'adresse par défaut (0xE0)
	 * 
	 * @param i2cdev numéro de l'adapateur I2C (0, 1, ...)
	 * @throws IOException 
	 */
	public SRF(int i2cdev) throws IOException {
		this(i2cdev, 0xE0);
	}

	/**
	 * (Ré-)ouvre la connexion I2C
	 * @throws IOException 
	 */
	private void init() throws IOException {
		i2c.open(i2cdev, i2caddr);
	}

	/**
	 * Retourne l'adresse 7 bits de ce périphérique
	 */
	public int getAddress() {
		return i2caddr;
	}

	/**
	 * Renvoie la version du firmware du capteur
	 * @throws IOException 
	 */
	public int getVersion() throws IOException {
		return i2c.readRegister(REG_COMMAND);
	}

	/**
	 * Effectue une mesure de distance
	 * 
	 * @param sleepms durée en ms pendant laquelle on attend une réponse
	 * @return distance de l'obstacle en pieds (oui c'est parfaitement inutile)
	 * @throws IOException 
	 * @see {@link #getCentimeters()}
	 * @see {@link #getMicroSeconds()}
	 */
	public int getInches(int sleepms) throws IOException {
		i2c.writeRegister(0, RANGE_INCHES);

		try {
			Thread.sleep(sleepms);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return -1;
		}

		return i2c.readRegister16(REG_RANGE_HIGH, REG_RANGE_LOW);
	}

	/**
	 * Effectue une mesure de distance
	 * 
	 * @param sleepms durée en ms pendant laquelle on attend une réponse
	 * @return temps de retour du signal en micro-secondes (pas beaucoup plus
	 *         utile)
	 * @throws IOException 
	 * @see {@link #getCentimeters()}
	 * @see {@link #getInches()}
	 */
	public int getMicroSeconds(int sleepms) throws IOException {
		i2c.writeRegister(0, RANGE_MICROSECONDS);

		try {
			Thread.sleep(sleepms);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return -1;
		}

		return i2c.readRegister16(REG_RANGE_HIGH, REG_RANGE_LOW);
	}

	/**
	 * Effectue une mesure de distance
	 * 
	 * @param sleepms durée en ms pendant laquelle on attend une réponse
	 * @return distance de l'obstacle en centimètres (bien plus utile)
	 * @throws IOException 
	 * @see {@link #getMicroSeconds()}
	 * @see {@link #getInches()}
	 */
	public int getCentimeters(int sleepms) throws IOException {
		i2c.writeRegister(REG_COMMAND, RANGE_CENTIMETERS);

		try {
			Thread.sleep(sleepms);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return -1;
		}

		return i2c.readRegister16(REG_RANGE_HIGH, REG_RANGE_LOW);
	}

	/**
	 * Modifie le gain maximal du capteur
	 * 
	 * @param reg_val valeur du registre de gain telle que spécifiée dans la
	 *            documentation
	 * @throws IOException 
	 */
	public void setMaxGain(int reg_val) throws IOException {
		i2c.writeRegister(REG_MAXGAIN, (byte) reg_val);
	}

	/**
	 * Modifie la portée maximale du capteur. Plus la portée est longue, plus la
	 * mesure prendra du temps. Une mesure sur une distance maximum de 50-60cm
	 * suffit généralement.
	 * 
	 * @param reg_val valeur du registre de portée telle que spécifiée dans la
	 *            documentation
	 * @throws IOException 
	 */
	public void setMaxRange(int reg_val) throws IOException {
		i2c.writeRegister(REG_RANGE_HIGH, (byte) reg_val);
	}

	/**
	 * Change l'adresse du capteur (et s'y reconnecte).
	 * 
	 * @param newaddr nouvelle adresse sur 7 ou 8 bits (entre 0xE0 et 0xFE ou
	 *            entre 0x70 et 0x7F)
	 * @throws IOException 
	 */
	public void changeAddress(int newaddr) throws IOException {
		newaddr = fixAddress(newaddr);

		i2c.writeRegister(REG_COMMAND, CHG_ADDR1);
		i2c.writeRegister(REG_COMMAND, CHG_ADDR2);
		i2c.writeRegister(REG_COMMAND, CHG_ADDR3);
		i2c.writeRegister(REG_COMMAND, (byte) (newaddr << 1));

		i2c.close();
		i2caddr = newaddr;
		init();
	}
}
