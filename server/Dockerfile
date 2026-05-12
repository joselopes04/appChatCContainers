# 1. Define a imagem base (Ubuntu) para criar o ambiente do servidor
FROM ubuntu:latest

# 2. Atualiza os repositórios e instala o compilador GCC e bibliotecas essenciais
# O 'rm -rf' limpa ficheiros temporários para manter o container leve
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*
# 3. Define a pasta principal de trabalho dentro do container
WORKDIR /app
# 4. Copia o código-fonte do servidor (server.c) para dentro do container
COPY server.c .

# 5. Compila o servidor
# A flag '-pthread' é usada aqui para que o servidor consiga gerir múltiplos 
# clientes (os 2 clientes pedidos) usando threads em simultâneo
RUN gcc server.c -pthread -o server

# 6. Informa ao Docker que este container vai "ouvir" na porta 8080
# Nota: Deves garantir que no teu server.c a porta definida é a mesma (8080)
EXPOSE 8080

# 7. Comando que executa o servidor assim que o container é iniciado
CMD ["./server"]