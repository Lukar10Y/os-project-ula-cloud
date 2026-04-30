#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la rutina del hilo monitor (Watchdog).
 * * Requisitos:
 * 1. Recuperar la estructura del servicio desde el argumento 'arg'.
 * 2. Utilizar 'waitpid' de forma BLOQUEANTE para esperar al proceso hijo.
 * 3. Analizar el estado de terminación (éxito, error o señal del sistema).
 * 4. Actualizar el Dashboard Global de forma SEGURA (evitar condiciones de carrera).
 */
void* monitor_service(void *arg) {
    // TODO: Castear el argumento al tipo de dato correcto.
    service_t* service = (service_t *)arg;
    // TODO: Implementar la espera del proceso específico.
    int sig;
    if(waitpid((*service).pid, &sig, 0) == -1) {
        perror("Error en waitpid");
        return NULL;
    }
    // Ayuda: Revisar el uso de waitpid(pid, &status, 0).

    /* * Una vez que waitpid retorna, el proceso hijo ha cambiado de estado.
     * TODO: Analizar el 'status' usando las macros de sys/wait.h:
     * - WIFEXITED: ¿Terminó normalmente?
     * - WEXITSTATUS: ¿Cuál fue su código de retorno?
     * - WIFSIGNALED: ¿Fue terminado por una señal (Segfault, OOM Killer)?
     * - WTERMSIG: ¿Qué señal lo mató?
     */
    pthread_mutex_lock(&dashboard_mutex);
    printf("Cambio de estado detectado para %s (PID: %d)\n", service->name, service->pid);
    service->exit_status = sig;
    if(WIFEXITED(sig)) {
        if(WEXITSTATUS(sig) == 0) {
            printf("Terminó normalmente con código (%d)\n", WEXITSTATUS(sig));
            service->state = STATE_STOPPED;
        } else {
            printf("Terminó con error con código (%d)\n", WEXITSTATUS(sig));
            service->state = STATE_CRASHED;
        }
    } else if(WIFSIGNALED(sig)) {
        printf("Fue terminado por la señal (%d)\n", WTERMSIG(sig));
        service->state = STATE_KILLED;
    }
    pthread_mutex_unlock(&dashboard_mutex);

    /*
     * TODO: Actualizar el dashboard global.
     * ¡CRÍTICO!: El acceso al array 'dashboard' debe estar protegido. 
     * No olvides liberar el mecanismo de sincronización al terminar.
     */

    return NULL;
}
