#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	//t_log* 		log_create(char* file, char *process_name, bool is_active_console, t_log_level level);

	logger = iniciar_logger();

	//leer_consola(log_create("hola.log","holanda",true,LOG_LEVEL_INFO));

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	puerto =  config_get_string_value(config, "PUERTO");
	ip =	config_get_string_value(config,"IP");
	valor = config_get_string_value(config,"CLAVE");
	// Loggeamos el valor de config
	log_info(logger,"Datos de cliente.config: \n IP: %s \n CLAVE: %s \n PUERTO: %s",ip,valor,puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto); // -> conexion: Devuelve un socket_cliente

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	//enviar_paquete(paquete,conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "Loggito", true , LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		log_info(nuevo_logger, "No se pudo crear el archivo");
		exit(1);
	}else{
	log_info(nuevo_logger, "Log creado con exito en la carpeta client :)");
    char *current_dir = getcwd(NULL, 0);
    printf("El directorio actual es %s\n", current_dir);
    free(current_dir);
	return nuevo_logger;

	}
}


t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config =config_create("/home/utnso/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		printf("No se pudo encontrar la ruta");
		exit(3);
	}
	return nuevo_config;

}

void leer_consola(t_log* logger)
{
	char* leido;
	//char* saludillo;

	// La primera te la dejo de yapa

	leido = readline("> ");

		//while(!string_is_empty(leido)){
		while(strcmp(leido,"") !=0){
		leido = readline("> ");
	    log_info(logger,"Los caracteres ingresados por pantalla fueron:  %ld \n", strlen(leido)+1);

	     }
	//	log_info(logger,"No se ingreso nada en la cosola");
		//exit(2);


	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!
		free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	//agregar_a_paquete(2): Dado un stream y su tamaño, lo agrega al paquete
	//void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)

	while(strcmp(leido,"") != 0){


	agregar_a_paquete(paquete,leido,strlen(leido)+1);
	free(leido);
	leido=readline("> ");
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){
		log_destroy(logger);
	}
	if(config !=NULL){
		config_destroy(config);
	}
	liberar_conexion(conexion);
}
