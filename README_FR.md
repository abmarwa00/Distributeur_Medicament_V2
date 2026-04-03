# 💊 MédSmart V2 — Guide d'Utilisation Professionnel (V2.4)

Bienvenue dans le projet **MédSmart V2**, un distributeur de médicaments intelligent conçu pour optimiser l'observance thérapeutique avec une fiabilité maximale.

Ce document fournit un guide clair pour l'utilisateur final, quel que soit son niveau technique.

---

## 🏗️ Architecture du Système

Le projet est composé de deux briques essentielles qui travaillent de concert :

### 1. Le Cerveau : `Distributeur_Medicament_V2.ino`
C'est le programme résidant dans la carte Arduino au cœur de la machine.
- **Rôle** : Contrôle des moteurs, gestion du buzzer/LED, et surveillance du capteur de retrait de pilule.
- **Autonomie** : Une fois programmé, il fonctionne de manière autonome pour distribuer les pilules chaque jour à l'heure exacte.

### 2. Le Tableau de Bord : `index2.html`
C'est l'interface web intuitive que vous ouvrez dans votre navigateur (Chrome / Edge).
- **Rôle** : Visualisation en temps réel de l'état de la machine, programmation des horaires, et suivi historique.
- **Simplicité** : Aucune installation de serveur n'est requise. Il suffit d'ouvrir le fichier.

---

## 🚀 Mise en Route Rapide

1. **Préparation** : Téléversez `Distributeur_Medicament_V2.ino` sur votre Arduino via le câble USB.
2. **Connexion** : Ouvrez `index2.html`, cliquez sur **"Connecter Arduino"** et sélectionnez votre port USB.
3. **Synchronisation** : L'heure de l'interface et de la machine se synchronisent automatiquement.
4. **Programmation** : Réglez l'heure de votre traitement dans "Configuration Alarme" et validez.

---

## ✨ Fonctionnalités Exceptionnelles

*   **Alarme Quotidienne Ultra-Fiable** : Le déclenchement se base uniquement sur l'horloge interne (Heure/Minute), garantissant que l'alarme sonnera chaque jour même en cas d'erreur de date calendrier.
*   **Alignement de Précision** : Le moteur est calibré (1125 pas) pour s'arrêter exactement au centre de chaque compartiment.
*   **Alerte Email Professionnelle** : Recevez une notification instantanée (via EmailJS) dès que :
    - Le médicament est distribué.
    - Le patient retire sa dose.
    - **URGENT** : Une dose est oubliée (non retirée après 10s).
*   **Historique Intelligent** : Toutes les prises sont enregistrées avec l'heure précise pour un suivi médical rigoureux.

---

## 🔐 Notes Spéciales (V2.4)

> [!IMPORTANT]
> **Pourquoi cette version ?**
> La V2.4 privilégie la **stabilité**. Elle utilise une alarme unique et un système de notification par email direct pour éliminer les risques de défaillance réseau ou mécanique.
> **Note** : Le support WhatsApp a été retiré pour simplifier l'utilisation et supprimer la nécessité d'un serveur local.

---
*Projet développé dans le cadre d'un PFE - Avril 2026*
