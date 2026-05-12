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

typedef struct {
    int socket_origem;
    int socket_destino;
} DadosRelay;

int socket_client_1, socket_client_2; // Sockets

void* retransmitir_mensagem(void* arg) {
    // 1. Extrair os dados do argumento 
    DadosRelay* dados = (DadosRelay*)arg;
    int origin = dados->socket_origem;
    int destination = dados->socket_destino;
    
    char message[1000];
    while(1) {
        memset(message, 0, sizeof(message)); // Limpar o buffer
        
        // Receber do socket de origem
        int tamanho_mensagem = recv(origin, message, sizeof(message), 0); 
        
        // Se a origem se desligar, encerra a comunicação
        if (tamanho_mensagem <= 0) {
            printf("\nUm utilizador desconectou-se. A fechar o chat...\n");
            pthread_exit(NULL); 
        }
        
        // Enviar para o socket de destino
        send(destination, message, tamanho_mensagem, 0); 
    }
    return NULL;
}

int main() {
    int socket_server = socket(PF_INET, SOCK_STREAM, 0);
    exit_on_error(socket_server, "socket");

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY; 
    s_addr.sin_port = htons(8080);

    int status = bind(socket_server, (struct sockaddr*)&s_addr, sizeof(s_addr));
    exit_on_error(status, "bind");
    listen(socket_server, 3);

    printf ("Server on.\n");
    fflush(stdout); 

    struct sockaddr_in s_addr_c1; int t1 = sizeof(s_addr_c1);
    socket_client_1 = accept(socket_server, (struct sockaddr *)&s_addr_c1, &t1);
    printf("Cliente 1 conectado!\n"); fflush(stdout);

    struct sockaddr_in s_addr_c2; int t2 = sizeof(s_addr_c2);
    socket_client_2 = accept(socket_server, (struct sockaddr *)&s_addr_c2, &t2);
    printf("Cliente 2 conectado!\n"); 
    fflush(stdout);

    // Lançar as duas vias de comunicação em simultâneo 
    DadosRelay dados_c1_para_c2 = {socket_client_1, socket_client_2};
    DadosRelay dados_c2_para_c1 = {socket_client_2, socket_client_1};

    pthread_t t1_thread, t2_thread;

    // Lançar as threads usando a mesma função genérica 
    pthread_create(&t1_thread, NULL, retransmitir_mensagem, &dados_c1_para_c2);
    pthread_create(&t2_thread, NULL, retransmitir_mensagem, &dados_c2_para_c1);

    // O Main fica à espera que as threads terminem
    pthread_join(t1_thread, NULL);
    pthread_join(t2_thread, NULL);
    
    close(socket_client_1); 
    close(socket_client_2);
    close(socket_server);
    return 0;
}