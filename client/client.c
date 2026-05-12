#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <pthread.h> // Incluído da Ficha P3 
#include <unistd.h>

#include "common.h"


// --- Thread separada APENAS para receber mensagens ---
void* receive_msgs(void* arg) { //Função que será executada pela thread de receção. Recebe um argumento genérico do tipo void*
    int client_socket = *(int*)arg; //Converte o ponteiro genérico de volta para inteiro e guarda o identificador do socket
    char msg_receive[1000]; //Declara um array de caracteres (buffer) de 1000 posições para guardar a mensagem que vai chegar
    
    while(1) {
        memset(msg_receive, 0, sizeof(msg_receive)); //Preenche todo o buffer com zeros para apagar qualquer lixo de memória
        int bytes_receive = recv ( client_socket, msg_receive, sizeof(msg_receive) - 1, 0 ); //Lê dados do socket. O programa pausa aqui até receber algo
        
        if (bytes_receive <= 0) { // Verifica se a quantidade de bytes recebidos é 0 ou menor, o que significa que o servidor desligou ou ocorreu um erro
            printf("\nA ligação com o servidor caiu.\n");
            exit(0);
        }
        
        // Imprime a mensagem recebida
        printf ("%s", msg_receive );
        fflush(stdout); //Força a impressão imediata no ecrã
    }
    return NULL; // [cite: 268]
}

int main() {
    printf ("Cliente on\n");
    fflush(stdout);
    int client_socket = socket ( PF_INET, SOCK_STREAM, 0 );
    exit_on_error ( client_socket, "socket"); //Verifica se a criação do socket falhou
    
    struct sockaddr_in server_address; //Declara uma estrutura que vai guardar os dados de endereço do servidor alvo
    server_address.sin_family = AF_INET; //Define a família de endereços como sendo IPv4
    server_address.sin_port = htons(8080); //Define o porto do servidor para 8080
    server_address.sin_addr.s_addr = inet_addr ( "127.0.0.1" ); //Converte o endereço IP "127.0.0.1" para o formato numérico necessário e guarda na estrutura
    
    int connection_status = connect( client_socket, (struct sockaddr*)&server_address, sizeof(server_address) );
    exit_on_error ( connection_status, "connect");

    //Pede o nome
    char nome[50];
    printf("Introduz o teu nome para começar: ");
    fflush(stdout);
    fgets(nome, 50, stdin);
    nome[strlen(nome) - 1] = '\0'; //Remove \n para não dar enter quando aparece o nome

    
    // Iniciar a Thread de Receção 
    pthread_t thread_receive; //Declara uma variável para armazenar a thread que vamos criar
    pthread_create(&thread_receive, NULL, receive_msgs, &client_socket); //Cria a thread secundária. Vai correr a função e passa-lhe o endereço do socket
    
    // O ciclo principal (Main) fica APENAS a tratar do teclado
    char msg_send[1000];
    while(1) {
        sprintf(msg_send, "%s: ", nome);

        int tamanho_nome = strlen(msg_send);
        fgets(msg_send + tamanho_nome, 1000 - tamanho_nome, stdin); //Fica à espera e lê uma linha inteira do teclado (stdin), e guarda na var 'msg_send'
        
        
        int bytes_sent = send ( client_socket, msg_send, strlen(msg_send), 0 );
        exit_on_error ( bytes_sent, "send");
    }
    
    return 0;
}