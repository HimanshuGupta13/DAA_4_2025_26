#include <iostream>
using namespace std;

struct Node{
    int data;
    Node *prev;
    Node *next;
};
Node *insert(Node *head, int value){
    Node *newNode = new Node();
    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    if (head == NULL){
        return newNode;
    }    
    Node *temp = head;
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
    return head;
}
Node *deleteByValue(Node *head, int value)
{
    Node *temp = head;
    while (temp != NULL && temp->data != value)
        temp = temp->next;

    if (temp == NULL)
    {
        cout << "Value not found\n";
        return head;
    }
    if (temp->prev == NULL){
        head = temp->next;
    }
    else
        temp->prev->next = temp->next;

    if (temp->next != NULL){
        temp->next->prev = temp->prev;
    }
    delete temp;
    return head;
}

void display(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

int main()
{
    Node *head = NULL;
    head = insert(head, 10);
    head = insert(head, 20);
    head = insert(head, 30);
    display(head);
    head = deleteByValue(head, 10);
    display(head);
    return 0;
}
