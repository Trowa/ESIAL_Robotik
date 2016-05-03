#include "Main.h"
#include <iostream>

extern serial_t stdio_uart;

extern "C" void HardFault_Handler()
{
    error("Planté !!\r\n");
}

void loadConfig() {
    Config::loadFile("/local/config.txt");
    printf("Version configuration : %lld\r\n", Config::configVersion);
}

Serial pc(USBTX, USBRX);

int main()
{
    pc.baud(230400);
    // Initialisation du port série par défaut (utilisé par printf & co)
/*    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
    serial_baud(&stdio_uart, 230400); // GaG va être content*/

    printf("--- Asservissement Nancyborg ---\r\n");
    printf("Version " GIT_VERSION " - Compilée le " DATE_COMPIL " par " AUTEUR_COMPIL "\r\n\r\n");

    LocalFileSystem local("local");

    loadConfig();
    initAsserv();

    refLed = 1;
    gotoLed = 0;
#ifdef DEBUG
    debugUdp = new DebugUDP(commandManager, odometrie);
    dataLed = 0;
    receiveLed = 0;
    liveLed = 0;
#endif

    // On attache l'interruption timer à la méthode Live_isr
    Live.attach(Live_isr, 0.005);

    //On est prêt !
    //printf("GOGO !");

    while (1) {
        ecouteSerie();
    }
}

void ecouteSerie()
{
    double consigneValue1 = 0;
    double consigneValue2 = 0;
    char c = getchar();
    std::string name, value;
    const Parameter *param;

    switch (c) {
        //Test de débug
        case 'z' :
            // Go 10cm
            //printf("consigne avant : %d\n", consignController->getDistConsigne());
            consignController->add_dist_consigne(Utils::mmToUO(odometrie,100));
            //printf("consigne apres : %d\n", consignController->getDistConsigne());
            //putchar('z');
            break;

        case 's' :
            // Backward 10cm
            //printf("consigne avant : %d\n", consignController->getDistConsigne());
            consignController->add_dist_consigne(-Utils::mmToUO(odometrie,100));
            //printf("consigne apres : %d\n", consignController->getDistConsigne());
            //putchar('s');
            break;

        case 'q' :
            // Left 45
            consignController->add_angle_consigne(Utils::degToUO(odometrie,45));
            //putchar('q');
            break;

        case 'd' :
            // Right 45
            consignController->add_angle_consigne(-Utils::degToUO(odometrie,45));
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
            printf("x%lfy%lfa%lf\r\n", (double)Utils::UOTomm(odometrie, odometrie->getX()),
                                     (double)Utils::UOTomm(odometrie, odometrie->getY()),
                                     odometrie->getTheta());
            break;

        case 'c': {//calage bordure
                char sens = getchar(); // si 0, y vers l'intérieur de la table, si 1, y vers l'extérieur de la table
                char gros = getchar(); // g pour le Gros, p pour le petit

                if (sens != '1' && sens != '0') {
                    return;
                }

                if (gros == 'g') {
                    commandManager->calageBordureGros(sens == '1' ? 1 : 0);
                } else if (gros == 'p') {
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

            if (param == NULL) {
                std::cout << "error" << endl;
            } else {
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

void initAsserv() {
    printf("Creation des objets... ");
    fflush(stdout);

    odometrie = new Odometrie();
    motorController = new Md22(p9, p10);
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

#ifdef DEBUG
    debugUdp->setNewObjectPointers(commandManager, odometrie);
#endif

    //On reprend l'asserv
    Live.attach(Live_isr, 0.005);
}

// On rafraichit l'asservissement régulièrement
void Live_isr()
{
    static int mod = 0;
    odometrie->refresh();

    if (!Config::disableAsserv) {
        consignController->perform();
        commandManager->perform();
    }

    liveLed = 1 - liveLed;


    if ((mod++) % 20 == 0) {
        printf("#x%lfy%lfa%lfd%dvg%dvd%d\r\n", (double)Utils::UOTomm(odometrie, odometrie->getX()),
                                   (double)Utils::UOTomm(odometrie, odometrie->getY()),
                                   odometrie->getTheta(),
                                   commandManager->getLastCommandStatus(),
                                   motorController->getVitesseG(),
                                   motorController->getVitesseD());

        if (commandManager->getLastCommandStatus() == 1)
            commandManager->setLastCommandStatus(2);
    }
#ifdef DEBUG
    temps++;
    static int refreshPeriod = 0;

    if (refreshPeriod++ == 10) {
        Net::poll();

        if (debugUdp->getDebugSend()) {
            debugUdp->addData("t", (double)(temps * 0.05));
            debugUdp->sendData();
            dataLed = 1 - dataLed;
        }

        refreshPeriod = 0;
    } else {
        dataLed = 0;
        debugUdp->dropCurrentData();
    }
#endif
}

