#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }

int main() {
    // 1. Criar o socket PF_INET (Internet) conforme a secção 8.2
    int s = socket ( PF_INET, SOCK_STREAM, 0 );
    exit_on_error ( s, "socket");

    // 2. Configurar o endereço com AF_INET
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    
    // 3. Usar INADDR_ANY (da secção 8.3) para funcionar no Docker
    s_addr.sin_addr.s_addr = INADDR_ANY;
    
    // 4. Configurar a porta. Vamos usar a 8080 (se quiseres usar a 5678 da sebenta, muda também no docker-compose)
    s_addr.sin_port = htons(8080);

    int status;
    status = bind ( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "bind");

    status = listen ( s, 3 );
    exit_on_error ( status, "listen");

    while ( 1 ) {
        printf ("Miguel + Gui = <3...\n");
        fflush(stdout); // Mantemos o fflush para garantir que aparece nos logs do Docker

        int s_cliente;
        
        // CORREÇÃO DA SEBENTA: usar sockaddr_in em vez de sockaddr_un
        struct sockaddr_in s_addr_c; 
        int t = sizeof(s_addr_c);
        
        s_cliente = accept ( s, (struct sockaddr *)&s_addr_c, &t );
        exit_on_error ( s_cliente, "accept");

        char msg1[1000], msg2[1000];
        
        int n = recv ( s_cliente, msg1, sizeof(msg1), 0 );
        exit_on_error ( n, "recv");
        
        printf ("pedido de %s\n", msg1 );
        sprintf(msg2, "Hello, %s", msg1);
        
        n = send ( s_cliente, msg2, sizeof(msg2), 0 );
        exit_on_error ( n, "send");

        // Fechar a ligação com o cliente após responder (conforme o exemplo 8.3)
        close(s_cliente);
    }
    
    return 0;
}