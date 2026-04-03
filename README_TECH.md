# 🔬 Spécifications Techniques — MédSmart V2.4

Ce document détaille l'architecture matérielle et logicielle du projet **MédSmart V2**, un distributeur de médicaments intelligent optimisé pour une fiabilité maximale.

---

## 🏗️ Architecture Matérielle (Hardware)

Le système repose sur un microcontrôleur **Arduino** raccordé aux composants suivants :

*   **Microcontrôleur** : Arduino (Uno/Nano/Mega).
*   **Moteur Pas à Pas** (28BYJ-48) : Piloté par une puce ULN2003 (Pins 8, 10, 9, 11).
    *   Résolution : 2048 pas par révolution.
    *   **Précision** : 1125 pas par compartiment pour garantir un alignement mécanique parfait.
*   **Horloge Temps Réel (RTC DS3231)** : Communication I2C pour maintenir l'heure système.
*   **Écran LCD 16x2 (I2C 0x27)** : Affichage du statut, de l'heure et de la date (Heure/Minute/Seconde).
*   **Capteur d'Obstacle (TCRT5000)** : Détection infrarouge du retrait du médicament avec timeout de 10s.
*   **Actionneurs** : Buzzer actif et LED témoin pour les alertes.

---

## 💻 Architecture Logicielle (Software)

### 1. Firmware Arduino (`.ino`)
Utilise une **Machine à États finis (FSM)** non-bloquante pour la gestion concurrente de l'affichage, des capteurs et de la communication série.

**Logique de l'Alarme (V2.4) :**
- **Déclenchement Temporel** : L'alarme compare uniquement `Hour` et `Minute`. Ceci élimine les échecs liés à des erreurs de calendrier sur le RTC et assure une répétition quotidienne sans intervention.
- **États** : `IDLE` (Veille), `DISPENSING` (Moteur), `WAITING` (Capteur), `CLOSING` (Fin de cycle).

### 2. Interface Web (`index2.html`)
Développée en **Vanilla JS** pour une performance maximale et une maintenance simplifiée.
- **Web Serial API** : Échange de données direct avec l'Arduino sans serveur local.
- **Notification Engine** : Moteur Email professionnel intégré via **EmailJS**. Les modèles sont conçus pour être clairs, riches et professionnels (sans emojis).
- **Synchronisation** : Boutons "Test 1 min" synchronisés avec l'heure réelle rapportée par l'Arduino pour une validation précise.

---

## 📡 Protocole de Communication Serial (9600 bauds)

### Commandes (Web ➡️ Arduino)
| Commande | Usage |
| :--- | :--- |
| `STATUS` | Demande un rapport complet. |
| `DISPENSE` | Force une distribution immédiate. |
| `SET_TIME:H:M:S:D:M:Y` | Synchronise le RTC. |
| `SET_ALARM:D:M:Y:H:M:A` | Alarme unique (`A`=1 pour actif). L'Arduino gère la répétition quotidienne. |
| `TEST_MOTOR` / `BUZZ_TEST` | Tests matériels isolés. |

### Rapports (Arduino ➡️ Web)
| Format | Description |
| :--- | :--- |
| `TIME:XX|DATE:XX|ALM:XX...` | Trame d'état complète toutes les 2 secondes. |
| `STATUS:ALARM_SET` | Confirmation de programmation d'alarme. |
| `STATUS:PILL_TAKEN` | Médicament retiré avec succès. |
| `STATUS:PILL_MISSED` | Timeout de 10s atteint (Pilule non retirée). |

---

## 🔐 Sécurité et Déploiement

### Dépendances :
- Arduino : `RTClib`, `LiquidCrystal_I2C`, `Stepper`.
- Web : Aucune dépendance externe locale (nécessite juste une connexion Internet pour EmailJS).

### Nouveauté V2.4 :
- **Architecture Sans Serveur** : Suppression de `server.js` et de Twilio pour réduire les points de défaillance. Le système est maintenant entièrement autonome côté client.

---
*Projet développé dans le cadre d'un PFE - Avril 2026*
