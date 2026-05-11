FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY server.c .

# O servidor agora também leva a flag -pthread 
RUN gcc server.c -pthread -o server

EXPOSE 8080

CMD ["./server"]