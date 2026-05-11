#Cliente

# 1. Usar a imagem oficial do Ubuntu (versão estável mais recente)
FROM ubuntu:latest

# 2. Atualizar os repositórios e instalar o GCC
# No Ubuntu, o pacote 'build-essential' é o mais recomendado, 
# pois traz o gcc, g++ e as bibliotecas base (libc).
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Criar uma pasta de trabalho
WORKDIR /app

# Copiar o código fonte do servidor para dentro do container
COPY client.c .
RUN gcc client.c -o client

# O cliente não precisa de expor portas, só se liga.
CMD ["./client"]