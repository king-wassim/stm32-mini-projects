Un timer (ou compteur/temporisateur) dans un microcontrôleur STM32 agit comme une horloge numérique très précise et polyvalente intégrée directement dans la puce.
Analogie Simplifiée

Imaginez le timer STM32 comme un chronomètre de cuisine sophistiqué ou un métronome programmable :
* Le Chronomètre : Vous pouvez lui dire de compter jusqu'à un certain temps (par exemple, 10 millisecondes) et de vous prévenir (déclencher une interruption) quand le temps est écoulé. C'est parfait pour savoir combien de temps une tâche a pris ou pour exécuter quelque chose à intervalles réguliers.
* Le Métronome : Vous pouvez aussi le régler pour qu'il émette un "clic" (un signal électrique) à une fréquence très précise (par exemple, 50 fois par seconde). Cela génère des signaux utiles pour contrôler la vitesse d'un moteur ou la luminosité d'une LED (PWM).
Fonctions Clés

Le timer est essentiellement un compteur qui :
* Compte le temps : Il incrémente un registre à chaque impulsion d'horloge du système.
* Déclenche des actions : Lorsqu'il atteint une valeur prédéfinie, il peut arrêter le comptage, recommencer depuis zéro, ou  *signaler au processeur qu'il doit exécuter une fonction spécifique (interruption).
*Génère des signaux : Il peut automatiquement changer l'état d'une broche de sortie (ON/OFF) selon un cycle prédéfini (PWM).
C'est un composant fondamental qui permet au microcontrôleur de gérer le temps et les événements externes avec une grande précision.

Supposons : PCLK1 = 50 MHz prescaler 
APB1 = /2
 → donc TIMxCLK = 2×PCLK1 = 100 MHz
 PSC = 99 Alors : 
𝐹 𝑐 𝑜 𝑢 𝑛 𝑡 𝑒 𝑟 = 100 𝑀 𝐻 𝑧 99 + 1 = 1 𝑀 𝐻 𝑧 
F counter ​ = 99+1 100MHz ​ =1MHz 
➡️ Le timer incrémente son compteur à 1 MHz (1 tick = 1 µs)

Si TIMxCLK = 8 MHz :
PSC = 7999
ARR = 999   (L'ARR détermine la valeur maximale que le compteur du timer) 999 car on commence par 0 (999-0+1 =1000ms=1s)
➡️ interruption (ou flag update) toutes les 1 seconde



