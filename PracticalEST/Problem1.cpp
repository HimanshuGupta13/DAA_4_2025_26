#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* next;

    Node(int x) {
        data = x;
        next = NULL;
    }
};

class Stack {
    Node* top;
public:
    Stack() {
        top = NULL;
    }

    void push(int x) {
        Node* newNode = new Node(x);
        newNode->next = top;
        top = newNode;
        cout << x << " pushed\n";
    }

    void pop() {
        if (top == NULL) {
            cout << "Stack Underflow\n";
            return;
        }
        Node* temp = top;
        cout << temp->data << " popped\n";
        top = top->next;
        delete temp;
    }
    void isempty() {
        if (top == NULL) {
            cout << "empty\n";
        }
        else{
            cout<<"Not empty"<<endl;
        }
    }
    void peek(){
        cout<<top->data <<endl;
    }

    void size() {
        Node* temp = top;
        int c=0;
        while (temp != NULL) {
            c++;
            temp = temp->next;
        }
        cout <<c <<endl;
    }
};

int main() {
    Stack s;
    s.push(10);   
    s.push(20);
    s.push(30);
    s.pop();
    s.peek();
    s.isempty();
    s.size();
    return 0;
}