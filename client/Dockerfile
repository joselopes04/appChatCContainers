# 1. Define a base do sistema (estás a usar um Ubuntu "limpo")
FROM ubuntu:latest
# 2. Instala as ferramentas necessárias (gcc para compilar o código C)
# O rm -rf serve para apagar ficheiros temporários e tornar a imagem mais leve
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*
# 3. Cria e entra na pasta /app dentro do container
WORKDIR /app

# 4. Copia o teu código fonte (client.c) do teu PC para dentro do container
COPY client.c .

# 5. Compila o código dentro do container
# A flag -pthread é fundamental porque o enunciado valoriza o uso de threads
RUN gcc client.c -pthread -o client
# 6. Define o comando que arranca automaticamente quando o container ligar
CMD ["./client"]