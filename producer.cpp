#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>
#include "common.h"

int main() {
    // 1. Création de la mémoire partagée (SHM)
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(RingBuffer));
    
    // 2. Mapping de la SHM dans l'espace d'adressage du processus
    void* ptr = mmap(0, sizeof(RingBuffer), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    RingBuffer* buffer = (RingBuffer*)ptr;

    // Initialisation des pointeurs (Head/Tail)
    // En production, on vérifierait si c'est déjà initialisé
    buffer->head.store(0);
    buffer->tail.store(0);

    std::cout << "[TRADER] Démarrage du moteur haute fréquence..." << std::endl;

    // 3. Boucle d'écriture (Simulation de Trading)
    for (int i = 0; i < 10000; ++i) {
        uint64_t current_head = buffer->head.load(std::memory_order_relaxed);
        uint64_t current_tail = buffer->tail.load(std::memory_order_acquire);

        // Vérifier si le buffer est plein (Head a rattrapé Tail)
        if (current_head - current_tail >= BUFFER_SIZE) {
            // Buffer plein : on doit attendre (ou dropper le paquet)
            // Dans un vrai HFT, on ne dort pas, on écrase ou on dérive.
            std::this_thread::yield(); 
            i--; // On réessaie le même message
            continue;
        }

        // ÉCRITURE (Sans verrou !)
        // On utilise le modulo (%) pour tourner en rond dans le tableau
        int index = current_head % BUFFER_SIZE;
        
        buffer->messages[index].id = i;
        buffer->messages[index].timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        buffer->messages[index].price = 150.0 + (i * 0.01);
        std::strcpy(buffer->messages[index].symbol, "AAPL");

        // On publie le message en avançant la tête
        buffer->head.store(current_head + 1, std::memory_order_release);

        if (i % 1000 == 0) std::cout << "[TRADER] 1000 ordres envoyés..." << std::endl;
        
        // Simulation de rapidité (très courte pause)
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    std::cout << "[TRADER] Fin de la session de trading." << std::endl;
    return 0;
}