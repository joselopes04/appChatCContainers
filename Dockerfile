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

# 4. Copiar o teu ficheiro hello.c para dentro da imagem
COPY hello.c .

# 5. Compilar o ficheiro C
RUN gcc -o hello hello.c

# 6. Correr o programa ao iniciar o contentor
CMD ["./hello"]