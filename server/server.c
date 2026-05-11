#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> // Incluído da Ficha P3 

#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }

int s_cli1, s_cli2; // Variáveis globais para as threads acederem [cite: 263]

// Thread que escuta o Cliente 1 e reencaminha para o Cliente 2
void* retransmite_c1_para_c2(void* arg) {
    char buffer[1000];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv ( s_cli1, buffer, sizeof(buffer), 0 );
        if (n <= 0) exit(0); // Se o cliente 1 sair, fecha tudo
        send ( s_cli2, buffer, n, 0 );
    }
    return NULL; // [cite: 268]
}

// Thread que escuta o Cliente 2 e reencaminha para o Cliente 1
void* retransmite_c2_para_c1(void* arg) {
    char buffer[1000];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv ( s_cli2, buffer, sizeof(buffer), 0 );
        if (n <= 0) exit(0); // Se o cliente 2 sair, fecha tudo
        send ( s_cli1, buffer, n, 0 );
    }
    return NULL; // [cite: 268]
}

int main() {
    int s = socket ( PF_INET, SOCK_STREAM, 0 );
    exit_on_error ( s, "socket");

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY; 
    s_addr.sin_port = htons(8080);

    int status = bind ( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "bind");
    listen ( s, 3 );

    printf ("Server on. A aguardar 2 clientes...\n");
    fflush(stdout); 

    struct sockaddr_in s_addr_c1; int t1 = sizeof(s_addr_c1);
    s_cli1 = accept ( s, (struct sockaddr *)&s_addr_c1, &t1 );
    printf ("Cliente 1 conectado!\n"); fflush(stdout);

    struct sockaddr_in s_addr_c2; int t2 = sizeof(s_addr_c2);
    s_cli2 = accept ( s, (struct sockaddr *)&s_addr_c2, &t2 );
    printf ("Cliente 2 conectado! O chat comecou.\n"); fflush(stdout);

    // Lançar as duas vias de comunicação em simultâneo [cite: 272, 273]
    pthread_t t1_thread, t2_thread;
    pthread_create(&t1_thread, NULL, retransmite_c1_para_c2, NULL); // 
    pthread_create(&t2_thread, NULL, retransmite_c2_para_c1, NULL); // [cite: 273]

    // O Main fica à espera que as threads terminem [cite: 274, 275]
    pthread_join(t1_thread, NULL); // [cite: 274]
    pthread_join(t2_thread, NULL); // [cite: 275]
    
    close(s_cli1); close(s_cli2); close(s);
    return 0;
}