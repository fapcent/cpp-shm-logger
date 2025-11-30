# 🕸️ HFT Network Sniffer (Raw Sockets)

![Python](https://img.shields.io/badge/python-3670A0?style=flat&logo=python&logoColor=ffdd54) ![Network](https://img.shields.io/badge/Network-TCP%2FIP-blue) ![Low Level](https://img.shields.io/badge/Low-Level-red)

Un analyseur de trafic réseau passif conçu pour monitorer la latence et les micro-bursts en contournant les abstractions de haut niveau.

## 📡 Fonctionnalités

* **Raw Sockets (AF_PACKET) :** Interception directe des trames Ethernet au niveau de la carte réseau (NIC).
* **Binary Parsing :** Décodage manuel des en-têtes Ethernet, IP et TCP (via \struct.unpack\).
* **Micro-burst Detection :** Identification en temps réel des pics de trafic anormaux pouvant saturer la bande passante HFT.

## 🛠 Stack Technique

* **Langage :** Python (Optimisé sans librairies externes lourdes).
* **OS Access :** Privileged Docker Container pour l'accès direct aux interfaces réseau.
* **Protocole :** Analyse approfondie des flags TCP (SYN, ACK, PSH) pour mesurer la santé des connexions.

## 🚀 Utilisation

\\\ash
# Nécessite les droits privilégiés pour ouvrir un Raw Socket
docker-compose up --build
\\\

## 🎯 Objectif
Démontrer la compréhension de la pile TCP/IP et la capacité à construire des outils de diagnostic réseau sur mesure pour des environnements à contraintes fortes.
