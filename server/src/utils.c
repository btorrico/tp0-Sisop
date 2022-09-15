#include"utils.h"

t_log* logger;

int iniciar_servidor(void)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	int socket_servidor;

	struct addrinfo hints, *servinfo, *p ;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;  /* ai_family = Protocol family for socket.  */
	hints.ai_socktype = SOCK_STREAM; 	/* ai_socktype =	Socket type.  */
	hints.ai_flags = AI_PASSIVE;  /*ai_flags= Input flags.  */

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	// Asociamos el socket a un puerto
//bind() lo que hace es tomar el socket que creamos con anterioridad y pegarlo con pegamento industrial al puerto que le digamos

	/* ai_addr: Socket address for socket.  */
	/* ai_addrlen: Length of socket address.  */



    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
        	printf("connection_error_crear_socket");
            continue;
        }
		int yes = 1;
		setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) != 0) {
        	printf("Error en el bind");
            close(socket_servidor);
            continue;
        }
        break;
    }

	// Escuchamos las conexiones entrantes

	// listen() : toma el socket del servidor creado y lo marca en el sistema como un socket cuya unica responsabilidad es notificar cuando
	//un nuevo cliente esta intentando conectarse

	listen(socket_servidor,SOMAXCONN); // SOMAXCONN: es la cantidad maxima de conexiones vivas que admite el sistema operativo

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente;

	//accept() es bloqueante, significa que el proceso servidor se quedara bloqueado en accept hasta que llegue un cliente

	struct sockaddr_in direccion_Cliente;
	socklen_t tam_Direccion = sizeof(struct sockaddr_in);

	socket_cliente = accept(socket_servidor, (void*) &direccion_Cliente, &tam_Direccion);


	//Una vez que el cliente aceptado, accept retorna un nuevo socket(file descriptor) que representa la conexion BIDIRECCIONAL entre el servidor y el cliente
	//La comunicacion entre el cliente y el servidor se hace entre el socket que realizo connect(lado del cliente) hacia el servidor, y el socket que fue devuelto por el accept

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) // original: ) > 0
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje: %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
