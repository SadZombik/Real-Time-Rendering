#include <stdio.h>
#include <Windows.h>

#define MAX_THREADS 4

typedef struct {
    HANDLE hMutex;
    HANDLE hSemaphore;
    HANDLE hThreads[MAX_THREADS];
    int tasks[MAX_THREADS];
} ThreadPool;

DWORD WINAPI WorkerThread(LPVOID lpParam) {
    ThreadPool* pool = (ThreadPool*)lpParam;

    while (1) {
        // Wait for a task
        WaitForSingleObject(pool->hSemaphore, INFINITE);

        // Perform the task
        WaitForSingleObject(pool->hMutex, INFINITE);

        int taskId = -1;
        for (int i = 0; i < MAX_THREADS; ++i) {
            if (pool->tasks[i] != 0) {
                taskId = i;
                pool->tasks[i] = 0;
                break;
            }
        }

        ReleaseMutex(pool->hMutex);

        if (taskId != -1) {
            // Execute the task (replace this with your actual task)
            printf("Thread %d executing task %d\n", GetCurrentThreadId(), taskId);
        }
    }

    return 0;
}

void InitializeThreadPool(ThreadPool* pool) {
    pool->hMutex = CreateMutex(NULL, FALSE, NULL);
    pool->hSemaphore = CreateSemaphore(NULL, 0, MAX_THREADS, NULL);

    for (int i = 0; i < MAX_THREADS; ++i) {
        pool->tasks[i] = 0;
        pool->hThreads[i] = CreateThread(NULL, 0, WorkerThread, pool, 0, NULL);
    }
}

void EnqueueTask(ThreadPool* pool, int taskId) {
    WaitForSingleObject(pool->hMutex, INFINITE);

    // Enqueue the task
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (pool->tasks[i] == 0) {
            pool->tasks[i] = taskId;
            ReleaseSemaphore(pool->hSemaphore, 1, NULL);
            break;
        }
    }

    ReleaseMutex(pool->hMutex);
}

void CleanupThreadPool(ThreadPool* pool) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        EnqueueTask(pool, -1);  // Signal threads to exit
    }

    WaitForMultipleObjects(MAX_THREADS, pool->hThreads, TRUE, INFINITE);

    for (int i = 0; i < MAX_THREADS; ++i) {
        CloseHandle(pool->hThreads[i]);
    }

    CloseHandle(pool->hSemaphore);
    CloseHandle(pool->hMutex);
}

int main() {
    ThreadPool pool;

    InitializeThreadPool(&pool);

    // Enqueue tasks
    for (int i = 0; i < 10; ++i) {
        EnqueueTask(&pool, i);
    }

    // Allow some time for tasks to be executed
    Sleep(1000);

    // Cleanup
    CleanupThreadPool(&pool);

    return 0;
}
