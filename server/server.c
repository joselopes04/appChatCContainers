#include <stdio.h> // Input/Output (printf)
#include <stdlib.h>
#include <string.h> // Funções com strings
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> // API dos sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> // Incluído da Ficha P3 

//Função para imprimir mensagens de erro  
//GUARDAR NUM FICHEIRO À PARTE DE BIBLIOTECAS
void exit_on_error(int status, const char *message) {
    if (status < 0) { //Verificar se existe erro(funções em C retornam valor negativo)
        perror(message);
        exit(1); //Indicar ao SO que houve um erro
    }
}

int socket_client_1, socket_client_2; // Sockets

// Thread que escuta o cliente 1 e reencaminha a mensagem para o cliente 2
void* retransmite_c1_para_c2(void* arg) {
    char message[1000];
    while(1) {
        memset(message, 0, sizeof(message)); //Limpar o espaço na memória onde vai ser guardada a mensagem
        int tamanho_mensagem = recv(socket_client_1, message, sizeof(message), 0);
        if (tamanho_mensagem <= 0) exit(0); // Se o cliente 1 sair, fecha tudo
        send(socket_client_2, message, tamanho_mensagem, 0);
    }
    return NULL;
}

// Thread que escuta o cliente 2 e reencaminha a mensagem para o cliente 1
void* retransmite_c2_para_c1(void* arg) {
    char message[1000];
    while(1) {
        memset(message, 0, sizeof(message));
        int tamanho_mensagem = recv ( socket_client_2, message, sizeof(message), 0 );
        if (tamanho_mensagem <= 0) exit(0); // Se o cliente 2 sair, fecha tudo
        send ( socket_client_1, message, tamanho_mensagem, 0 );
    }
    return NULL;
}

int main() {
    int socket_server = socket( PF_INET, SOCK_STREAM, 0 );
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
    pthread_t t1_thread, t2_thread;
    pthread_create(&t1_thread, NULL, retransmite_c1_para_c2, NULL); 
    pthread_create(&t2_thread, NULL, retransmite_c2_para_c1, NULL);

    // O Main fica à espera que as threads terminem 
    pthread_join(t1_thread, NULL);
    pthread_join(t2_thread, NULL); 
    
    close(socket_client_1); 
    close(socket_client_2);
     close(socket_server);
    return 0;
}