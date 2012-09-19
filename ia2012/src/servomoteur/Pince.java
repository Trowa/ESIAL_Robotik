package servomoteur;

public class Pince {
	PololuMicroMaestro p;
	
	public Pince(){
		p = new PololuMicroMaestro("/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00026569-if00", 
					"/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00026569-if02", 12);
		p.setMinPosms(1,1224);// Pince AVG
		//p.setMaxPosms(1,2450);
		p.setMaxPosms(1,2142);
		//p.setMinPosms(2,928);  // Pince AVD
		p.setMinPosms(2,1025);
		p.setMaxPosms(2,1952);
		//p.setMinPosms(3,992); //Pince ARG
		p.setMinPosms(3,1118);
		p.setMaxPosms(3,2000);
		p.setMinPosms(4,847); //Pince ARD
		//p.setMaxPosms(4,2000);
		p.setMaxPosms(4,1715);
		p.setMinPosms(5,1050);
		p.setMaxPosms(5,1740);
	}
	
	public void openPinceAv(){
		System.out.println("Ouverture pince avant");
		p.setMaxPosms(1,2450);
		p.setMinPosms(2,800);
		p.setPositionms(1,p.getMaxPosms(1));
		p.setPositionms(2,p.getMinPosms(2));
	}
	
	public void openPinceAvForEject(){
		System.out.println("Ouverture pince avant pour éjection");
		p.setMaxPosms(1,2142);
		p.setMinPosms(2,1025);
		p.setPositionms(1,p.getMaxPosms(1));
		p.setPositionms(2,p.getMinPosms(2));
	}
		
	public void closePinceAv(){
		System.out.println("Fermeture pince avant");
		p.setPositionms(1,p.getMinPosms(1));
		p.setPositionms(2,p.getMaxPosms(2));
	}
	
		
	public void openPinceAr(){
		System.out.println("ouverture pince arrière");
		p.setMaxPosms(4,2000);
		p.setMinPosms(3,992);
		p.setPositionms(3,p.getMinPosms(3));
		p.setPositionms(4,p.getMaxPosms(4));
		
			
	}
	
	public void openPinceArForEject(){
		System.out.println("ouverture pince arrière pour éjection");
		p.setMinPosms(3,1118);
		p.setMaxPosms(4,1715);
		p.setPositionms(3,p.getMinPosms(3));
		p.setPositionms(4,p.getMaxPosms(4));
			
	}
		
	public void closePinceAr(){
		System.out.println("fermeture pince arrière");
		p.setPositionms(3,p.getMaxPosms(3));
		p.setPositionms(4,p.getMinPosms(4));
	}
	
	public void setSpeed(int speed){ // MARCHE PAS !
		p.setmaxSpeed(1,speed);
		p.setmaxSpeed(2,speed);
	}

	public void ejecter(){
		System.out.println("éjection");
		p.setPositionms(5,p.getMaxPosms(5));
	}
	
	public void fermerBenne(){
		System.out.println("fermeture benne");
		p.setPositionms(5,p.getMinPosms(5));
	}
	

}

