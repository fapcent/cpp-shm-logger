#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "common.h"

int main() {
    // 1. Ouverture de la mémoire partagée avec boucle de tentative (Retry Loop)
    int shm_fd = -1;
    while (true) {
        shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
        if (shm_fd != -1) {
            break; // Succès ! On sort de la boucle
        }
        // Échec : on attend un peu et on réessaie
        std::cout << "[LOGGER] En attente du Trader (SHM)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // 2. Mapping de la SHM
    void* ptr = mmap(0, sizeof(RingBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    RingBuffer* buffer = (RingBuffer*)ptr;

    std::cout << "[LOGGER] Système d'audit démarré. En attente de données..." << std::endl;

    // 3. Boucle de lecture
    while (true) {
        uint64_t current_tail = buffer->tail.load(std::memory_order_relaxed);
        uint64_t current_head = buffer->head.load(std::memory_order_acquire);

        // Si Head == Tail, le buffer est vide, rien à lire
        if (current_tail == current_head) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        // LECTURE
        int index = current_tail % BUFFER_SIZE;
        LogMessage msg = buffer->messages[index];

        // Simulation d'écriture disque
        std::cout << "[DISK WRITE] ID:" << msg.id 
                  << " | " << msg.symbol 
                  << " @ " << msg.price << std::endl;

        // On libère la place en avançant la queue
        buffer->tail.store(current_tail + 1, std::memory_order_release);
    }

    return 0;
}