#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la restricción de recursos del sistema.
 * * Requisitos:
 * 1. Definir los límites (soft y hard) para el espacio de direccionamiento virtual.
 * 2. Aplicar el límite utilizando la syscall 'setrlimit'.
 * 3. Garantizar que si la configuración falla, el proceso no continúe su ejecución.
 * * Ayuda: Revisar la constante RLIMIT_AS en la documentación de Linux.
 */
void apply_resource_limits(size_t mem_limit) {
    // TODO: Configurar la estructura rlimit y ejecutar la syscall.
    struct rlimit limits;
    limits.rlim_cur = limits.rlim_max = mem_limit; 
    // De manera preventida se le asigna asi para evitar que los procesos se modifiquen por si mismos.

    // Casos a considerar:
    // - ¿Qué diferencia hay entre rlim_cur y rlim_max?
    // - ¿Qué sucede si el límite solicitado es menor al tamaño del propio binario?
    if (setrlimit(RLIMIT_AS, &limits) == -1) {
        perror("Error al aplicar límites en memoria");
        exit(EXIT_FAILURE);
    }
}
