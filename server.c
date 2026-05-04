//Exemplo 4.1.1-Serv
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }
main() {
    int s;
    s = socket ( AF_UNIX, SOCK_STREAM, 0 );
    exit_on_error ( s, "ao criar socket");
    struct sockaddr_un s_addr;
    s_addr.sun_family = AF_UNIX;
    strcpy ( s_addr.sun_path, "meu-socket");
    unlink ( s_addr.sun_path );
    int status;
    status = bind ( s, (struct sockaddr*)&s_addr, sizeof(s_addr) );
    exit_on_error ( status, "bind");
    status = listen ( s, 3 );
    exit_on_error ( status, "listen");
    while ( 1 ) {
        printf ("listening...\n");
        int s_cliente;
        struct sockaddr_un s_addr_c;
        int t = sizeof(s_addr_c);
        s_cliente = accept ( s, (struct sockaddr *)&s_addr_c, &t );
        exit_on_error ( s_cliente, "accept");
        char msg1[100], msg2[100];
        int n = recv ( s_cliente, msg1, sizeof(msg1), 0 );
        exit_on_error ( n, "recv");
        printf ("pedido de %s\n", msg1 );
        sprintf ( msg2, "Hello, %s", msg1);
        n = send ( s_cliente, msg2, sizeof(msg2), 0 );
        exit_on_error ( n, "send");
    }
}