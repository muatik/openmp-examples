#include <iostream>
#include <omp.h>
#include <zconf.h>


struct Node {
    int data;
    struct Node* next = NULL;
    Node() {}
    Node(int data) {
        this->data = data;
    }
    Node(int data, Node* node) {
        this->data = data;
        this->next = node;
    }
};

void processNode(Node *pNode);

struct Queue {
    Node *head = NULL, *tail = NULL;

    Queue& add(int data) {
        add(new Node(data));
        return *this;
    }

    void add(Node *node) {
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    Node* remove() {
        Node *node;
            node = head;
        if (head != NULL)
            head = head->next;

        return node;
    }

};

int main() {
    srand(12);
    Queue queue;
    for (int i = 0; i < 6; ++i) {
        queue.add(i);
    }

    double timer_started = omp_get_wtime();
    omp_set_num_threads(3);
    #pragma omp parallel
    {
        Node *n;
        while ((n = queue.remove()) != NULL) {
            double started = omp_get_wtime();
            processNode(n);
            double elapsed = omp_get_wtime() - started;
            printf("Thread id: %d data: %d, took: %f \n", omp_get_thread_num(), n->data, elapsed);
        }
    }
    double elapsed = omp_get_wtime() - timer_started;

    std::cout << "end. took " << elapsed << " in total " << std::endl;
    return 0;
}

void processNode(Node *node) {
    int r = rand() % 3 + 1; // between 1 and 3
    sleep(r);
}