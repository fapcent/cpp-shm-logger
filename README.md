# 💾 Zero-Copy Shared Memory Logger

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white) ![IPC](https://img.shields.io/badge/IPC-Shared%20Memory-green) ![Lock-Free](https://img.shields.io/badge/Concurrency-Lock--Free-orange)

Une implémentation de système de logging ultra-faible latence utilisant la **Mémoire Partagée (POSIX SHM)** pour découpler le chemin critique du trading des opérations d'I/O lentes.

## 🎯 Le Problème Résolu
Dans les systèmes critiques, écrire sur le disque (std::cout ou printf) est une opération bloquante et lente.
**Solution :** Le Trader écrit dans un buffer circulaire en RAM. Un processus Logger séparé vide ce buffer et écrit sur le disque.

## ⚙️ Architecture Technique

1.  **IPC (Inter-Process Communication) :** Utilisation de \mmap\ et \shm_open\ pour partager une zone de RAM entre deux processus distincts.
2.  **Lock-Free Ring Buffer :** Utilisation de \std::atomic\ pour gérer les pointeurs \head\ et \	ail\ sans utiliser de Mutex (verrous), éliminant les changements de contexte coûteux.
3.  **Busy Spinning vs Yielding :** Stratégies d'attente optimisées pour réduire la latence CPU.

## 🚀 Démo

\\\ash
# Lancer le Producteur et le Consommateur
docker build -t shm-logger .
docker run --rm --ipc=host shm-logger
\\\

## 🧬 Structure de Données
Le buffer utilise une structure alignée en mémoire pour éviter le *false sharing* et maximiser l'efficacité du cache CPU.
