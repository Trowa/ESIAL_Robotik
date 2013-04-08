package api.gpio;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

public class Gpio {
	
	public int pin; // Numero de la pin
	public boolean entree; // Entree ou Sortie
	
	public Gpio(int pin, boolean entree) {
		this.pin = pin;
		this.entree = entree;
		this.configGpio();
	}
	
	public void configGpio() {
		System.out.println("Configuration Gpio "+this.pin);
		try {
			// On export le GPIO
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/export", false)));
			export.write(""+this.pin);
			export.flush();
			export.close();
			// On set la direction
			export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + this.pin + "/direction", false)));
			if (this.entree) {
				export.write("in");
			} else {
				export.write("out");
			}
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void closeGpio() {
		System.out.println("Fermeture Gpio "+this.pin);
		try {
			// On unexport les GPIOs
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/unexport", false)));
			export.println(""+this.pin);
			export.flush();
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	// Est-ce qu'on recoit un 1 ?
	public boolean isHigh() {
		try {
			InputStream ips=new FileInputStream("/sys/class/gpio/gpio"+this.pin+"/value"); 
			InputStreamReader ipsr=new InputStreamReader(ips);
			BufferedReader br=new BufferedReader(ipsr);
			String value = br.readLine();
			br.close();
			ipsr.close();
			ips.close();
			return (value != null && value.equals("1"))?true:false;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		return false;
	}
	
	// Est-ce qu'on recoit un 0 ?
	public boolean isLow() {
		return !this.isHigh();
	}
	
	// On ecrit 1 sur la pin
	public void setHigh() {
		try {
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + this.pin + "/value", false)));
			export.write("1");
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	// On ecrit 1 sur la pin
	public void setLow() {
		try {
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + this.pin + "/value", false)));
			export.write("0");
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
