package api.gpio;

public class Led {
	
	public Gpio gpio;
	
	public Led(int gpio) {
		this.gpio = new Gpio(gpio, false);
		this.gpio.setLow();
	}
	
	public void allumer() {
		this.gpio.setHigh();
	}

	public void eteindre() {
		this.gpio.setLow();
	}
}
