#include <stdio.h>
#include <stdlib.h>

/* 
 * Typedef: node / node_t
 * ----------------------
 * used as one piece of the linked list
 * 
 * int data
 * - data stored in the node
 * struct node *next
 * - pointer to next node in the chain
 * - set to NULL if this is the last node in the list
 * struct node *prev
 * - pointer to previous node in the chain
 * - set to NULL if this is the first node in the list
 */
typedef struct node
{
    int data;
    struct node *next;
    struct node *prev;
} node_t;

/*
 * Function: get_last
 * ------------------
 * returns pointer to the last node in the list
 * 
 * input:
 * - node_t *node - pointer to any node in the list
 * 
 * return value:
 * - pointer to last node in the list 
 * - NULL if input is NULL
 */
node_t *get_last(node_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->next != NULL)
    {
        node = node->next;
    }

    return node;
}

/*
 * Function: get_first
 * -------------------
 * returns pointer to the first node in the list
 * 
 * input:
 * - node_t *node - pointer to any node in the list
 * 
 * return value:
 * - pointer to first node in the list 
 * - NULL if input is NULL
 */
node_t *get_first(node_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->prev != NULL)
    {
        node = node->prev;
    }

    return node;
}

/*
 * Function: insert
 * ----------------
 * add a new node to the list at specified index
 * 
 * input:
 * - node_t **head - pointer to the head pointer
 * - int index - index of the list, where the new node will sit, -1 means last
 * - int data - data to be stored in the new node
 * 
 * return value:
 * - error codes:
 * - 0 - success
 * - 1 - index error
 * - 2 - allocation error
 */
int insert(node_t **head, int index, int data)
{
    node_t *new_node;
    node_t *prev_node;
    node_t *next_node;

    // invalid index
    if (index < -1)
        return 1;

    // allocate a new node and set its data
    // return error if allocation fails
    new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL)
        return 2;
    new_node->data = data;

    // set prev_node and next_node to the right values
    if (index >= 0)
    {
        prev_node = NULL;
        next_node = *head;

        for (int i = 0; i < index; i++)
        {
            // index out of range
            if (next_node == NULL)
            {
                free(new_node);
                return 1;
            }

            prev_node = next_node;
            next_node = next_node->next;
        }
    }
    else // index == -1
    {
        next_node = NULL;
        prev_node = get_last(*head);
    }

    // set link new node to its previous and next nodes
    new_node->prev = prev_node;
    new_node->next = next_node;

    // link the adjacent nodes to the new one, if they exist
    if (prev_node != NULL)
    {
        prev_node->next = new_node;
    }
    else
    {
        *head = new_node;
    }

    if (next_node != NULL)
    {
        next_node->prev = new_node;
    }

    return 0;
}

/*
 * Function: erase
 * ---------------
 * erases node of the list at specified index
 * 
 * input:
 * - node **head - pointer to the head pointer
 * - int index - index of the list, from where should be the node deleted, 
 *             => -1 means last 
 * 
 * return value:
 * - error codes:
 * - 0 - success
 * - 1 - index error
 */
int erase(node_t **head, int index)
{
    node_t *node;

    // invalid index
    if (index < -1)
        return 1;

    // set node to point on the specified index
    if (index >= 0)
    {
        node = *head;

        for (int i = 0; i < index; i++)
        {
            // index out of range
            if (node == NULL)
                return 1;

            node = node->next;
        }
    }
    else // index == -1
    {
        node = get_last(*head);
    }

    // link adjacent nodes to point to each other instead of this node
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        *head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }

    // free the unlinked node
    free(node);
    return 0;
}

/*
 * Function: append
 * ----------------
 * add a new node at the end of the list
 * 
 * input:
 * - node_t **node - pointer to the head pointer
 * - int data - data to be stored in the new node
 * 
 * return value:
 * - error codes:
 * - 0 - success
 * - 2 - allocation error
 */
int append(node_t **head, int data)
{
    return insert(head, -1, data);
}

/*
 * Function: pop
 * -------------
 * removes last node from the list and returns its data
 * does not check if head is NULL
 * 
 * input:
 * - node_t **head - pointer to the head pointer
 * 
 * return value:
 * - data of the deleted node
 */
int pop(node_t **head)
{
    int rv;

    rv = get_last(*head)->data;
    erase(head, -1);

    return rv;
}

/*
 * Function: print_list
 * --------------------
 * prints all elements of the list as comma separated values enclosed 
 * in brackets
 * each line is of format "index: value"
 * prints "empty" if there are no nodes in the list
 * 
 * input:
 * - node_t *head - pointer to head of the list
 */
void print_list(node_t *head)
{
    node_t *node;
    node = head;

    if (node == NULL)
    {
        printf("empty\n");
        return;
    }

    printf("[ ");
    while (node != NULL)
    {
        printf("%d%s", node->data, (node->next != NULL) ? ", " : " ]\n");
        node = node->next;
    }
}

int main(int argc, char const *argv[])
{
    // showcase code for the functions above
    // this code does not check for errors that may occur during runtime
    node_t *head;
    head = NULL;

    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    insert(&head, 2, 4);

    print_list(head);
    // Output: [ 1, 2, 4, 3 ]

    erase(&head, 2);

    print_list(head);
    // Output: [ 1, 2, 3 ]

    pop(&head);
    pop(&head);
    pop(&head);

    print_list(head);
    // Output: empty

    return 0;
}
