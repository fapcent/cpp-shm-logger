#ifndef COMMON_H
#define COMMON_H

#include <atomic>
#include <cstdint>

// Nom de la zone mémoire partagée
#define SHM_NAME "/citadel_shm"
// Taille du buffer (doit être une puissance de 2 pour l'optimisation bitwise)
#define BUFFER_SIZE 1024 

struct LogMessage {
    uint64_t timestamp;
    int id;
    double price;
    char symbol[8];
};

// Structure du Ring Buffer qui vivra en mémoire partagée
struct RingBuffer {
    // std::atomic permet de modifier ces variables sans Mutex (Verrou)
    // C'est la clé de la performance "Lock-Free"
    std::atomic<uint64_t> head; // Où le Producteur écrit
    std::atomic<uint64_t> tail; // Où le Consommateur lit
    
    LogMessage messages[BUFFER_SIZE];
};

#endif