package api.gpio;

public class Interrupteur {

	public Gpio gpio;
	
	public Interrupteur(int gpio) {
		this.gpio = new Gpio(gpio, true);
	}
	
	public boolean isOuvert() {
		return this.gpio.isLow();
	}
	
	public boolean isFerme() {
		return this.gpio.isHigh();
	}
}
