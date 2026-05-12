# 1. Define a base do sistema
FROM ubuntu:latest

# 2. Instala as ferramentas necessárias
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Cria e entra na pasta /app dentro do container
WORKDIR /app

# 4. Copia o código específico do cliente e os ficheiros partilhados
# Nota: Os caminhos partem da raiz do projeto onde vais correr o build
COPY client/client.c .
COPY common.c .
COPY common.h .

# 5. Compila o código dentro do container
# Adicionamos o common.c à compilação para que as funções exportadas funcionem
RUN gcc client.c common.c -pthread -o client

# 6. Comando de execução
CMD ["./client"]