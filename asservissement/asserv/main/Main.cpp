#include "Main.h"
#include "mbed.h"
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <iostream>

//extern serial_t stdio_uart;

extern "C" void HardFault_Handler()
{
	error("Planté !!\r\n");
}

void loadConfig()
{
	Config::loadFile("/local/config.txt");
	printf("Version configuration : %ld\r\n", Config::configVersion);
}

Serial pc(USBTX, USBRX);

I2CSlave slave(p9, p10);

int main()
{
	ErrorLed = 0;

	//pc.baud(9600);
	pc.baud(230400);
	// Initialisation du port série par défaut (utilisé par printf & co)
	//    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
	// serial_baud(&stdio_uart, 230400); // GaG va être content

	printf("--- Asservissement Nancyborg ---\r\n");
	printf("Version " GIT_VERSION " - Compilée le " DATE_COMPIL " par " AUTEUR_COMPIL "\r\n\r\n");

	LocalFileSystem local("local");

	loadConfig();
	initAsserv();

	refLed = 1;
	gotoLed = 0;
#ifdef DEBUG_UDP
	debugUdp = new DebugUDP(commandManager, odometrie);
	dataLed = 0;
	receiveLed = 0;
	liveLed = 0;
#endif

	// On attache l'interruption timer à la méthode Live_isr
	Live.attach(Live_isr, ASSERV_FREQ);

	//On est prêt !
	printf("GOGO !");

	leftSpeed = 0;

	ecouteI2cConfig();

	while (1)
	{
		//ecouteSerie();
		ecouteSeriePC();
		ecouteI2c();
	}
}
void ecouteSeriePC()
{

	/*
	 *
	 *
	 * Commande / Caractères à envoyer sur la série / Paramètres / Effets obtenus

	 g%x#%y\n / Goto / x, y : entiers, en mm /Le robot se déplace au point de coordonnée (x, y). Il tourne vers le point, puis avance en ligne droite. L'angle est sans cesse corrigé pour bien viser le point voulu.
	 e%x#%y\n / goto Enchaîné / x, y : entiers, en mm / Idem que le Goto, sauf que lorsque le robot est proche du point d'arrivée (x, y), on s'autorise à enchaîner directement la consigne suivante si c'est un Goto ou un Goto enchaîné, sans marquer d'arrêt.
	 v%d\n / aVancer / d : entier, en mm / Fait avancer le robot de d mm, tout droit
	 t%a\n / Tourner / a : entier, en degrées / Fait tourner le robot de a degrées. Le robot tournera dans le sens trigonométrique : si a est positif, il tourne à gauche, et vice-versa.
	 f%x#%y\n / faire Face / x, y : entiers, en mm / Fait tourner le robot pour être en face du point de coordonnées (x, y). En gros, ça réalise la première partie d'un Goto : on se tourne vers le point cible, mais on avance pas.
	 h / Halte ! / Arrêt d'urgence ! Le robot est ensuite systématiquement asservi à sa position actuelle. Cela devrait suffire à arrêter le robot correctement. La seule commande acceptée par la suite sera un Reset de l'arrêt d'urgence : toute autre commande sera ignorée.
	 r / Reset de l'arrêt d'urgence / Remet le robot dans son fonctionnement normal après un arrêt d'urgence. Les commandes en cours au moment de l'arrêt d'urgence NE sont PAS reprises. Si le robot n'est pas en arrêt d'urgence, cette commande n'a aucun effet.
	 c%s%r / Calage bordure / s : sens du calage bordure, r : robot ('g' : gros ; 'p' : petit) / Effectue un calage bordure. Le robot doit être dans sa zone de départ au début du calage, dirigé vers la case de départ adverse en face de la table. Il doit être assez proche de la bordure derrière lui, et pas trop proche de la bordure sur le côté. A la fin du calage, le robot est prêt à partir pour un match dans sa case de départ.
	 Le choix du robot est possible, si on veut que deux robots asservis concourent en même temps sur la même table, pour qu'ils puissent faire un calage bordure en même temps sans se rentrer dedans.
	 p / Position / Récupère la position et le cap du robot sur la connexion série, sous la forme "x%xy%ya%a\n", avec x, y, et a les coordonnées et l'angle du robot.

	 z / avance de 10 cm
	 s / recule de 10 cm
	 q / tourne de 45° (gauche)
	 d / tourne de -45° (droite)

	 c / calage bordure

	 S / modifie la valeur d'un paramètre / name, value
	 R / réinitialiser l'asserv
	 D / dump la config du robot
	 G / lire la valeur d'un paramètre / name
	 L / recharge la config config.txt
	 W / sauvegarde la config courante  config~1.txt = config.default.txt

	 a / active/reactive le consignController et le commlandManager
	 + / applique une valeur +1 sur les moteurs setLeftSpeed
	 - / applique une valeur -1 sur les moteurs setLeftSpeed


	 */

	double consigneValue1 = 0;
	double consigneValue2 = 0;
	std::string name, value;
	const Parameter *param;

	if (pc.readable())
	{

		switch (pc.getc())
		{
		case 'h': //Arrêt d'urgence
			commandManager->setEmergencyStop();
			pc.printf("Arrêt d'urgence ! ");
			break;

		case 'r': //Reset de l'arrêt d'urgence
			commandManager->resetEmergencyStop();
			break;

		case 'z':
			// Go 10cm
			//printf("consigne avant : %d\n", consignController->getDistConsigne());
			consignController->add_dist_consigne(Utils::mmToUO(odometrie, 100));
			//consignController->add_angle_consigne(Utils::degToUO(odometrie, 0));
			//pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
			break;

		case 's':
			// Backward 10cm
			//printf("consigne avant : %d\n", consignController->getDistConsigne());
			consignController->add_dist_consigne(-Utils::mmToUO(odometrie, 100));
			//pc.printf("consigne apres : %d\n", consignController->getDistConsigne());
			break;

		case 'q':
			// Left 45
			consignController->add_angle_consigne(Utils::degToUO(odometrie, 45));
			break;

		case 'd':
			// Right 45
			consignController->add_angle_consigne(-Utils::degToUO(odometrie, 45));
			break;

		case 'v': //aVance d'un certain nombre de mm
			pc.scanf("%lf", &consigneValue1);
			commandManager->addStraightLine((int32_t) consigneValue1);
			pc.printf("v%lf\r\n", consigneValue1);
			break;

		case 't': //Tourne d'un certain angle en degrés
			scanf("%lf", &consigneValue1);
			commandManager->addTurn((int64_t) consigneValue1);
			pc.printf("t%lf\n", consigneValue1);
			break;

		case 'f': //faire Face à un point précis, mais ne pas y aller, juste se tourner
			scanf("%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
			commandManager->addGoToAngle((int64_t) consigneValue1, (int64_t) consigneValue2);
			pc.printf("g%lf#%lf\n", consigneValue1, consigneValue2);
			break;

		case 'p': //retourne la Position et l'angle courants du robot
			printf("x%lfy%lfa%lf\r\n", (double) Utils::UOTomm(odometrie, odometrie->getX()),
					(double) Utils::UOTomm(odometrie, odometrie->getY()), odometrie->getTheta());
			break;

		case 'c':
		{ //calage bordure
			char sens = getchar(); // si 0, y vers l'intérieur de la table, si 1, y vers l'extérieur de la table
			char gros = getchar(); // g pour le Gros, p pour le petit

			if (sens != '1' && sens != '0')
			{
				return;
			}

			if (gros == 'g')
			{
				commandManager->calageBordureGros(sens == '1' ? 1 : 0);
			}
			else if (gros == 'p')
			{
				commandManager->calageBordurePetit(sens == '1' ? 1 : 0);
			}

			//printf("c%c%c", sens, gros);
		}
			break;

		case 'R': // réinitialiser l'asserv
			resetAsserv();
			break;

		case 'D': // dump la config du robot
			std::cout << Config::dumpConfig() << std::endl;
			break;

		case 'G': // lire la valeur d'un paramètre
			std::getline(std::cin, name, '\r');
			param = Config::getParam(name);

			if (param == NULL)
				std::cout << "error" << endl;
			else
				std::cout << param->toString() << std::endl;
			break;

		case 'S': // modifie la valeur d'un paramètre
			std::getline(std::cin, name, '\r');
			std::getline(std::cin, value, '\r');
			param = Config::getParam(name);

			if (param == NULL)
			{
				std::cout << "error" << endl;
			}
			else
			{
				param->setFromString(value);
				std::cout << "ok" << std::endl;
			}
			break;

		case 'L': // recharge la config
			loadConfig();
			std::cout << "ok" << endl;
			break;

		case 'W': // sauvegarde la config courante
			// config~1.txt = config.default.txt
			Config::saveToFile("/local/config~1.txt", "/local/config.txt");
			std::cout << "ok" << endl;
			break;

		case '+':
			leftSpeed++;
			consignController->setLeftSpeed(leftSpeed);
			pc.printf("+%d ", leftSpeed);
			break;
		case '-':
			leftSpeed--;
			consignController->setLeftSpeed(leftSpeed);
			pc.printf("-%d ", leftSpeed);
			break;

		case 'a':
			// do what you want for 'a'
			pc.printf("--a\r\n");
			consignController->perform_On(false);
			commandManager->perform_On(false);
			break;

		case '3':
			// do what you want for '3'
			pc.printf("--3\r\n");

			break;
		default:
			pc.printf(" - unexpected character\r\n");
			break;
		}
	}
}

void ecouteI2cConfig()
{
	slave.frequency(100000);
	slave.address(0xAA);
}

void ecouteI2c()
{
	char buf[10];
	char msg[] = "Slave!";
	while (1)
	{
		int i = slave.receive();
		switch (i)
		{
		case I2CSlave::ReadAddressed:
			slave.write(msg, strlen(msg) + 1); // Includes null char
			break;
		case I2CSlave::WriteGeneral:
			slave.read(buf, 10);
			printf("Read G: %s\n", buf);
			break;
		case I2CSlave::WriteAddressed:
			slave.read(buf, 10);
			printf("Read A: %s\n", buf);
			break;
		}
		for (int i = 0; i < 10; i++)
			buf[i] = 0;    // Clear buffer
	}

}

void ecouteSerie()
{
	double consigneValue1 = 0;
	double consigneValue2 = 0;
	char c = getchar();
	std::string name, value;
	const Parameter *param;

	switch (c)
	{
	//Test de débug
	case 'z':
		// Go 10cm
		//printf("consigne avant : %d\n", consignController->getDistConsigne());
		consignController->add_dist_consigne(Utils::mmToUO(odometrie, 100));
		//printf("consigne apres : %d\n", consignController->getDistConsigne());
		//putchar('z');
		break;

	case 's':
		// Backward 10cm
		//printf("consigne avant : %d\n", consignController->getDistConsigne());
		consignController->add_dist_consigne(-Utils::mmToUO(odometrie, 100));
		//printf("consigne apres : %d\n", consignController->getDistConsigne());
		//putchar('s');
		break;

	case 'q':
		// Left 45
		consignController->add_angle_consigne(Utils::degToUO(odometrie, 45));
		//putchar('q');
		break;

	case 'd':
		// Right 45
		consignController->add_angle_consigne(-Utils::degToUO(odometrie, 45));
		//putchar('d');
		break;

	case 'h': //Arrêt d'urgence
		commandManager->setEmergencyStop();
		//putchar('h');
		break;

	case 'r': //Reset de l'arrêt d'urgence
		commandManager->resetEmergencyStop();
		//putchar('r');
		break;

	case 'g': //Go : va à un point précis
		gotoLed = !gotoLed;
		scanf("%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
		commandManager->addGoTo((int64_t) consigneValue1, (int64_t) consigneValue2);
		//printf("g%lf#%lf\n", consigneValue1, consigneValue2);
		break;

	case 'e': // goto, mais on s'autorise à Enchainer la consigne suivante sans s'arrêter
		gotoLed = !gotoLed;
		scanf("%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
		commandManager->addGoToEnchainement((int64_t) consigneValue1, (int64_t) consigneValue2);
		//printf("g%lf#%lf\n", consigneValue1, consigneValue2);
		break;

	case 'v': //aVance d'un certain nombre de mm
		scanf("%lf", &consigneValue1);
		commandManager->addStraightLine((int64_t) consigneValue1);
		//printf("v%lf\n", consigneValue1);
		break;

	case 't': //Tourne d'un certain angle en degrés
		scanf("%lf", &consigneValue1);
		commandManager->addTurn((int64_t) consigneValue1);
		//printf("t%lf\n", consigneValue1);
		break;

	case 'f': //faire Face à un point précis, mais ne pas y aller, juste se tourner
		scanf("%lf#%lf", &consigneValue1, &consigneValue2); //X, Y
		commandManager->addGoToAngle((int64_t) consigneValue1, (int64_t) consigneValue2);
		//printf("g%lf#%lf\n", consigneValue1, consigneValue2);
		break;

	case 'p': //retourne la Position et l'angle courants du robot
		printf("x%lfy%lfa%lf\r\n", (double) Utils::UOTomm(odometrie, odometrie->getX()),
				(double) Utils::UOTomm(odometrie, odometrie->getY()), odometrie->getTheta());
		break;

	case 'c':
	{ //calage bordure
		char sens = getchar(); // si 0, y vers l'intérieur de la table, si 1, y vers l'extérieur de la table
		char gros = getchar(); // g pour le Gros, p pour le petit

		if (sens != '1' && sens != '0')
		{
			return;
		}

		if (gros == 'g')
		{
			commandManager->calageBordureGros(sens == '1' ? 1 : 0);
		}
		else if (gros == 'p')
		{
			commandManager->calageBordurePetit(sens == '1' ? 1 : 0);
		}

		//printf("c%c%c", sens, gros);
	}
		break;

	case 'R': // réinitialiser l'asserv
		resetAsserv();
		break;

	case 'D': // dump la config du robot
		std::cout << Config::dumpConfig() << std::endl;
		break;

	case 'G': // lire la valeur d'un paramètre
		std::getline(std::cin, name, '\r');
		param = Config::getParam(name);

		if (param == NULL)
			std::cout << "error" << endl;
		else
			std::cout << param->toString() << std::endl;
		break;

	case 'S': // modifie la valeur d'un paramètre
		std::getline(std::cin, name, '\r');
		std::getline(std::cin, value, '\r');
		param = Config::getParam(name);

		if (param == NULL)
		{
			std::cout << "error" << endl;
		}
		else
		{
			param->setFromString(value);
			std::cout << "ok" << std::endl;
		}
		break;
	case 'L': // recharge la config
		loadConfig();
		std::cout << "ok" << endl;
		break;
	case 'W': // sauvegarde la config courante
		// config~1.txt = config.default.txt
		Config::saveToFile("/local/config~1.txt", "/local/config.txt");
		std::cout << "ok" << endl;
		break;
	default:
		//putchar(c);
		break;
	}
}

void initAsserv()
{
	printf("Creation des objets... ");
	fflush(stdout);

	odometrie = new Odometrie();
	motorController = new Md25ctrl(p28, p27);
	//motorController = new Md25ctrl(p9, p10);
	//motorController = new Md22(p9, p10);
	//motorController = new Qik(p9, p10);
	//motorController = new PololuSMCs(p13, p14, p28, p27);

	consignController = new ConsignController(odometrie, motorController);
	commandManager = new CommandManager(50, consignController, odometrie);

	printf("ok\r\n");
}

void resetAsserv()
{
	printf("Réinitialisation de l'asserv...\r\n");
	//On arrête le traitement de l'asserv
	Live.detach();

	// On détruit tout les objets (sauf les moteurs, on s'en fiche de ça)
	delete odometrie;
	odometrie = NULL;

	delete consignController;
	consignController = NULL;

	delete commandManager;
	commandManager = NULL;

	delete motorController;
	motorController = NULL;

	// Et on les refait !!!
	initAsserv();

#ifdef DEBUG_UDP
	debugUdp->setNewObjectPointers(commandManager, odometrie);
#endif

	//On reprend l'asserv
	Live.attach(Live_isr, ASSERV_FREQ);
}

// On rafraichit l'asservissement régulièrement
void Live_isr()
{

	liveLed = 1 - liveLed;

	static int mod = 0;
	odometrie->refresh();

	if (!Config::disableAsserv)
	{
		consignController->perform();
		commandManager->perform();
	}

	if ((mod++) % 20 == 0)
	{
#ifdef DEBUG_ASSERV
		printf("%d #x%lfy%lfa%lfd%dvg%dvd%d\r\n", asserv_flip,
				(double) Utils::UOTomm(odometrie, odometrie->getX()),
				(double) Utils::UOTomm(odometrie, odometrie->getY()),
				odometrie->getTheta(), commandManager->getLastCommandStatus(),
				motorController->getVitesseG(), motorController->getVitesseD());
#endif
		if (commandManager->getLastCommandStatus() == 1)
			commandManager->setLastCommandStatus(2);
	}

#ifdef DEBUG_UDP
	temps++;
	static int refreshPeriod = 0;

	if (refreshPeriod++ == 10)
	{
		Net::poll();

		if (debugUdp->getDebugSend())
		{
			debugUdp->addData("t", (double)(temps * 0.05));
			debugUdp->sendData();
			dataLed = 1 - dataLed;
		}

		refreshPeriod = 0;
	}
	else
	{
		dataLed = 0;
		debugUdp->dropCurrentData();
	}
#endif
}

