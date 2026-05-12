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

#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }

// --- Thread separada APENAS para receber mensagens ---
void* receber_mensagens(void* arg) {
    int s = *(int*)arg;
    char msg_recebe[1000];
    
    while(1) {
        memset(msg_recebe, 0, sizeof(msg_recebe));
        int n = recv ( s, msg_recebe, sizeof(msg_recebe) - 1, 0 );
        
        if (n <= 0) {
            printf("\nA ligação com o servidor caiu.\n");
            exit(0);
        }
        
        // Imprime a mensagem recebida
        printf ("Amigo: %s", msg_recebe );
        fflush(stdout);
    }
    return NULL; // [cite: 268]
}

int main() {
    printf ("Cliente on\n");
    fflush(stdout);
    int s = socket ( PF_INET, SOCK_STREAM, 0 );
    exit_on_error ( s, "socket");
    
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8080);
    s_addr.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
    
    int status = connect( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "connect");
    
    // Iniciar a Thread de Receção 
    pthread_t t_recebe;
    pthread_create(&t_recebe, NULL, receber_mensagens, &s); // 
    
    // O ciclo principal (Main) fica APENAS a tratar do teclado
    char msg_envia[1000];
    while(1) {
        // printf ("Tu: ");
        // fflush(stdout); 
        fgets ( msg_envia, 1000, stdin );
        
        int n = send ( s, msg_envia, strlen(msg_envia), 0 );
        exit_on_error ( n, "send");
    }
    
    return 0;
}