# CONTEXTE DU PROJET : Distributeur de Médicaments Automatisé

## 1. Informations Générales

- **Domaine :** Génie Électrique et Informatique Industrielle (PFE)
- **Objectif :** Conception, câblage et programmation d'un distributeur de pilules sécurisé et autonome.
- **Microcontrôleur :** Carte Arduino (Uno/Nano).

## 2. Architecture Matérielle (Hardware)

- **Horloge RTC DS3231 :** Bus I2C (Pins SDA/A4, SCL/A5). Suivi autonome du temps.
- **Écran LCD 16x2 I2C :** Bus I2C (Pins SDA/A4, SCL/A5). Affichage HH:MM:SS et JJ/MM/AAAA.
- **Moteur Pas-à-Pas 28BYJ-48 + Driver ULN2003 :** Pins Arduino D8, D10, D9, D11. Rotation du carrousel (2048 pas/tour).
- **Capteur Infrarouge TCRT5000 :** Pin D4 (Digital Out). Détection du retrait du médicament (Logique : LOW = pris/absent).
- **Avertisseurs :** Buzzer actif sur Pin D3 (Sonore), LED avec résistance 220Ω sur Pin D13 (Visuel).
- **Bouton Poussoir :** Pin D2 (INPUT_PULLUP). Validation manuelle de secours.

## 3. Conception Mécanique

- **Carrousel :** 6 compartiments au total (5 pour les jours de soins + 1 vide servant de "Point Zéro" sécurisé).
- **Cinématique :** Le passage d'un compartiment à l'autre nécessite une rotation de 60 degrés, soit exactement 341 pas moteur (2048 / 6).

## 4. Logique de Fonctionnement et Algorithme

Le cycle se divise en 5 étapes strictes :

1. **État de Veille :** Le carrousel est sur le Point Zéro (fermé). L'écran affiche l'heure et la date en temps réel.
2. **Déclenchement :** À l'heure programmée (ex: 12:12:00), le moteur avance de 341 pas (60 degrés) pour présenter la pilule.
3. **Temporisation de Sécurité :** Une boucle `millis()` de 10 secondes s'active. La LED et le buzzer clignotent (intervalles de 200ms). L'Arduino scrute le TCRT5000 et le bouton.
4. **Bilan de la Prise :** - *Succès :* Si capteur passe à LOW ou bouton pressé < 10s -> Arrêt des alarmes, affichage "STATUT: PRIS".
   - *Échec :* Si délai > 10s sans action -> Arrêt des alarmes (anti-surchauffe), affichage "STATUT: OMIS".
5. **Mise en Sécurité :** Le moteur recule systématiquement de -341 pas (-60 degrés) pour revenir au Point Zéro.

## 5. Directives pour l'Agent IA

- **Code :** Langage C++ (Arduino IDE). Utilisation des bibliothèques `Wire.h`, `RTClib.h`, `LiquidCrystal_I2C.h`, et `Stepper.h`.
- **Contrainte :** Ne pas utiliser la fonction `delay()` pour les temporisations de sécurité afin de ne pas bloquer la lecture du capteur (utiliser `millis()`).
