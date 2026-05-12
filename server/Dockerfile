# 1. Define a imagem base (Ubuntu) para criar o ambiente do servidor
FROM ubuntu:latest

# 2. Atualiza os repositórios e instala o compilador GCC e bibliotecas essenciais
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Define a pasta principal de trabalho dentro do container
WORKDIR /app

# 4. Copia os ficheiros necessários.
# Como o build será iniciado na raiz do projeto, indicamos o caminho relativo.
# Copiamos o server.c que está na pasta /server e os ficheiros common que estão na raiz.
COPY server/server.c .
COPY common.c .
COPY common.h .

# 5. Compila o servidor
# Agora o GCC encontra todos os ficheiros na pasta atual (/app) do container
RUN gcc server.c common.c -pthread -o server

# 6. Informa ao Docker que este container vai "ouvir" na porta 8080
EXPOSE 8080

# 7. Comando que executa o servidor
CMD ["./server"]