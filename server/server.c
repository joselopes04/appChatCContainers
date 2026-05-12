#include <stdio.h> // Input/Output (printf)
#include <stdlib.h>
#include <string.h> // Funções com strings
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> // API dos sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> // Incluído da Ficha P3 

#include "./common.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1000

// Lista partilhada de sockets dos clientes
int clientes[MAX_CLIENTS];
int num_clientes = 0;

// Mutex para proteger o acesso ao array de clientes [cite: 346-347]
pthread_mutex_t mutex_clientes = PTHREAD_MUTEX_INITIALIZER;

// Função para enviar mensagem a todos os clientes conectados (exceto o remetente)
void broadcast(char* mensagem, int remetente_fd) {
    pthread_mutex_lock(&mutex_clientes); // Bloqueia para acesso exclusivo [cite: 348]
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i] != remetente_fd) {
            send(clientes[i], mensagem, strlen(mensagem), 0);
        }
    }
    pthread_mutex_unlock(&mutex_clientes); // Liberta o acesso [cite: 349]
}

void* tratar_cliente(void* arg) {
    int cli_fd = *(int*)arg;
    free(arg); // Liberta a memória alocada no main
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(cli_fd, buffer, BUFFER_SIZE, 0);
        
        if (n <= 0) {
            // Se o cliente desconectar, removemos do array
            pthread_mutex_lock(&mutex_clientes);
            for (int i = 0; i < num_clientes; i++) {
                if (clientes[i] == cli_fd) {
                    clientes[i] = clientes[num_clientes - 1]; // Substitui pelo último
                    num_clientes--;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex_clientes);
            close(cli_fd);
            printf("Um utilizador saiu. Clientes ativos: %d\n", num_clientes);
            fflush(stdout);
            return NULL;
        }
        
        // Espalha a mensagem recebida para todos os outros
        broadcast(buffer, cli_fd);
    }
}

int main() {
    int s = socket(PF_INET, SOCK_STREAM, 0);
    exit_on_error(s, "socket");

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY; //
    s_addr.sin_port = htons(8080);

    bind(s, (struct sockaddr*)&s_addr, sizeof(s_addr));
    listen(s, MAX_CLIENTS);

    printf("Servidor de Chat Multilizador Online (Porta 8080)...\n");
    fflush(stdout);

    while (1) {
        struct sockaddr_in c_addr;
        socklen_t len = sizeof(c_addr);
        int cli_fd = accept(s, (struct sockaddr*)&c_addr, &len);

        pthread_mutex_lock(&mutex_clientes);
        if (num_clientes < MAX_CLIENTS) {
            clientes[num_clientes++] = cli_fd;
            
            // Criar uma thread para este novo cliente 
            int* p_cli_fd = malloc(sizeof(int));
            *p_cli_fd = cli_fd;
            pthread_t t;
            pthread_create(&t, NULL, tratar_cliente, p_cli_fd);
            
            printf("Novo cliente conectado! Total: %d\n", num_clientes);
        } else {
            printf("Aviso: Sala cheia. Conexão rejeitada.\n");
            close(cli_fd);
        }
        pthread_mutex_unlock(&mutex_clientes);
        fflush(stdout);
    }

    pthread_mutex_destroy(&mutex_clientes); // [cite: 350]
    return 0;
}