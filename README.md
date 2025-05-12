# TRILATERATION_ULTRASON
TRILATERATION A BASE DE 3 CAPTEURS ULTRASONS
Projet Arduino - Trilatération avec capteurs à ultrasons
Ce projet utilise un Arduino pour effectuer une trilatération à l'aide de trois capteurs à ultrasons afin de déterminer la position d'une cible dans un espace 2D. Les résultats de la position sont ensuite affichés sur un écran LCD et un servomoteur est utilisé pour orienter l'Arduino en fonction de la position calculée.

Fonctionnalités
Trilatération avec 3 capteurs à ultrasons : Le système utilise trois capteurs à ultrasons pour mesurer les distances entre les capteurs et la cible.

Affichage LCD : Les coordonnées calculées sont affichées sur un écran LCD 16x2 pour une visualisation claire.

Contrôle du servomoteur : Un servomoteur ajuste l'orientation en fonction des coordonnées calculées pour suivre la cible.

Calcul de position moyen : Le code calcule la position moyenne sur plusieurs itérations pour une meilleure précision.

Matériel requis
Arduino (Uno, Mega ou autre compatible)

3 capteurs à ultrasons (HC-SR04 ou similaire)

1 servomoteur

1 écran LCD 16x2 RGB

Câbles de connexion

Alimentation pour Arduino

Schéma de connexion
Capteurs à ultrasons :

Le premier capteur est connecté aux pins TRIGGER_PIN1 (9) et ECHO_PIN1 (10).

Le deuxième capteur est connecté aux pins TRIGGER_PIN2 (11) et ECHO_PIN2 (12).

Le troisième capteur est connecté aux pins TRIGGER_PIN3 (3) et ECHO_PIN3 (2).

Servomoteur : Le servomoteur est connecté à la pin servoPin (6).

LCD : L'écran LCD est connecté via le bus I2C, ce qui permet une communication simplifiée avec l'Arduino.

Installation
Télécharger et installer l'IDE Arduino : Télécharge l'IDE Arduino depuis le site officiel.

Bibliothèques nécessaires :

rgb_lcd : Pour l'affichage LCD RGB.

Servo : Pour contrôler le servomoteur.

Wire : Pour la communication I2C avec l'écran LCD.

Tu peux installer ces bibliothèques via le gestionnaire de bibliothèques de l'IDE Arduino.

Téléverser le code : Ouvre l'IDE Arduino, colle le code dans un nouveau fichier, sélectionne ta carte et ton port, puis téléverse le programme.

Code Explication
Mesure des distances : Le programme utilise trois capteurs à ultrasons pour mesurer la distance entre chaque capteur et la cible. La fonction MesureUS gère cette tâche.

Trilatération : Les distances mesurées sont utilisées pour calculer les coordonnées (x, y) de la cible dans un espace 2D via la fonction DeterminerPosition.

Affichage et contrôle du servomoteur : Une fois la position calculée, elle est affichée sur un écran LCD et un servomoteur est ajusté pour orienter l'Arduino vers la cible.

Fonctionnement
Le programme commence par mesurer la distance avec chaque capteur à ultrasons.

Les distances mesurées sont utilisées pour résoudre un système d'équations permettant de déterminer les coordonnées de la cible.

Les coordonnées calculées sont affichées sur l'écran LCD et utilisées pour ajuster la position du servomoteur.

Le processus se répète à chaque itération, avec une mise à jour des coordonnées et de l'orientation.

Remarques
Précision : La précision de la trilatération dépend de la qualité des mesures des capteurs à ultrasons et de la configuration de l'espace.

Améliorations possibles : Il est possible d'ajouter des fonctionnalités supplémentaires, comme la prise en charge de plus de capteurs, l'amélioration de la précision avec des algorithmes plus complexes, ou l'ajout d'un système de compensation pour les erreurs de mesure.
