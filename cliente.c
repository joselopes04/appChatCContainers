//Exemplo 4.1.1-Cliente
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }
main() {
    int s;
    s = socket ( AF_UNIX, SOCK_STREAM, 0 );
    exit_on_error ( s, "socket");
    struct sockaddr_un s_addr;
    s_addr.sun_family = AF_UNIX;
    strcpy ( s_addr.sun_path, "meu-socket");
    int status;
    status = connect( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "connect");
    char nome[100], msg2[100];
    printf ("Nome: ");
    fgets ( nome, 100, stdin );
    int n = send ( s, nome, sizeof(nome), 0 );
    exit_on_error ( n, "nome");
    n = recv ( s, msg2, sizeof(msg2), 0 );
    exit_on_error ( n, "recv");
    printf ("%s\n", msg2 );
}