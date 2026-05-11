#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Necessário para a função inet_addr() da vossa sebenta

#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }

int main() {
    int s = socket ( PF_INET, SOCK_STREAM, 0 );;
    
    // 1. Alterado para PF_INET (Internet) em vez de AF_UNIX (Ficheiro local)
    exit_on_error ( s, "socket");
    
    // 2. Utilizar a estrutura sockaddr_in conforme a secção 8.2
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    
    // 3. Definir a porta 8080 (para bater certo com o teu servidor)
    s_addr.sin_port = htons(8080);
    
    // 4. Definir o IP de destino com inet_addr() da sebenta
    s_addr.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
    
    int status;
    status = connect( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "connect");
    
    char nome[100], msg2[100];
    
    printf ("Nome: ");
    fflush(stdout); // Adicionado para garantir que o Docker imprime logo o pedido!
    fgets ( nome, 100, stdin );
    
    int n = send ( s, nome, sizeof(nome), 0 );
    exit_on_error ( n, "nome");
    
    n = recv ( s, msg2, sizeof(msg2), 0 );
    exit_on_error ( n, "recv");
    
    printf ("%s\n", msg2 );
    
    return 0;
}