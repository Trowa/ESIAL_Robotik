package gpio;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

/*
 * Objet Interrupteur sur la PandaBoard
 * Le contructeur prend en parametres les deux pins necessaire au fontionnement d'un interrupteur
 * parmis les GPIO, la premiere est une sortie envoyant 1 en continue et la seconde est l'entree
 * permettant de verifier l'etat de l'interrupteur
 */

public class Interrupteur {

	public int out; // Numero de la pin de sortie a 1 en permanence
	public int in; // Numero de la pin en entree qui check l'interrupteur

	public Interrupteur(int out, int in) {
		System.out.println("New Interrupteur : Out = "+out+", In = "+in);
		this.out = out;
		this.in = in;
		this.configGpios();
	}
	
	public void configGpios() {
		System.out.println("Configuration Interrupteur Gpios "+this.out+" et "+this.in);
		try {
			// On export les GPIOs
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/export", false)));
			export.println(out);
			export.flush();
			export.println(in);
			export.flush();
			export.close();
			// On met le premier en sortie
			export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + out + "/direction", false)));
			export.write("out");
			export.close();
			//  On met le second en entree
			export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + in + "/direction", false)));
			export.write("in");
			export.close();
			// On met la sortie a 1
			export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/gpio" + out + "/value", false)));
			export.write(1);
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void closeGpios() {
		System.out.println("Fermeture Interrupteur Gpios "+this.out+" et "+this.in);
		try {
			// On unexport les GPIOs
			PrintWriter export = new PrintWriter(new BufferedWriter(new FileWriter("/sys/class/gpio/unexport", false)));
			export.println(out);
			export.flush();
			export.println(in);
			export.flush();
			export.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public boolean checkInterrupteur() {
		try {
			InputStream ips=new FileInputStream("/sys/class/gpio/gpio"+this.in+"/value"); 
			InputStreamReader ipsr=new InputStreamReader(ips);
			BufferedReader br=new BufferedReader(ipsr);
			String value = br.readLine();
			System.out.println(value);
			return (value != null && value.equals("1"))?true:false;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} 
		return false;
	}

}
