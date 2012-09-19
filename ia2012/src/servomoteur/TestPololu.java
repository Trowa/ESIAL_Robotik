package servomoteur;



public class TestPololu {
	
	public static void main(String args[]) throws Exception {
		Pince pince = new Pince();
		pince.closePinceAv();
		pince.closePinceAr();
		Thread.sleep(1000);
		pince.openPinceAv();
		Thread.sleep(2000);
		pince.closePinceAv();
		Thread.sleep(2000);
		pince.openPinceAr();
		Thread.sleep(2000);
		pince.closePinceAr();
		Thread.sleep(2000);
		pince.openPinceAv();
		pince.openPinceAr();
		Thread.sleep(2000);
		pince.ejecter();
		Thread.sleep(500);
		pince.fermerBenne();
		Thread.sleep(500);
		pince.ejecter();
		Thread.sleep(500);
		pince .fermerBenne();
		Thread.sleep(2000);
		pince.closePinceAv();
		pince.closePinceAr();
			
	}

}
