FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Atenção ao nome exato do teu ficheiro aqui! 
COPY client.c .

# A flag -pthread é obrigatória 
RUN gcc client.c -pthread -o client

CMD ["./client"]