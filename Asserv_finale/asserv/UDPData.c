#include <string>
#include <list>
#include "EthernetNetIf.h"
#include "UDPSocket.h"

#include "ConsignController.h"

#define PARAM "Pa=Pivot angle#Pd=Pivot distance#X=coordonnee X#Y=coordonnee Y#T=Cap#v=Vitesse gauche#V=Vitesse droit#Vl=Vitesse lineaire#Va=Vitesse angulaire#Ea=Erreur angle#Ed=Erreur distance"

static EthernetNetIf *eth;
static Host* brdHost;
static UDPSocket udp;
static Serial* pc; 
static void onUDPSocketEvent(UDPSocketEvent e);
static EthernetNetIf* loadIPFromConf();
static char trame[4096];
static char buf[1024];

static ConsignController* cc;
static Odometrie* odo;

void udp_data_init(ConsignController* c , Odometrie* o) {
    eth = loadIPFromConf();
    EthernetErr ethErr = eth->setup();
    brdHost = new Host(IpAddr(192,168,0,1), 50000);
    udp.setOnEvent(&onUDPSocketEvent);
    udp.bind(*brdHost);
    pc = new Serial(USBTX, USBRX);
    pc->baud(115200);
    cc = c;
    odo = o;
}

void udp_data_add(char* name, double value) {
    sprintf(trame, "%s%s=%f#",trame, name, value);
}

void udp_data_send() {
   int n = 0;
   Net::poll();
   n = udp.sendto( trame, strlen(trame), brdHost);
   trame[0] = '\0';  
}

void udp_data_destroy() {
    delete(eth);
    delete(brdHost);
}

