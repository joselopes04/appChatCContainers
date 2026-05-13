#include <stdio.h> // Input/Output printf()
#include <stdlib.h> // Funções de sistema exit(), malloc()
#include <string.h> // Funções com strings
#include <unistd.h> // close()
#include <sys/types.h> //Define tipos de dados socklen_t
#include <sys/socket.h> // API dos sockets
#include <netinet/in.h> //Funções como AF_INET
#include <arpa/inet.h> // Funções como inet_addr()
#include <pthread.h> // Funções das Threads
#include "./common.h"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1000 //Tamanho da mensagem

int clientes[MAX_CLIENTS]; // Lista de sockets dos clientes
int num_clientes = 0;

pthread_mutex_t mutex_clientes = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger o acesso ao array de clientes

//Função configura e inicializa o servidor e devolve o socket do mesmo
int iniciarServidor(){
    int sockt = socket(PF_INET, SOCK_STREAM, 0); //Cria o socket do server
    exit_on_error(sockt, "socket");

    struct sockaddr_in s_addr; // Estrutra para guardar dados do server
    s_addr.sin_family = AF_INET; //Definir família de protocolos(IPv4)
    s_addr.sin_addr.s_addr = INADDR_ANY; // Permite o server aceitar ligações de qlqr IP
    s_addr.sin_port = htons(8080); //Atribui esta porta 8080 ao servidor
    int status_bind = bind(sockt, (struct sockaddr*)&s_addr, sizeof(s_addr));  //Regista a estrutura do socket
    exit_on_error(status_bind, "bind");

    int status_listen = listen(sockt, MAX_CLIENTS); //Ativa o socket para receber pedidos de ligação
    exit_on_error(status_listen, "listen");
    printf("Servidor ON\n");
    fflush(stdout);
    return sockt
}

// Função que envia mensagem a todos os clientes conectados (exceto o remetente)
void sendMessage(char* mensagem, int remetente_fd){
    pthread_mutex_lock(&mutex_clientes); // Bloqueia para acesso ao array 
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i] != remetente_fd) {
            send(clientes[i], mensagem, strlen(mensagem), 0);
        }
    }
    pthread_mutex_unlock(&mutex_clientes); // Liberta o acesso do array
}

// Função para
void* tratar_cliente(void* arg){
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
        sendMessage(buffer, cli_fd);
    }
}

int main() {
    int sockt = iniciarServidor()    

    while (1) {
        struct sockaddr_in client_addr; //Estrutura para guardar dados do cliente
        socklen_t len = sizeof(client_addr); // Define o tamanho da estrutura de endereço para a função accept
        int client_file_descriptor = accept(sockt, (struct sockaddr*)&client_addr, &len);
        exit_on_error(client_file_descriptor, "accept");

        pthread_mutex_lock(&mutex_clientes); // Tranca o mutex para garantir exclusão mútua ao alterar a lista de clientes.
        if (num_clientes < MAX_CLIENTS) {
            clientes[num_clientes++] = client_file_descriptor;
            
            // Criar uma thread para este novo cliente 
            int* p_cli_fd = malloc(sizeof(int));
            *p_cli_fd = client_file_descriptor;
            pthread_t t;
            pthread_create(&t, NULL, tratar_cliente, p_cli_fd);
            
            printf("Novo cliente conectado! Total: %d\n", num_clientes);
        } else {
            printf("Aviso: Sala cheia. Conexão rejeitada.\n");
            close(client_file_descriptor);
        }
        pthread_mutex_unlock(&mutex_clientes);
        fflush(stdout);
    }

    pthread_mutex_destroy(&mutex_clientes); // [cite: 350]
    return 0;
}