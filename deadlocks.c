#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_func1(void* arg) {
    // Bloqueia o primeiro mutex
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: bloqueou mutex1, tentando bloquear mutex2...\n");
    sleep(1); // Inicia a simulaçao 

    // Tenta bloquear o segundo mutex, mas mutex2 já está bloqueado pela thread 2
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: bloqueou mutex2 também.\n");

    // Libera os mutexes após terminar
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

// Função para a segunda thread
void* thread_func2(void* arg) {
    // Bloqueia o segundo mutex
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: bloqueou mutex2, tentando bloquear mutex1...\n");
    sleep(1); // Simula algum processamento

    // Tenta bloquear o primeiro mutex, mas mutex1 já está bloqueado pela thread 1
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: bloqueou mutex1 também.\n");

    // Libera os mutexes após terminar
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Cria as duas threads
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);

    // Espera as threads terminarem (caso contrário, o deadlock não é visível)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destrói os mutexes após o uso
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    printf("Programa concluído (se não houver deadlock).\n");
    return 0;
}
