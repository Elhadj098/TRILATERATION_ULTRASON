==============================
PROJET ARDUINO : Trilateration 2D avec Capteurs à Ultrasons
==============================

Ce projet permet de localiser une cible en 2D à l'aide de trois capteurs à ultrasons fixes, un algorithme de trilatération, un servomoteur qui s'oriente vers la cible, et un écran LCD RGB qui affiche ses coordonnées.

------------------------------
MATÉRIEL UTILISÉ
------------------------------
– 1 Arduino UNO ou MEGA  
– 3 capteurs à ultrasons HC-SR04  
– 1 servomoteur (ex: SG90)  
– 1 écran LCD RGB compatible Grove (Adafruit / Seeed Studio)  
– Câbles de connexion Dupont  
– Breadboard (facultatif, pour faciliter le câblage)  
– Résistances si nécessaire pour protéger les entrées  

------------------------------
CÂBLAGE DES CAPTEURS
------------------------------
Capteur 1 (US1) :  
- TRIG : pin D9  
- ECHO : pin D10  

Capteur 2 (US2) :  
- TRIG : pin D11  
- ECHO : pin D12  

Capteur 3 (US3) :  
- TRIG : pin D3  
- ECHO : pin D2  

Servomoteur :  
- Signal : pin D6  
- VCC : 5V  
- GND : GND  

Écran LCD RGB :  
- Connecté via I2C (SDA/SCL)

------------------------------
ÉTAPES POUR COMPILER ET TÉLÉVERSER
------------------------------
1. Ouvrir le fichier `.ino` dans l'IDE Arduino.  
2. Aller dans **Outils > Type de carte** et sélectionner votre carte (UNO ou MEGA).  
3. Sélectionner le bon port COM.  
4. Vérifier que les bibliothèques nécessaires sont installées (voir ci-dessous).  
5. Cliquer sur **Vérifier** puis **Téléverser**.  

------------------------------
PARAMÈTRES AJUSTABLES
------------------------------
– `iteration` : nombre de mesures pour la moyenne (par défaut 100)  
– `distanceMax` : distance maximale à considérer pour la trilatération (par défaut 60 cm)  
– `offset` : décalage angulaire pour le servomoteur (par défaut 219)  
– `delayTime` : délai entre deux cycles de mesure (par défaut 500 ms)  

Ces paramètres peuvent être modifiés dans les premières lignes du code.

------------------------------
BIBLIOTHÈQUES NÉCESSAIRES
------------------------------
– `rgb_lcd.h` : pour l’écran LCD RGB  
– `Servo.h` : pour le contrôle du servomoteur  

Ces bibliothèques sont disponibles via le **Gestionnaire de bibliothèques** dans l’IDE Arduino :
- Aller dans **Croquis > Inclure une bibliothèque > Gérer les bibliothèques**
- Rechercher et installer **rgb_lcd** et **Servo**

------------------------------
REMARQUE
------------------------------
Le projet repose sur une disposition fixe des capteurs et suppose que la cible est réfléchissante aux ultrasons. Des erreurs de mesure peuvent apparaître si l’environnement est bruyant ou si la surface n’est pas adaptée à l’écho.

