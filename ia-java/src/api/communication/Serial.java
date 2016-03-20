package api.communication;

import gnu.io.*;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.file.FileSystems;

/**
 * Représente un port série.
 * 
 * @author mickael
 * 
 */
public class Serial {
	private final SerialPort port;
	private final OutputStream out;
	private final BufferedInputStream in;

	/**
	 * Ouvre un port série
	 * 
	 * @param path chemin du port série (/dev/ttyAMA0 par exemple)
	 * @param speed vitesse en bauds
	 */
	public Serial(String path, int speed) throws NoSuchPortException, PortInUseException, UnsupportedCommOperationException, IOException {
		path = FileSystems.getDefault().getPath(path).toRealPath().toString();
		System.out.println("Open Serial: " + path);

		this.port = (SerialPort) CommPortIdentifier.getPortIdentifier(path).open("IA", 1000);
		this.port.setSerialPortParams(speed, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
		this.out = this.port.getOutputStream();
		this.in = new BufferedInputStream(this.port.getInputStream());
	}

	/**
	 * Écrit un ou plusieurs octets dans le port série.
	 * 
	 * @param bytes
	 */
	public synchronized void write(byte... bytes) throws IOException {
		this.out.write(bytes);
		this.out.flush();
	}

	/**
	 * Écrit un ou plusieurs octets dans le port série.
	 * 
	 * @param bytes
	 */
	public synchronized void write(int... bytes) throws IOException {
		byte[] data = new byte[bytes.length];
		for (int i = 0; i < bytes.length; i++)
			data[i] =  (byte)(bytes[i] & 0xFF);
		this.write(data);
	}

	/**
	 * Écrit une chaine de caractère dans le port série.
	 * 
	 * @param str
	 */
	public synchronized void write(String str) throws IOException {
		this.out.write(str.getBytes());
		this.out.flush();
	}

	/**
	 * Lit un octet depuis le port série.
	 */
	public synchronized byte readByte() throws IOException {
		return (byte) in.read();
	}

	/**
	 * Lit un octet depuis le port série (résultat dans un entier).
	 */
	public synchronized int read() throws IOException {
		return in.read();
	}

	/**
	 * Lit plusieurs octets depuis le port série
	 * 
	 * @throws IOException
	 */
	public byte[] read(int numbytes) throws IOException {
		byte[] arr = new byte[numbytes];
		int offset = 0;

		while (offset < numbytes) {
			int nb = in.read(arr, offset, numbytes - offset);

			if (nb <= 0) {
				throw new IOException("EOF");
			}

			offset += nb;
		}

		return arr;
	}

	/**
	 * Lit un caractère depuis le port série.
	 */
	public synchronized char readChar() throws IOException {
		return (char) read();
	}

	/**
	 * Lit une ligne depuis le port série.
	 */
	public synchronized String readLine() throws IOException {
		String ret = "";

		while (true) {
			char c = readChar();

			if (c == '\n' || c == 0) {
				break;
			}

			ret += c;
		}

		return ret;
	}

	/**
	 * Indique si il y a au moins un octet en attente d'être lu
	 */
	public synchronized boolean ready() throws IOException {
		return in.available() > 0;
	}

	public void sendBreak() {
		port.sendBreak(100);
	}
}
