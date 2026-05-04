# 1. Usar um sistema Alpine leve e instalar as ferramentas para o C (gcc)
FROM alpine:latest
RUN apk add --no-cache gcc musl-dev

# 2. Criar uma pasta de trabalho lá dentro
WORKDIR /app

# 3. Copiar o teu ficheiro hello.c para dentro da bolha
COPY hello.c .

# 4. Compilar o ficheiro C
RUN gcc -o hello hello.c

# 5. Dizer à bolha para correr o programa quando for ligada
CMD ["./hello"]
