#ifndef ODOMETRIE
#define ODOMETRIE

#include "Codeurs/Codeur.h"


#define PI 3.14159265358979

class Odometrie
{

    public:
        //Constructeur
        Odometrie(PinName PinChanACoderG , PinName PinChanBCoderG , PinName PinChanACoderD , PinName PinChanBCoderD);
        
        void resetX() {    x = 0; }
        void resetY() {     y = 0; }
        void resetTheta() {   theta = 0;}
        
        // Mise a jour ...
        void refresh();
        
        //Position en unites arbritraires ( Unite Odometrie [UO] pour les distances, et rad pour l'angle)
        int64_t getX() {return x;}
        int64_t getY() {return y;}
        double getTheta() {return theta;}
        
        // Position en unites 'humaines'
        int64_t getXmm() { return x/(frontParMetre*uOParFront)*1000; }
        int64_t getYmm() { return y/(frontParMetre*uOParFront)*1000; }
        double getThetaDeg() { return theta*180/PI; }
        
        // Calcul de position et/ou d'angle pratiques
        int64_t mmToUO(float mm) { return (int64_t)(mm/1000*frontParMetre*uOParFront); }
        int64_t degToUO(float deg) { return (PI * deg/180) * distanceRouesUO; }
        int64_t radToUO(float rad) { return rad *  distanceRouesUO; }
        float UOtoMM(int64_t uo) { return uo/(frontParMetre*uOParFront)*1000; }
        float UOtoDeg(int64_t uo) { return (uo*180/PI)/distanceRouesUO; }
        
        // Variation de la position depuis la derniere mise a jour
        int64_t getDeltaThetaBrut() { return diffCount; }
        double getDeltaTheta() { return deltaTheta; }
        int64_t getDeltaDist() { return deltaDist; }
        
        //configuration
        void setFrontParMetre(double codeurGFPM, double codeurDFPM);
        void setDistRouesMetre(double distRoues); 
        void setUOParFront(double newUOParFront);
        
        //Inversion logicielle des codeurs
        void inverseCodeurDroit() {inverseCodeurD=!inverseCodeurD; } //sens de rotation inverse pour le codeur droit
        void inverseCodeurGauche() {inverseCodeurG=!inverseCodeurG; } //idem, pour le codeur gauche
        void echangeCodeurs() {swapCodeurs=!swapCodeurs; } // codeurGauche <-> codeurDroit
        
        //debug
        /*
        int64_t getCountD();
        int64_t getCountG();
        */
        
    private:
        //Instances des deux codeurs
        Codeur codeurG;
        Codeur codeurD;
        
        // donnees sur les codeurs
        double uOParFront;
        double frontParMetre;
        double distanceRouesFront;
        double distanceRouesUO;
        double distanceRouesMetre;
        double ratioCodeurs;
        bool applyRatioOnG;
        
        // Position actuelle 
        int64_t  x, y; //En UO
        double theta; //En radian
    
        // Variation pour la mise a jour en cours
        int64_t deltaDist; // En UO
        double diffCount; // EN Brut
        double deltaTheta; //En radian
        int64_t countG;
        int64_t countD;
        double R;
        
        //Booleens gerant une eventuelle inversion des codeurs
        bool inverseCodeurG;
        bool inverseCodeurD;
        bool swapCodeurs;
};

#endif