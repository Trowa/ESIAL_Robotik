package servomoteur;



public class TestPololu {
	
	public static void main(String args[]) throws Exception {
		Pince pince = new Pince();
		pince.closePinceAv();
		pince.closePinceAr();
		Thread.currentThread().sleep(1000);
		pince.openPinceAv();
		Thread.currentThread().sleep(2000);
		pince.closePinceAv();
		Thread.currentThread().sleep(2000);
		pince.openPinceAr();
		Thread.currentThread().sleep(2000);
		pince.closePinceAr();
		Thread.currentThread().sleep(2000);
		pince.openPinceAv();
		pince.openPinceAr();
		Thread.currentThread().sleep(2000);
		pince.ejecter();
		Thread.currentThread().sleep(500);
		pince .FermerBenne();
		Thread.currentThread().sleep(500);
		pince.ejecter();
		Thread.currentThread().sleep(500);
		pince .FermerBenne();
		Thread.currentThread().sleep(2000);
		pince.closePinceAv();
		pince.closePinceAr();
			
	}

}
