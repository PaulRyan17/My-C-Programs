#include <stdio.h>
#include <stdbool.h>

//Protoypes
bool binarySearch(int arr [], int n, int key);
void selectionsort(int arr[], int n);


int main(void)
{
    int arraysize = 10;
    int key = 7;
    int i;

    int numarray[10] = {2,5,4,3,1,8,6,7,9,10};
    selectionsort(numarray, arraysize);

    //Sorted array
    printf("Array is now sorted\n");
    for(i =0;i<arraysize;i++)
    {
        printf("%d\n", numarray[i]);
    }


    bool ifexists = binarySearch(numarray, arraysize, key);

   printf("Does %d exist = %s\n",key, ifexists?"true":"false");
}

bool binarySearch(int arr [], int n, int key)
{
    int min;
    int max = n-1;

    while(min<=max)
    {
        int mid = (min+max)/2;

        if(arr[mid] < key)
        {
            min = mid + 1;
        }

        else if(arr[mid] == key)
        {
            return true;
        }

        else if(arr[mid] > key)
        {
            max = mid - 1;
        }
    }

    return false;
}

void selectionsort(int arr[], int n)
{
    int steps;
    int temp;
    int i;

    for(steps = 0; steps < n; steps++)
    {
        for(i = steps +1;i<n;i++)
        {
            if(arr[steps]>arr[i])
            {
                temp = arr[steps];
                arr[steps] = arr[i];
                arr[i] = temp;
            }
        }
    }
}

