#include <bits/stdc++.h>
using namespace std;
#define MAX 1000
int heapSize = 0;
int heap[MAX];
void heapifyUp(int i)
{
    while (i > 0 && heap[(i - 1) / 2] > heap[i])
    {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void insert(int val)
{

    if (heapSize == MAX)
    {
        cout << "heap full";
        return;
    }
    heap[heapSize] = val;
    heapSize++;
    heapifyUp(heapSize - 1);
}

void heapifyDown(int i)
{

    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && heap[smallest] > heap[left])
        smallest = left;
    if (right < heapSize && heap[smallest] > heap[right])
        smallest = right;

    if (smallest != i)
    {
        swap(heap[smallest], heap[i]);
        heapifyDown(smallest);
    }
}
void deleteValue(int val)
{
    int index = -1;
    for (int i = 0; i < heapSize; i++)
    {
        if (heap[i] == val)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        cout << "Value not found";
        return;
    }
    heap[index] = heap[heapSize - 1];
    heapSize--;
    if (index > 0 && heap[index] < heap[(index - 1) / 2])
    {
        heapifyUp(index);
    }
    else
    {
        heapifyDown(index);
    }
}
void deleteMin()
{
    if (heapSize == 0)
    {
        cout << "empty" << endl;
        return;
    }
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
}
int main()
{
    insert(10);
    insert(20);
    insert(1);
    insert(0);
    insert(100);
    insert(200);
    insert(14);
    insert(45);
    insert(110);
    insert(25);
    insert(111);
    insert(109);
    cout << "After Insert: \n";
    for (int i = 0; i < heapSize; i++)
    {
        cout << heap[i] << " ";
    }
    deleteMin();
    cout << "\nAfter Deleting root: \n";
    for (int i = 0; i < heapSize; i++)
    {
        cout << heap[i] << " ";
    }
    deleteValue(45);
    cout << "\nAfter Deleting Any Middle value: \n";
    for (int i = 0; i < heapSize; i++)
    {
        cout << heap[i] << " ";
    }
}