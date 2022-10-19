#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* FCFS(void* args);
void* SSTF(void* args);
void* SCAN(void* args);
void* CSCAN(void* args);
void* LOOK(void* args);
void* CLOOK(void* args);

int count = 0;
int size = 0;
int buffer1[1003];
int buffer2 = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
int exit_request = 0;
int read_err = 0;

int main(void)
{
    char fileName[10] = " ";
    int number = 0;
    pthread_t A, B, C, D, E, F;
    pthread_t threads[6] = {A, B, C, D, E, F};
    void* func[6] = {&FCFS, &SSTF, &SCAN, &CSCAN, &LOOK, &CLOOK};
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    while(exit_request != 1)
    {
        for(int i = 0; i < 6; i++)
        {
            if(pthread_create(&threads[i], NULL, func[i], NULL) != 0)
            {
                perror("Failed to create thread!");
            }
        }
        printf("Disk Scheduler Simulation: ");
        scanf(" %s", fileName);
        
        if(strcmp(fileName, "QUIT") == 0)
        {
            exit_request = 1;
            count = 1;
            pthread_cond_broadcast(&cond);
            pthread_exit(0);
        }
        else
        {
            FILE* f = fopen(fileName, "r");
            if(f == NULL)
            {
                printf("Unable to read file!\n");
                count = 1;
                read_err = 1;
                pthread_cond_broadcast(&cond);
                printf("\n");
            }
            else
            {
                fscanf(f, "%d", &size);
                while(!feof(f))
                {
                    fscanf(f, "%d", &number);
                    buffer1[count] = number;
                    count = count + 1;
                }
                char* token = strtok(fileName, ".");
                printf("For %s: \n", token);
                pthread_cond_broadcast(&cond);
                for(int i = 0; i < 6; i++)
                {
                    if(pthread_join(threads[i], NULL) != 0)
                    {
                        perror("Failed to join thread!");
                    }
                }
                printf("\n");
            }
            count = 0;
            read_err = 0;
        }
    }
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}

void* FCFS(void* args)
{
    pthread_mutex_lock(&mutex);
    int seekCount = 0;
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int next;
    int temp = 0;
    int i = 2;
    for(i; i < count ; i++)
    {
        next = buffer1[i];
        temp = abs(next - curr);
        seekCount = seekCount + temp;
        curr = buffer1[i];
    }
    printf("FCFS: %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* SSTF(void* args)
{
    pthread_mutex_lock(&mutex);
    int inc = 0;
    int seekCount = 0;
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int temp = 0;
    int index = 0;
    int arr2[count];
    for(int k = 0; k < count; k++)
    {
        arr2[k] = buffer1[k];
    }

    while(inc != count)
    {
        int min = size;
        for(int i = 2; i < count; i++)
        {
            temp = abs(arr2[i] - curr);
            if(temp == 0)
            {
                continue;
            }
            else
            {
                if(temp < min)
                {
                    min = temp;
                    index = i;
                }
            }
        }
        if(min != size)
        {
            seekCount = seekCount + min;
        }
        curr = arr2[index];
        arr2[index] = size+1000;
        inc++;
    }
    printf("SSTF: %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* SCAN(void* args)
{
    pthread_mutex_lock(&mutex);
    int seekCount = 0;
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int prev = buffer1[1];
    int temp = prev - curr;
    int arr2[count];
    int temp2;
    int index;
    int i;
    for(int k = 0; k < count; k++)
    {
        arr2[k] = buffer1[k];
    }
    
    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr2[i] < arr2[j])
            {
                temp2 = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr2[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        seekCount = seekCount + abs(size-arr2[i-1]-1);
        curr = size - 1;
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        seekCount = seekCount + abs(arr2[i+1]-0);
        curr = 0;
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    printf("SCAN : %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* CSCAN(void* args)
{
    pthread_mutex_lock(&mutex);
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int prev = buffer1[1];
    int temp = prev - curr;
    int seekCount = 0;
    int arr2[count];
    int i;
    int temp2;
    int index;
    for(int k = 0; k < count; k++)
    {
        arr2[k] = buffer1[k];
    }

    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr2[i] < arr2[j])
            {
                temp2 = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr2[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        seekCount = seekCount + abs(size - arr2[i-1] - 1);
        seekCount = seekCount + abs(size - 1 - 0);
        curr = 0;
        for(i = 2; i < index; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        seekCount = seekCount + abs(arr2[2]-0);
        seekCount = seekCount + abs(size-1-0);
        curr = size - 1;
        for(i = count - 1; i >= index; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    printf("C-SCAN : %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* LOOK(void* args)
{
    pthread_mutex_lock(&mutex);
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int prev = buffer1[1];
    int seekCount = 0;
    int temp = prev - curr;
    int arr2[count];
    int i;
    int temp2;
    int index;
    for(int k = 0; k < count; k++)
    {
        arr2[k] = buffer1[k];
    }

    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr2[i] < arr2[j])
            {
                temp2 = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr2[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        for(i = index - 1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    else
    {
        for(i = index - 1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    printf("LOOK: %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* CLOOK(void* args)
{
    pthread_mutex_lock(&mutex);
    while(count <= 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    if(exit_request == 1)
    {
        printf("%lu Has Terminated\n", pthread_self());
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    if(read_err == 1)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(args);
    }
    int curr = buffer1[0];
    int prev = buffer1[1];
    int seekCount = 0;
    int temp = prev - curr;
    int arr2[count];
    int i;
    int temp2;
    int index;
    for(int k = 0; k < count; k++)
    {
        arr2[k] = buffer1[k];
    }
    
    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr2[i] < arr2[j])
            {
                temp2 = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr2[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        for(i = 2; i < index; i++)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
        for(i = count-1; i >= index; i--)
        {
            seekCount = seekCount + abs(arr2[i] - curr);
            curr = arr2[i];
        }
    }

    printf("C-LOOK: %d\n", seekCount);
    pthread_mutex_unlock(&mutex);
    return 0;
}