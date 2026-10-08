#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int completed_tasks;
    pthread_mutex_t mutex;
} SharedData;

void task_completed(SharedData *shared, const char *task_name)
{
    pthread_mutex_lock(&shared->mutex);

    shared->completed_tasks++;
    printf("[SYNC] %s selesai. Total tugas selesai: %d\n",
           task_name, shared->completed_tasks);

    pthread_mutex_unlock(&shared->mutex);
}

void *factorial_task(void *arg)
{
    SharedData *shared = (SharedData *)arg;
    unsigned long long n = 10;
    unsigned long long result = 1;

    for (unsigned long long i = 1; i <= n; i++)
    {
        result *= i;
    }

    printf("[Thread-Faktorial] Faktorial %llu = %llu\n", n, result);
    task_completed(shared, "Faktorial");

    return NULL;
}

void *fibonacci_task(void *arg)
{
    SharedData *shared = (SharedData *)arg;
    int count = 10;
    unsigned long long a = 0, b = 1;

    printf("[Thread-Fibonacci] Fibonacci %d suku: ", count);

    for (int i = 0; i < count; i++)
    {
        printf("%llu%s", a, (i == count - 1) ? "\n" : " ");
        unsigned long long next = a + b;
        a = b;
        b = next;
    }

    task_completed(shared, "Fibonacci");

    return NULL;
}

void *file_task(void *arg)
{
    SharedData *shared = (SharedData *)arg;
    const char *filename = "data.txt";
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        file = fopen(filename, "w");
        if (file == NULL)
        {
            perror("Gagal membuat file");
            return NULL;
        }

        fprintf(file, "Baris pertama file teks.\n");
        fprintf(file, "Baris kedua dibaca oleh thread.\n");
        fclose(file);

        file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("Gagal membuka file");
            return NULL;
        }
    }

    char line[256];

    printf("[Thread-File] Isi file:\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("  %s", line);
    }

    fclose(file);

    task_completed(shared, "Baca File");

    return NULL;
}

int main(void)
{
    pthread_t t1, t2, t3;

    SharedData shared;
    shared.completed_tasks = 0;
    pthread_mutex_init(&shared.mutex, NULL);

    printf("=== Program Multithreading C (Pthreads) ===\n");

    pthread_create(&t1, NULL, factorial_task, &shared);
    pthread_create(&t2, NULL, fibonacci_task, &shared);
    pthread_create(&t3, NULL, file_task, &shared);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Semua thread selesai.\n");
    printf("Total tugas selesai = %d\n", shared.completed_tasks);

    pthread_mutex_destroy(&shared.mutex);

    return 0;
}