void udp_data_load() {
    LocalFileSystem local("conf");
    FILE *fp = fopen("/conf/coeffs.cf", "r");
    char name;
    double value = 0;
    
    if(fp == NULL) { // pas de fichier coeffs.conf
        pc->printf("\npas de fichier coeffs.conf", value);
        return;
    }
    
    while(fscanf(fp, "%c=%lf#",&name, &value) == 2) {
        switch(name) {
               case 'a':   /*kp*/
                    pc->printf("\nParametrage Coeff-Distance Kp = %f", value);
                    cc->dist_regu.setKP(value);
                    break;
                case 'b':   /*ki*/
                    pc->printf("\nParametrage Coeff-Distance Ki = %f", value);
                    cc->dist_regu.setKI(value);
                    break;
                case 'd':   /*kd*/
                    pc->printf("\nParametrage Coeff-Distance Kd = %f", value);
                    cc->dist_regu.setKD(value);
                    break;
                case 'e':   /*gain sortie*/
                    pc->printf("\nParametrage Coeff-Distance gain sortie = %f", value);
                    cc->dist_regu.setRatioSortie(value);
                    break;
                case 'f':   /*max output*/
                    pc->printf("\nParametrage Coeff-Distance max output = %ld", (int64_t)value);
                    cc->dist_regu.setSortieMax((int64_t)value);
                    break;
                case 'g':   /* max integrale */
                    pc->printf("\nParametrage Coeff-Distance max integrale = %f", value);
                   //cc->dist_regu.setMaxIntegral(value);
                    break;
                /* ------------------ coeffs angle ------------------------ */
                case 'h':   /*kp*/
                    pc->printf("\nParametrage Coeff-Distance Kp = %f", value);
                    cc->angle_regu.setKP(value);
                    break;
                case 'i':   /*ki*/
                    pc->printf("\nParametrage Coeff-Distance Ki = %f", value);
                    cc->angle_regu.setKI(value);
                    break;
                case 'j':   /*kd*/
                    pc->printf("\nParametrage Coeff-Distance Kd = %f", value);
                    cc->angle_regu.setKD(value);
                    break;
                case 'k':   /*gain sortie*/
                    pc->printf("\nParametrage Coeff-Distance gain sortie = %f", value);
                    cc->angle_regu.setRatioSortie(value);
                    break;
                case 'l':   /*max output*/
                    pc->printf("\nParametrage Coeff-Distance max output = %ld", (int64_t)value);
                    cc->angle_regu.setSortieMax((int64_t)value);
                    break;
                case 'm':   /* max integrale */
                    pc->printf("\nParametrage Coeff-Distance max integrale = %f", value);
                  //  cc->angle_regu.setMaxIntegral(value);
                    break;
                    
                    
                /* ------------------ coeffs Rampe distance  ------------------------ */
                
                 case 'A':
                    pc->printf("\nParametrage rampe-Distance derivee 1ier pos = %lf", value);
                    cc->dist_regu.setDerivee1ier( value, cc->dist_regu.getDerivee1ierNeg() );
                    break;
                case 'B':
                    pc->printf("\nParametrage rampe-Distance derivee 1ier neg = %lf", value);
                    cc->dist_regu.setDerivee1ier( cc->dist_regu.getDerivee1ierPos() , value);
                    break;
                case 'C':
                    pc->printf("\nParametrage rampe-Distance derivee 2ier pos AVANT=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAv( (int64_t)value, cc->dist_regu.getDerivee2ndNegAv());
                    break;
                case 'D':   
                    pc->printf("\nParametrage rampe-Distance derivee 2ier neg AVANT=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAv(  cc->dist_regu.getDerivee2ndPosAv() , (int64_t)value);
                    break;
                case 'E':
                    pc->printf("\nParametrage rampe-Distance derivee 2ier pos ARRIERE=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAr( (int64_t)value, cc->dist_regu.getDerivee2ndNegAr());
                    break;
                case 'F':   
                    pc->printf("\nParametrage rampe-Distance derivee 2ier neg ARRIERE=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAr(  cc->dist_regu.getDerivee2ndPosAr() , (int64_t)value);
                    break;
                case 'G':
                    pc->printf("\nParametrage rampe-Distance gain anticipation AVANT=  %f", value);
                    cc->dist_regu.setCoeffGainAnticipationAv( value);
                    break;
                case 'H':   
                    pc->printf("\nParametrage rampe-Distance gain anticipation ARRIERE=  %f",value);
                    cc->dist_regu.setCoeffGainAnticipationAr(value);
                case 'I':   
                    pc->printf("\nParametrage rampe-Distance taille fenetre arrivee=  %f", (double)value);
                    cc->dist_regu.setTailleFenetreArrivee(  (double)value);
                    break;
                    
                /* ------------------ coeffs Rampe angle  ------------------------ */
                case 'J':
                    pc->printf("\nParametrage rampe-Angle derivee 1ier pos = %lf", value);
                    cc->angle_regu.setDerivee1ier( value, cc->angle_regu.getDerivee1ierNeg() );
                    break;
                case 'K':
                    pc->printf("\nParametrage rampe-Angle derivee 1ier neg = %lf", value);
                    cc->angle_regu.setDerivee1ier( cc->angle_regu.getDerivee1ierPos() , value);
                    break;
                case 'L':
                    pc->printf("\nParametrage rampe-Angle derivee 2ier pos AVANT=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAv( (int64_t)value, cc->angle_regu.getDerivee2ndNegAv());
                    break;
                case 'M':   
                    pc->printf("\nParametrage rampe-Angle derivee 2ier neg AVANT=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAv(  cc->angle_regu.getDerivee2ndPosAv() , (int64_t)value);
                    break;
                case 'N':
                    pc->printf("\nParametrage rampe-Angle derivee 2ier pos ARRIERE=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAr( (int64_t)value, cc->angle_regu.getDerivee2ndNegAr());
                    break;
                case 'O':   
                    pc->printf("\nParametrage rampe-Angle derivee 2ier neg ARRIERE=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAr(  cc->angle_regu.getDerivee2ndPosAr() , (int64_t)value);
                    break;
                case 'P':
                    pc->printf("\nParametrage rampe-Angle gain anticipation AVANT=  %f", (double)value);
                    cc->angle_regu.setCoeffGainAnticipationAv( value);
                    break;
                case 'Q':   
                    pc->printf("\nParametrage rampe-Angle gain anticipation ARRIERE=  %f", value);
                    cc->angle_regu.setCoeffGainAnticipationAr( value);
                case 'R':   
                    pc->printf("\nParametrage rampe-Angle taille fenetre arrivee=  %ld", (int64_t)value);
                    cc->angle_regu.setTailleFenetreArrivee(  (int64_t)value);
                    break;
        }
    }
    fclose(fp);    
}

void udp_data_save() {
    LocalFileSystem local("conf");
    FILE *fp = fopen("/conf/coeffs.cf", "w");
    fprintf(fp,"a=%f#",cc->dist_regu.getKP() );
    fprintf(fp,"b=%f#",cc->dist_regu.getKI() );
    fprintf(fp,"d=%f#",cc->dist_regu.getKD() );
    fprintf(fp,"e=%f#",cc->dist_regu.getRatioSortie() );
    fprintf(fp,"f=%d#",cc->dist_regu.getSortieMax() );
    //sprintf(buffer,"%sg=%f#", cc->dist_regu.() );
    fprintf(fp,"h=%f#",cc->angle_regu.getKP() );
    fprintf(fp,"i=%f#",cc->angle_regu.getKI() );
    fprintf(fp,"j=%f#",cc->angle_regu.getKD() );
    fprintf(fp,"k=%f#",cc->angle_regu.getRatioSortie() );
    fprintf(fp,"l=%d#",cc->angle_regu.getSortieMax() );
                    
    /* R&#65533;gulateur sortie DISTANCE */
    fprintf(fp,"A=%d#",cc->dist_regu.getDerivee1ierPos() );
    fprintf(fp,"B=%d#",cc->dist_regu.getDerivee1ierNeg() );
    fprintf(fp,"C=%d#",cc->dist_regu.getDerivee2ndPosAv() );
    fprintf(fp,"D=%d#",cc->dist_regu.getDerivee2ndNegAv() );
    fprintf(fp,"E=%d#",cc->dist_regu.getDerivee2ndPosAr() );
    fprintf(fp,"F=%d#",cc->dist_regu.getDerivee2ndNegAr() );
    fprintf(fp,"G=%f#",cc->dist_regu.getCoeffGainAnticipationAv() );
    fprintf(fp,"H=%f#",cc->dist_regu.getCoeffGainAnticipationAr() );
    fprintf(fp,"I=%d#",cc->dist_regu.getTailleFenetreArrivee() );
                   
                    /* R&#65533;gulateur ANGLE*/
    fprintf(fp,"J=%d#",cc->angle_regu.getDerivee1ierPos() );
    fprintf(fp,"K=%d#",cc->angle_regu.getDerivee1ierNeg() );
    fprintf(fp,"L=%d#",cc->angle_regu.getDerivee2ndPosAv() );
    fprintf(fp,"M=%d#",cc->angle_regu.getDerivee2ndNegAv() );
    fprintf(fp,"N=%d#",cc->angle_regu.getDerivee2ndPosAr() );
    fprintf(fp,"O=%d#",cc->angle_regu.getDerivee2ndNegAr() );
    fprintf(fp,"P=%f#",cc->angle_regu.getCoeffGainAnticipationAv() );
    fprintf(fp,"Q=%f#",cc->angle_regu.getCoeffGainAnticipationAr() );
    fprintf(fp,"R=%d#",cc->angle_regu.getTailleFenetreArrivee() );
    
    fclose(fp);    
}

/* Callback to echo the packet that was received, on a different port) */
void onUDPSocketEvent(UDPSocketEvent e) {
    switch(e) {
        case UDPSOCKET_READABLE: //The only event for now
            Host host;
            char name;
            double value = 0;
            
            /* lecture */
            int len = udp.recvfrom( buf, 63, &host );
            pc->printf("\nReception de %s", buf);
            sscanf(buf, "%c=%lf",&name, &value);
            
            switch(name) {    /*todo: ajouter appel setters*/
                case 'c': /* connexion */
                    pc->printf("\nNouveau client!!");
                    brdHost->setIp(host.getIp());
                    brdHost->setPort(host.getPort());
                    udp.sendto(PARAM, strlen(PARAM), &host);
                    /* send coeffs init */
                    char buffer[1024];
                    buffer[0] ='\0';
                    
                    pc->printf("\nA");
                    sprintf(buffer,"%sa=%f#",buffer, cc->dist_regu.getKP() );
                    sprintf(buffer,"%sb=%f#", buffer,cc->dist_regu.getKI() );
                    sprintf(buffer,"%sd=%f#",buffer, cc->dist_regu.getKD() );
                    sprintf(buffer,"%se=%f#",buffer, cc->dist_regu.getRatioSortie() );
                    sprintf(buffer,"%sf=%d#",buffer, cc->dist_regu.getSortieMax() );
                    //sprintf(buffer,"%sg=%f#", cc->dist_regu.() );
                    sprintf(buffer,"%sh=%f#",buffer, cc->angle_regu.getKP() );
                    sprintf(buffer,"%si=%f#",buffer, cc->angle_regu.getKI() );
                    sprintf(buffer,"%sj=%f#",buffer, cc->angle_regu.getKD() );
                    sprintf(buffer,"%sk=%f#",buffer, cc->angle_regu.getRatioSortie() );
                    sprintf(buffer,"%sl=%d#",buffer, cc->angle_regu.getSortieMax() );
                    


                    /* R&#65533;gulateur sortie DISTANCE */
                    sprintf(buffer,"%sA=%d#",buffer, cc->dist_regu.getDerivee1ierPos() );
                    sprintf(buffer,"%sB=%d#", buffer,cc->dist_regu.getDerivee1ierNeg() );
                    sprintf(buffer,"%sC=%d#",buffer, cc->dist_regu.getDerivee2ndPosAv() );
                    sprintf(buffer,"%sD=%d#",buffer, cc->dist_regu.getDerivee2ndNegAv() );
                    sprintf(buffer,"%sE=%d#",buffer, cc->dist_regu.getDerivee2ndPosAr() );
                    sprintf(buffer,"%sF=%d#",buffer, cc->dist_regu.getDerivee2ndNegAr() );
                    sprintf(buffer,"%sG=%f#", buffer,cc->dist_regu.getCoeffGainAnticipationAv() );
                    sprintf(buffer,"%sH=%f#",buffer, cc->dist_regu.getCoeffGainAnticipationAr() );
                    sprintf(buffer,"%sI=%d#",buffer, cc->dist_regu.getTailleFenetreArrivee() );
                    
                    /* R&#65533;gulateur ANGLE*/
                    sprintf(buffer,"%sJ=%d#",buffer, cc->angle_regu.getDerivee1ierPos() );
                    sprintf(buffer,"%sK=%d#", buffer,cc->angle_regu.getDerivee1ierNeg() );
                    sprintf(buffer,"%sL=%d#",buffer, cc->angle_regu.getDerivee2ndPosAv() );
                    sprintf(buffer,"%sM=%d#",buffer, cc->angle_regu.getDerivee2ndNegAv() );
                    sprintf(buffer,"%sN=%d#",buffer, cc->angle_regu.getDerivee2ndPosAr() );
                    sprintf(buffer,"%sO=%d#",buffer, cc->angle_regu.getDerivee2ndNegAr() );
                    sprintf(buffer,"%sP=%f#", buffer,cc->angle_regu.getCoeffGainAnticipationAv() );
                    sprintf(buffer,"%sQ=%f#",buffer, cc->angle_regu.getCoeffGainAnticipationAr() );
                    sprintf(buffer,"%sR=%d#",buffer, cc->angle_regu.getTailleFenetreArrivee() );


                    udp.sendto(buffer, strlen(buffer), &host);
                    break;
                /* ------------------ coeffs distance ------------------------ */
                case 'a':   /*kp*/
                    pc->printf("\nParametrage Coeff-Distance Kp = %f", value);
                    cc->dist_regu.setKP(value);
                    break;
                case 'b':   /*ki*/
                    pc->printf("\nParametrage Coeff-Distance Ki = %f", value);
                    cc->dist_regu.setKI(value);
                    break;
                case 'd':   /*kd*/
                    pc->printf("\nParametrage Coeff-Distance Kd = %f", value);
                    cc->dist_regu.setKD(value);
                    break;
                case 'e':   /*gain sortie*/
                    pc->printf("\nParametrage Coeff-Distance gain sortie = %f", value);
                    cc->dist_regu.setRatioSortie(value);
                    break;
                case 'f':   /*max output*/
                    pc->printf("\nParametrage Coeff-Distance max output = %ld", (int64_t)value);
                    cc->dist_regu.setSortieMax((int64_t)value);
                    break;
                case 'g':   /* max integrale */
                    pc->printf("\nParametrage Coeff-Distance max integrale = %f", value);
                   //cc->dist_regu.setMaxIntegral(value);
                    break;
                /* ------------------ coeffs angle ------------------------ */
                case 'h':   /*kp*/
                    pc->printf("\nParametrage Coeff-Distance Kp = %f", value);
                    cc->angle_regu.setKP(value);
                    break;
                case 'i':   /*ki*/
                    pc->printf("\nParametrage Coeff-Distance Ki = %f", value);
                    cc->angle_regu.setKI(value);
                    break;
                case 'j':   /*kd*/
                    pc->printf("\nParametrage Coeff-Distance Kd = %f", value);
                    cc->angle_regu.setKD(value);
                    break;
                case 'k':   /*gain sortie*/
                    pc->printf("\nParametrage Coeff-Distance gain sortie = %f", value);
                    cc->angle_regu.setRatioSortie(value);
                    break;
                case 'l':   /*max output*/
                    pc->printf("\nParametrage Coeff-Distance max output = %ld", (int64_t)value);
                    cc->angle_regu.setSortieMax((int64_t)value);
                    break;
                case 'm':   /* max integrale */
                    pc->printf("\nParametrage Coeff-Distance max integrale = %f", value);
                  //  cc->angle_regu.setMaxIntegral(value);
                    break;
                    
                    
                /* ------------------ coeffs Rampe distance  ------------------------ */
                
                 case 'A':
                    pc->printf("\nParametrage rampe-Distance derivee 1ier pos = %lf", value);
                    cc->dist_regu.setDerivee1ier( value, cc->dist_regu.getDerivee1ierNeg() );
                    break;
                case 'B':
                    pc->printf("\nParametrage rampe-Distance derivee 1ier neg = %lf", value);
                    cc->dist_regu.setDerivee1ier( cc->dist_regu.getDerivee1ierPos() , value);
                    break;
                case 'C':
                    pc->printf("\nParametrage rampe-Distance derivee 2ier pos AVANT=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAv( (int64_t)value, cc->dist_regu.getDerivee2ndNegAv());
                    break;
                case 'D':   
                    pc->printf("\nParametrage rampe-Distance derivee 2ier neg AVANT=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAv(  cc->dist_regu.getDerivee2ndPosAv() , (int64_t)value);
                    break;
                case 'E':
                    pc->printf("\nParametrage rampe-Distance derivee 2ier pos ARRIERE=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAr( (int64_t)value, cc->dist_regu.getDerivee2ndNegAr());
                    break;
                case 'F':   
                    pc->printf("\nParametrage rampe-Distance derivee 2ier neg ARRIERE=  %ld", (int64_t)value);
                    cc->dist_regu.setDerivee2ndAr(  cc->dist_regu.getDerivee2ndPosAr() , (int64_t)value);
                    break;
                case 'G':
                    pc->printf("\nParametrage rampe-Distance gain anticipation AVANT=  %f", value);
                    cc->dist_regu.setCoeffGainAnticipationAv( value);
                    break;
                case 'H':   
                    pc->printf("\nParametrage rampe-Distance gain anticipation ARRIERE=  %f",value);
                    cc->dist_regu.setCoeffGainAnticipationAr(value);
                case 'I':   
                    pc->printf("\nParametrage rampe-Distance taille fenetre arrivee=  %f", (double)value);
                    cc->dist_regu.setTailleFenetreArrivee(  (double)value);
                    break;
              /*  case 'A':
                    pc->printf("\nParametrage rampe-Distance min distance deceleration avant = %ld", (int64_t)value);
                    cc->dist_regu.setMinDecelerationDistForward((int64_t) value);
                    break;
                case 'B':
                    pc->printf("\nParametrage rampe-Distance max distance deceleration avant = %ld", (int64_t)value);
                    cc->dist_regu.setMaxDecelerationDistForward( (int64_t)value);
                    break;
                case 'C':
                    pc->printf("\nParametrage rampe-Distance coeff deceleration avant = %f", (double)value);
                    cc->dist_regu.setDecelerationCoeffForward((double)value);
                    break;
                case 'D':   
                    pc->printf("\nParametrage rampe-Distance pas maximum avant = %f", (double)value);
                    cc->dist_regu.setMaxStepForward( (double)value);
                    break;
                case 'E':
                    pc->printf("\nParametrage rampe-Distance vitesse deceleration minimum avant = %ld", (int64_t)value);
                    cc->dist_regu.setMinDecelerationSpeedForward((int64_t)value);
                    break;
                case 'F': 
                    pc->printf("\nParametrage rampe-Distance mon distance deceleration arriere = %ld", (int64_t)value);
                    cc->dist_regu.setMinDecelerationDistBackward((int64_t)value);
                    break;

                case 'G':  
                    pc->printf("\nParametrage rampe-Distance distance deceleration max arriere = %ld", (int64_t)value);
                    cc->dist_regu.setMaxDecelerationDistBackward((int64_t)value);
                    break;
                case 'H':
                    pc->printf("\nParametrage rampe-Distance coeff deceleration arriere = %f", (double)value);
                    cc->dist_regu.setDecelerationCoeffBackward((double)value);
                    break;
                case 'I':
                    pc->printf("\nParametrage rampe-Distance pas max arriere = %f", (double)value);
                    cc->dist_regu.setMaxStepBackward((double)value);
                    break;
                case 'J':
                    pc->printf("\nParametrage rampe-Distance vitesse d&#65533;celeration mini arriere = %ld", (int64_t)value);
                    cc->dist_regu.setMinDecelerationSpeedBackward( (int64_t)value);
                    break;
                    */
                    
                    
                /* ------------------ coeffs Rampe angle  ------------------------ */
                case 'J':
                    pc->printf("\nParametrage rampe-Angle derivee 1ier pos = %lf", value);
                    cc->angle_regu.setDerivee1ier( value, cc->angle_regu.getDerivee1ierNeg() );
                    break;
                case 'K':
                    pc->printf("\nParametrage rampe-Angle derivee 1ier neg = %lf", value);
                    cc->angle_regu.setDerivee1ier( cc->angle_regu.getDerivee1ierPos() , value);
                    break;
                case 'L':
                    pc->printf("\nParametrage rampe-Angle derivee 2ier pos AVANT=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAv( (int64_t)value, cc->angle_regu.getDerivee2ndNegAv());
                    break;
                case 'M':   
                    pc->printf("\nParametrage rampe-Angle derivee 2ier neg AVANT=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAv(  cc->angle_regu.getDerivee2ndPosAv() , (int64_t)value);
                    break;
                case 'N':
                    pc->printf("\nParametrage rampe-Angle derivee 2ier pos ARRIERE=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAr( (int64_t)value, cc->angle_regu.getDerivee2ndNegAr());
                    break;
                case 'O':   
                    pc->printf("\nParametrage rampe-Angle derivee 2ier neg ARRIERE=  %ld", (int64_t)value);
                    cc->angle_regu.setDerivee2ndAr(  cc->angle_regu.getDerivee2ndPosAr() , (int64_t)value);
                    break;
                case 'P':
                    pc->printf("\nParametrage rampe-Angle gain anticipation AVANT=  %f", (double)value);
                    cc->angle_regu.setCoeffGainAnticipationAv( value);
                    break;
                case 'Q':   
                    pc->printf("\nParametrage rampe-Angle gain anticipation ARRIERE=  %f", value);
                    cc->angle_regu.setCoeffGainAnticipationAr( value);
                case 'R':   
                    pc->printf("\nParametrage rampe-Angle taille fenetre arrivee=  %ld", (int64_t)value);
                    cc->angle_regu.setTailleFenetreArrivee(  (int64_t)value);
                    break;
                case 'W': // sauvegarde 
                    udp_data_save();
                    break;
                
                /*
                case 'K':
                    pc->printf("\nParametrage rampe-angle min distance deceleration avant = %ld", (int64_t)value);
                    cc->angle_regu.setMinDecelerationDistForward((int64_t) value);
                    break;
                case 'L':
                    pc->printf("\nParametrage rampe-angle max distance deceleration avant = %ld", (int64_t)value);
                    cc->angle_regu.setMaxDecelerationDistForward( (int64_t)value);
                    break;
                case 'M':
                    pc->printf("\nParametrage rampe-angle coeff deceleration avant = %f", (double)value);
                    cc->angle_regu.setDecelerationCoeffForward((double)value);
                    break;
                case 'N':   
                    pc->printf("\nParametrage rampe-angle pas maximum avant = %f", (double)value);
                    cc->angle_regu.setMaxStepForward( (double)value);
                    break;
                case 'O':
                    pc->printf("\nParametrage rampe-angle vitesse deceleration minimum avant = %ld", (int64_t)value);
                    cc->angle_regu.setMinDecelerationSpeedForward((int64_t)value);
                    break;
                case 'P': 
                    pc->printf("\nParametrage rampe-angle mon distance deceleration arriere = %ld", (int64_t)value);
                    cc->angle_regu.setMinDecelerationDistBackward((int64_t)value);
                    break;

                case 'Q':  
                    pc->printf("\nParametrage rampe-angle distance deceleration max arriere = %ld", (int64_t)value);
                    cc->angle_regu.setMaxDecelerationDistBackward((int64_t)value);
                    break;
                case 'R':
                    pc->printf("\nParametrage rampe-angle coeff deceleration arriere = %f", (double)value);
                    cc->angle_regu.setDecelerationCoeffBackward((double)value);
                    break;
                case 'S':
                    pc->printf("\nParametrage rampe-angle pas max arriere = %f", (double)value);
                    cc->angle_regu.setMaxStepBackward((double)value);
                    break;
                case 'T':
                    pc->printf("\nParametrage rampe-angle vitesse d&#65533;celeration mini arriere = %ld", (int64_t)value);
                    cc->angle_regu.setMinDecelerationSpeedBackward( (int64_t)value);
                    break;
                
                    
               /* ------------------ CONSIGNES  ------------------------ */  
                case '0':   /* Ajout consigne de Distance */
                    pc->printf("\n Distance += %d", (int)value);
                    cc->add_dist_consigne(odo->mmToUO( value ));
                    
                    
                    
                    break;
                case '1':   /* Ajout consigne d'Angle */
                    pc->printf("\n Angle += %d", (int)value);
                    cc->add_angle_consigne(odo->degToUO( value ));
                    break;
            }
            break;
    }
 }

EthernetNetIf* loadIPFromConf() {
    int ip[4], mask[4], gateway[4], dns[4];
    EthernetNetIf* eth;
    
    LocalFileSystem local("conf");
    
    /* display information -> PC */
    Serial pc(USBTX, USBRX);
    pc.baud(115200);
    pc.printf("\n----Load IP.CF----");
    /* open file eth.conf */
    FILE *fp = fopen("/conf/ip.cf", "r");
    if(!fp) {
        pc.printf("\n----IP.CF could not be opened!----");
        pc.printf("\n----Default configuration----");
        eth = new EthernetNetIf(  IpAddr(192,168,0,2), //IP Address
                                  IpAddr(255,255,255,0), //Network Mask
                                  IpAddr(192,168,0,1), //Gateway
                                  IpAddr(192,168,0,1)  //DNS
                                );
    } else {
        /* read ip configuration */
        if(fscanf(fp, "ip = %d.%d.%d.%d\n", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3])) != 4) {
            pc.printf("\nload IP error, apply default ip (192.168.10.2)");
            ip[0] = 192; ip[1] = 168; ip[2] = 0; ip[3] = 2;
        }
        if(fscanf(fp, "mask = %d.%d.%d.%d\n", &(mask[0]), &(mask[1]), &(mask[2]), &(mask[3])) != 4) {
            pc.printf("\nload mask error, apply default mask (255.255.255.0)");
            ip[0] = 255; ip[1] = 255; ip[2] = 255; ip[3] = 0;
        }
        if(fscanf(fp, "gateway = %d.%d.%d.%d\n", &(gateway[0]), &(gateway[1]), &(gateway[2]), &(gateway[3])) != 4) {
            pc.printf("\nload gateway error, apply default gateway (192.168.0.1)");
            ip[0] = 192; ip[1] = 168; ip[2] = 0; ip[3] = 1;
        }
        if(fscanf(fp, "dns = %d.%d.%d.%d\n", &(dns[0]), &(dns[1]), &(dns[2]), &(dns[3])) != 4) {
            pc.printf("\nload dns error, apply default dns (192.168.0.1)");
            ip[0] = 192; ip[1] = 168; ip[2] = 0; ip[3] = 1;
        }
        
        /* set configuration */
        eth = new EthernetNetIf( IpAddr(ip[0],ip[1],ip[2],ip[3]), //IP Address
                                 IpAddr(mask[0],mask[1],mask[2],mask[3]),  //Network Mask
                                 IpAddr(gateway[0],gateway[1],gateway[2],gateway[3]),  //Gateway
                                 IpAddr(dns[0],dns[1],dns[2],dns[3])  //DNS
                                );
        /* close file */
        fclose(fp);
    }
    /* display current configuration */
    pc.printf("\n----IP = %d.%d.%d.%d----\n", eth->getIp()[0], eth->getIp()[1], eth->getIp()[2], eth->getIp()[3]);
    
    /* return EthernetNetIf object */
    return eth;
}
