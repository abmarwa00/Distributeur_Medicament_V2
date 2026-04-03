<div align="center">

<img src="MedSmart%20V5.png" alt="MedSmart Logo" width="650">

# 💊 MédSmart V2 — L'Observance Thérapeutique Connectée

[![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![JavaScript](https://img.shields.io/badge/-JavaScript-F7DF1E?style=for-the-badge&logo=JavaScript&logoColor=black)](https://developer.mozilla.org/fr/docs/Web/JavaScript)
[![HTML5](https://img.shields.io/badge/-HTML5-E34F26?style=for-the-badge&logo=HTML5&logoColor=white)](https://developer.mozilla.org/fr/docs/Web/HTML)
[![CSS3](https://img.shields.io/badge/-CSS3-1572B6?style=for-the-badge&logo=CSS3&logoColor=white)](https://developer.mozilla.org/fr/docs/Web/CSS)
[![EmailJS](https://img.shields.io/badge/-EmailJS-FF6B6B?style=for-the-badge&logo=Gmail&logoColor=white)](https://www.emailjs.com/)

**Une solution intelligente, autonome et sécurisée pour garantir la prise régulière des médicaments.**

![Version](https://img.shields.io/badge/Version-2.4-blue?style=flat-square)
![Statut](https://img.shields.io/badge/Statut-Stable-brightgreen?style=flat-square)
![Licence](https://img.shields.io/badge/Licence-Projet%20PFE-orange?style=flat-square)
![Avril 2026](https://img.shields.io/badge/Avril-2026-purple?style=flat-square)

</div>

---

## 🌟 Présentation du Projet

**MédSmart V2** est un système de distribution automatique de médicaments intelligent, conçu pour améliorer l'autonomie des patients et la tranquillité d'esprit de leurs proches. Il associe un matériel embarqué fiable (Arduino) à une interface web de contrôle moderne, le tout sans nécessiter d'installation de serveur.

> [!IMPORTANT]
> **Pourquoi MédSmart ?**
> Selon l'OMS, la non-observance des traitements médicamenteux est responsable de 125 000 décès par an en Europe. MédSmart V2 élimine ce risque grâce à une alarme infaillible, une distribution mécanique précise et un système d'alerte en temps réel.

### Objectifs du projet

- ✅ Automatiser la distribution quotidienne de médicaments.
- ✅ Détecter la prise effective du médicament via un capteur infrarouge.
- ✅ Alerter immédiatement en cas de dose oubliée.
- ✅ Fournir un tableau de bord de suivi pour médecins et aidants.

---

## 🏗️ Architecture du Système

Le projet repose sur deux couches complémentaires qui communiquent en temps réel via le port USB :

<div align="center">
  <img src="architecture%20flux%20syst%C3%A8me.jpg.png" alt="Architecture du Système MédSmart" width="850">
  <br>
  <em>Schéma du flux de données entre le Firmware Arduino et le Dashboard Web</em>
</div>

```
┌─────────────────────────────────────────────────────────────────┐
│                        MEDSMART V2.4                            │
│                                                                 │
│   ┌───────────────┐   USB/Serial   ┌───────────────────────┐   │
│   │   FIRMWARE    │ ◄────────────► │    WEB DASHBOARD      │   │
│   │   Arduino     │   9600 bauds   │   (index2.html)       │   │
│   │               │                │   Web Serial API      │   │
│   │ RTC DS3231    │                │   Vanilla JS          │   │
│   │ Stepper Motor │                │                       │   │
│   │ LCD 16x2      │                │      ┌─────────┐      │   │
│   │ TCRT5000 IR   │                │      │ EmailJS │      │   │
│   │ Buzzer + LED  │                │      └────┬────┘      │   │
│   └───────────────┘                └──────────┼───────────┘   │
│                                               │                │
│                                    Notifications Email          │
└───────────────────────────────────────────────┼────────────────┘
                                                ▼
                                       📧 Patient / Médecin
```

| Couche | Technologie | Rôle |
| :--- | :--- | :--- |
| **Firmware** | C++ / Arduino IDE | Contrôle moteur, RTC, capteurs, affichage LCD |
| **Interface** | HTML5 / CSS3 / Vanilla JS | Tableau de bord, configuration, historique |
| **Communication** | Web Serial API (9600 bauds) | Échange de commandes et rapports d'état |
| **Notifications** | EmailJS | Alertes automatiques par email |

---

## ✨ Fonctionnalités Détaillées

### 🎯 Machine à États (Finite State Machine)

Le firmware utilise une FSM non-bloquante à 4 états pour garantir une réactivité maximale :

```
    ┌─────────┐    Alarme / DISPENSE     ┌────────────┐
    │  IDLE   │ ──────────────────────► │ DISPENSING │
    │  Veille │                          │  Rotation  │
    └─────────┘                          └─────┬──────┘
         ▲                                     │ Moteur OK
         │                               ┌─────▼──────┐
    Retour 0           Pilule prise /   │  WAITING   │
    Moteur  ◄─────── Timeout 10s  ──── │  Capteur   │
         │                               └────────────┘
    ┌────┴──────┐
    │  CLOSING  │
    │  Fermeture│
    └───────────┘
```

### 📋 Tableau des Fonctionnalités

| Fonctionnalité | Description | Technologie |
| :--- | :--- | :--- |
| 🕒 **Alarme Quotidienne** | Déclenchement à l'heure H/Min configurée, répété chaque jour | RTC DS3231 |
| ⚙️ **Précision Mécanique** | 1 125 pas par compartiment pour un alignement parfait | Stepper 28BYJ-48 |
| 🛡️ **Anti-Oubli** | Timeout de 10 secondes avec LED clignotante et buzzer | TCRT5000 + Buzzer |
| 📧 **Notification Email** | 3 types d'alertes (Distribution, Prise, Oubli) | EmailJS |
| 📊 **Historique** | Journal horodaté de toutes les actions avec code couleur | Vanilla JS |
| 🔌 **Web Serial** | Connexion directe USB sans driver ni serveur | Web Serial API |
| 🕹️ **Tests Hardware** | Validation isolée du Moteur, Buzzer et LED | Commandes série |

---

## 📧 Galerie des Notifications Email

MédSmart envoie automatiquement 3 types d'emails selon les événements système :

<table>
  <tr>
    <th align="center">⏰ Heure du Médicament</th>
    <th align="center">✅ Pilule Prise</th>
    <th align="center">🚨 Dose Manquée (URGENT)</th>
  </tr>
  <tr>
    <td align="center"><img src="Notification%20d'Alerte%20(Heure%20du%20M%C3%A9dicament).jpg" alt="Alerte" width="250"></td>
    <td align="center"><img src="Notification%20de%20Confirmation%20(Pilule%20Prise).jpg" alt="Confirmation" width="250"></td>
    <td align="center"><img src="Notification%20d'Urgence%20(Dose%20Manqu%C3%A9e).jpg" alt="Urgence" width="250"></td>
  </tr>
  <tr>
    <td align="center"><em>Envoyée lors de la distribution automatique</em></td>
    <td align="center"><em>Envoyée dès que le capteur IR détecte le retrait</em></td>
    <td align="center"><em>Envoyée après 10s si la pilule n'est pas retirée</em></td>
  </tr>
</table>

---

## 🔬 Spécifications Techniques

### ⚡ Configuration Matérielle (Hardware)

| Composant | Modèle | Interface |
| :--- | :--- | :--- |
| **Microcontrôleur** | Arduino Uno / Nano / Mega | — |
| **Moteur Pas à Pas** | 28BYJ-48 + Driver ULN2003 | Pins 8, 9, 10, 11 |
| **Horloge Temps Réel** | RTC DS3231 | I2C |
| **Écran LCD** | 16x2 I2C (Adresse 0x27) | I2C |
| **Capteur Infrarouge** | TCRT5000 | Pin 4 |
| **Buzzer** | Actif | Pin 3 |
| **LED Témoin** | Standard | Pin 13 |
| **Bouton Manuel** | Pull-up interne | Pin 2 |

### 📡 Protocole de Communication Série

**Paramètres** : `9600 bauds`, `8N1`, `USB`

#### Commandes (Dashboard → Arduino)
| Commande | Paramètres | Description |
| :--- | :--- | :--- |
| `STATUS` | — | Demande un rapport d'état immédiat |
| `DISPENSE` | — | Force une distribution manuelle |
| `SET_TIME` | `H:M:S:D:M:Y` | Synchronise l'horloge RTC |
| `SET_ALARM` | `D:M:Y:H:M:A` | Programme l'alarme (`A=1` pour activer) |
| `TEST_MOTOR` | — | Teste un aller-retour du moteur |
| `BUZZ_TEST` | — | Teste le buzzer (200ms) |
| `LED_TEST` | — | Teste la LED (500ms) |

#### Rapports (Arduino → Dashboard)
| Format | Description |
| :--- | :--- |
| `TIME:HH:MM:SS\|DATE:DD/MM/YYYY\|COMP:N\|ALM:...\|SENS:N` | Trame complète (toutes les 2s) |
| `STATUS:ALARM_SET` | Confirmation de l'enregistrement de l'alarme |
| `STATUS:DISPENSING` | Début de la rotation du moteur |
| `STATUS:WAITING_FOR_PILL` | En attente de retrait de la pilule |
| `STATUS:PILL_TAKEN` | Pilule retirée avec succès (capteur IR) |
| `STATUS:PILL_MISSED` | Timeout 10s atteint, pilule non retirée |
| `STATUS:READY` | Système prêt, retour en veille |

---

## 🚀 Guide d'Installation et d'Utilisation

### Prérequis
- [x] Arduino IDE installé ([Télécharger](https://www.arduino.cc/en/software))
- [x] Bibliothèques : `RTClib`, `LiquidCrystal_I2C`, `Stepper`
- [x] Navigateur Chrome ou Microsoft Edge (Web Serial API requis)
- [x] Compte EmailJS configuré ([emailjs.com](https://www.emailjs.com/))

### Étapes de Mise en Route

**Étape 1 — Flash du Firmware**
```
1. Ouvrir Arduino IDE
2. Installer les bibliothèques (Gestionnaire de bibliothèques)
3. Connecter l'Arduino en USB
4. Ouvrir Distributeur_Medicament_V2.ino
5. Sélectionner la bonne carte et le bon port
6. Cliquez sur "Téléverser"
```

**Étape 2 — Lancer l'Interface Web**
```
1. Ouvrir index2.html dans Chrome ou Edge
2. (Ne PAS utiliser Firefox — Web Serial API non supportée)
```

**Étape 3 — Connexion et Configuration**
```
1. Cliquer sur le bouton "Connecter Arduino" (en haut)
2. Sélectionner le port COM correspondant à votre Arduino
3. L'heure se synchronise automatiquement
4. Configurer l'heure d'alarme dans le panneau "Configuration Alarme"
5. Activer l'alarme via le toggle et valider
```

**Étape 4 — Vérification**
```
1. Utiliser "Test Moteur" pour valider l'alignement mécanique
2. Utiliser "Test Buzzer" et "Test LED" pour valider les actionneurs
3. Vérifier que les emails de notification arrivent correctement
```

> [!TIP]
> Pour tester rapidement, utilisez le bouton **"Test 1 min"** qui synchronise l'alarme sur la minute suivante pour un déclenchement immédiat.

> [!WARNING]
> La Web Serial API nécessite que le navigateur soit **Chrome ou Edge**. Firefox et Safari ne sont **pas compatibles**.

---

## 🧑‍🤝‍🧑 Équipe du Projet

<div align="center">

Ce projet a été conçu et développé dans le cadre d'un **Projet de Fin d'Études (PFE)** par :

<table>
  <tr>
    <td align="center" width="300">
      <br>
      <strong>👩‍💻 Marwa Abarkan</strong>
      <br>
      <em>Développeuse & Conceptrice</em>
      <br>
      <sub>Firmware Arduino · Interface Web · Système de Notifications</sub>
    </td>
    <td align="center" width="300">
      <br>
      <strong>👩‍💻 Imane Oumragh</strong>
      <br>
      <em>Développeuse & Conceptrice</em>
      <br>
      <sub>Architecture Matérielle · Intégration · Tests</sub>
    </td>
  </tr>
</table>

</div>

---

## 📁 Structure du Projet

```
Distributeur_Medicament_V2/
│
├── 📄 Distributeur_Medicament_V2.ino   → Firmware Arduino (FSM, Moteur, RTC, Série)
├── 🌐 index2.html                       → Dashboard Web (Web Serial, EmailJS, Historique)
├── 📧 emailTemplate.html                → Modèle d'email professionnel (EmailJS)
│
├── 🖼️ MedSmart V5.png                  → Logo du projet
├── 🖼️ architecture flux système.jpg    → Schéma d'architecture
├── 🖼️ Notification d'Alerte (...).jpg  → Capture notification alerte
├── 🖼️ Notification de Confirmation(...).jpg
├── 🖼️ Notification d'Urgence(...).jpg
│
├── 📚 README.md                         → Ce document
├── 📚 README_FR.md                      → Guide utilisateur simplifié
└── 📚 README_TECH.md                    → Spécifications techniques détaillées
```

---

<div align="center">

**MédSmart V2** — Projet de Fin d'Études

*Marwa Abarkan & Imane Oumragh — Avril 2026*

![GitHub last commit](https://img.shields.io/badge/Dernière%20MàJ-Avril%202026-informational?style=flat-square)

</div>
