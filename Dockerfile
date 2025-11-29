FROM gcc:latest

WORKDIR /app

COPY . .

# Compilation des deux programmes avec la librairie rt (Real Time)
RUN g++ -o producer producer.cpp -lrt -pthread -std=c++17
RUN g++ -o consumer consumer.cpp -lrt -pthread -std=c++17

# Script pour lancer les deux en parallèle
RUN echo '#!/bin/bash\n./consumer & \nsleep 1\n./producer \nwait' > run.sh && chmod +x run.sh

CMD ["./run.sh"]