#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int FCFS(int arr[], int count);
int SSTF(int arr[], int count, int size);
int SCAN(int arr[], int count, int size);
int CSCAN(int arr[], int count, int size);
int LOOK(int arr[], int count, int size);
int CLOOK(int arr[], int count, int size);

int main(void)
{
    char fileName[10] = " ";
    int exit_cond = 0;
    printf("Disk Scheduler Simulation: ");
    scanf(" %s", fileName);
    while(exit_cond != 1)
    {
        int number = 0;
        int count = 0;
        int size = 0;
        if(strcmp(fileName, "QUIT") == 0)
        {
            exit_cond = 1;
            return 0;
        }
        else
        {
            FILE* f = fopen(fileName, "r");
            if(f == NULL)
            {
                printf("Unable to read file!\n");
            }
            else
            {
                fscanf(f, "%d", &size);
                int arr[size];
                while(!feof(f))
                {
                    fscanf(f, "%d", &number);
                    arr[count] = number;
                    count = count + 1;
                }
                int arr2[count];
                int arr3[count];
                for(int i = 0; i < count; i++)
                {
                    arr2[i] = arr[i];
                    arr3[i] = arr[i];
                }
                char* token = strtok(fileName, ".");
                printf("For %s: \n", token);
                FCFS(arr, count);
                SSTF(arr2, count, size);
                SCAN(arr, count, size);
                CSCAN(arr, count, size);
                LOOK(arr, count, size);
                CLOOK(arr, count, size);
            }
            printf("\n");
            printf("Disk Scheduler Simulation: ");
            scanf(" %s", fileName);
        }
    }
    return 0;
}

int FCFS(int arr[], int count)
{
    int curr = arr[0];
    int next;
    int seekCount = 0;
    int temp = 0;
    int i = 2;
    for(i; i < count ; i++)
    {
        next = arr[i];
        temp = abs(next - curr);
        seekCount = seekCount + temp;
        curr = arr[i];
    }

    printf("FCFS: %d\n", seekCount);
    return 0;
}

int SSTF(int arr[], int count, int size)
{
    int inc = 0;
    int curr = arr[0];
    int seekCount = 0;
    int temp = 0;
    int index = 0;
    while(inc != count)
    {
        int min = size;
        for(int i = 2; i < count; i++)
        {
            temp = abs(arr[i] - curr);
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
        curr = arr[index];
        arr[index] = size+1000;
        inc++;
    }
    printf("SSTF: %d\n", seekCount);
}

int SCAN(int arr[], int count, int size)
{
    int curr = arr[0];
    int prev = arr[1];
    int temp = prev - curr;
    int seekCount = 0;
    int temp2;
    int index;
    int i;
    
    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr[i] < arr[j])
            {
                temp2 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        seekCount = seekCount + abs(size-arr[i-1]-1);
        curr = size - 1;
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        seekCount = seekCount + abs(arr[i+1]-0);
        curr = 0;
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    printf("SCAN : %d\n", seekCount);
}

int CSCAN(int arr[], int count, int size)
{
    int curr = arr[0];
    int prev = arr[1];
    int temp = prev - curr;
    int seekCount = 0;
    int i;
    int temp2;
    int index;

    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr[i] < arr[j])
            {
                temp2 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        seekCount = seekCount + abs(size - arr[i-1] - 1);
        seekCount = seekCount + abs(size - 1 - 0);
        curr = 0;
        for(i = 2; i < index; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        seekCount = seekCount + abs(arr[2]-0);
        seekCount = seekCount + abs(size-1-0);
        curr = size - 1;
        for(i = count - 1; i >= index; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    printf("C-SCAN : %d\n", seekCount);
}

int LOOK(int arr[], int count, int size)
{
    int curr = arr[0];
    int prev = arr[1];
    int seekCount = 0;
    int temp = prev - curr;
    int i;
    int temp2;
    int index;

    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr[i] < arr[j])
            {
                temp2 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        for(i = index - 1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    else
    {
        for(i = index - 1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    printf("LOOK: %d\n", seekCount);
}

int CLOOK(int arr[], int count, int size)
{
    int curr = arr[0];
    int prev = arr[1];
    int seekCount = 0;
    int temp = prev - curr;
    int i;
    int temp2;
    int index;
    
    for(i = 2; i < count; i++)
    {
        for(int j = 2; j < count; j++)
        {
            if(arr[i] < arr[j])
            {
                temp2 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp2;
            }
        }
    }

    for(i = 2; i < count; i++)
    {
        if(curr < arr[i])
        {
            index = i;
            break;
        }
    }

    if(temp < 0)
    {
        for(i = index; i < count; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        for(i = 2; i < index; i++)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }
    else
    {
        for(i = index-1; i >= 2; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
        for(i = count-1; i >= index; i--)
        {
            seekCount = seekCount + abs(arr[i] - curr);
            curr = arr[i];
        }
    }

    printf("C-LOOK: %d\n", seekCount);
}