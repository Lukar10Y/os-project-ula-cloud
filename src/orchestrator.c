#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar el despliegue del microservicio.
 * * Requisitos:
 * 1. Crear un nuevo proceso.
 * 2. En el proceso HIJO:
 * - Configurar las restricciones de recursos (memoria).
 * - Reemplazar la imagen del proceso por el binario del servicio.
 * 3. En el proceso PADRE:
 * - Registrar el PID y el estado inicial en el dashboard.
 * - Retornar el PID asignado.
 */
int spawn_service(int index) {
    pid_t pid = fork();

    // TODO: Invocar la creación del proceso hijo.

    // Casos a manejar:
    // - Error en la creación del proceso.
    // - Lógica del proceso HIJO (Setup de límites y Ejecución).
    // - Lógica del proceso PADRE (Gestión del dashboard).

    if(pid == 0){
        apply_resource_limits(dashboard[index].mem_limit);
        char* args = {dashboard[index].name, NULL};
        execvp(args[0], args);
        perror("Error en execvp");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0){
        dashboard[index].pid = pid;
        dashboard[index].state = STATE_RUNNING;
    }
    else{
        perror("Error en fork");
        return -1;
    }
    return pid; // Cambiar por el PID real
}
