package api.lcd03;

import api.hardware.RaspberryPi;

public class LCDTest {
	public static void main(String[] args) throws Exception {
		int dev = (RaspberryPi.getHardwareVersion() >= 4 ? 1 : 0);

		LCDController controller = new LCDController(dev);

		System.out.println("Version = " + controller.getVersion());

		System.out.println("**** TEST LCD ****");
		System.out.println();

		LCD lcd = controller.getLCD();

		System.out.println("* Effacage écran");
		Thread.sleep(3000);
		lcd.clearScreen();
		System.out.println();

		System.out.println("* Allumage backlight");
		Thread.sleep(3000);
		lcd.setBacklight(true);
		System.out.println();

		System.out.println("* Arrêt backlight");
		Thread.sleep(3000);
		lcd.setBacklight(false);
		System.out.println();

		System.out.println("* Remplissage");
		Thread.sleep(3000);
		lcd.write("\r");
		lcd.write("\t\tTHE GAME\r\r");
		lcd.write("  You just lost it");
		System.out.println();

		System.out.println("* Curseur début");
		Thread.sleep(3000);
		lcd.cursorHome();
		System.out.println();

		System.out.println("* Curseur ligne 3, colonne 10");
		Thread.sleep(3000);
		lcd.setCursor(3, 10);
		System.out.println();

		System.out.println("* Smiley face");
		Thread.sleep(3000);
		lcd.createCustomChar(128, new byte[] { (byte) 138, (byte) 138, (byte) 138, (byte) 128, (byte) 145, (byte) 145, (byte) 142, (byte) 128 });
		lcd.write((byte) 128);
		System.out.println();

		System.out.println("* Curseur caché");
		Thread.sleep(3000);
		lcd.hideCursor();
		System.out.println();

		System.out.println("* Curseur souligné");
		Thread.sleep(3000);
		lcd.underlineCursor();
		System.out.println();

		System.out.println("* Curseur clignotant");
		Thread.sleep(3000);
		lcd.blinkCursor();
		System.out.println();

		System.out.println("* Backspace");
		Thread.sleep(3000);
		lcd.backspace();
		System.out.println();

		System.out.println("* Curseur position ");
		Thread.sleep(3000);
		lcd.backspace();
		System.out.println();
	}
}
