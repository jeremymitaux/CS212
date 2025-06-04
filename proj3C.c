#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 40

typedef struct {
    int front, back;
    char *strings[QUEUE_SIZE];
} Queue;

Queue female_donors, male_donors, female_recipients, male_recipients, hospitals;

void InitializeQueue(Queue *q)
{
    q->front = 0;
    q->back = 0;
}

char *NewString(char *s)
{
    int len = strlen(s);
    char *rv = malloc(len+1);
    strcpy(rv, s);
    rv[len-1] = '\0'; /* take out newline */
    return rv;
}

void Enqueue(Queue *q, char *str)
{
    if ((q->back +1) % QUEUE_SIZE == q->front)
    {
        // Queue is full
        return;
    }
    q->strings[q->back] = NewString(str); // Allocate and copy the string
    q->back = (q->back + 1) % QUEUE_SIZE;
}

char* Dequeue(Queue *q)
{
    if (q->front == q->back)
    {
        // Queue is empty
        return NULL;
    }
    char *str = q->strings[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return str;
}

int IsEmptyQueue(Queue *q) {
    return q->front == q->back;
}

void PrintQueue(Queue *q)
{
    int i;
    printf("Printing queue %p\n", q);
    printf("\tThe index for the front of the queue is %d\n", q->front);
    printf("\tThe index for the back of the queue is %d\n", q->back);
    if (q->front == q->back)
    {
        printf("\tThe queue is empty.\n");
        return;
    }
    int back = q->back;
    if (q->back < q->front)
    {
        // wrapped around, so modify indexing 
        back += QUEUE_SIZE;
    }

    for (i = q->front ; i < back ; i++)
    {
        printf("\t\tEntry[%d] = \"%s\"\n", i%QUEUE_SIZE, q->strings[i%QUEUE_SIZE]);
    }
}

char* Front(Queue *q) 
{
    if (IsEmptyQueue(q)) {
        return NULL;
    }
    return q->strings[q->front];
}


void
PrettyPrintQueue(Queue *q, char *type)
{
    if (Front(q) == NULL)
    {
        printf("No unmatched entries for %s\n", type);
    }
    else
    {
        char *s;
        printf("Unmatched %s:\n", type);
        while ((s = Dequeue(q)))
        {
            printf("%s\n", s);
        }
    }
}



int main(int argc, char *argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }

FILE *filename = fopen(argv[1], "r");
if (filename == NULL)
{
    perror("Unable to open file.\n");
    return 1;
}

    InitializeQueue(&female_donors);
    InitializeQueue(&male_donors);
    InitializeQueue(&female_recipients);
    InitializeQueue(&male_recipients);
    InitializeQueue(&hospitals);

/***  STEP #1: Test your queue code.  ***/

/* This code:
    Queue q;
    InitializeQueue(&q);
    PrintQueue(&q);
    Enqueue(&q, "hello");
    PrintQueue(&q);
    Enqueue(&q, "world");
    PrintQueue(&q);
    printf("Dequeue: %s\n", Dequeue(&q));
    PrintQueue(&q);
*/

/* Gives this output (with different pointers):
Printing queue 0x7fff5e6874a8
	The index for the front of the queue is 0
	The index for the back of the queue is 0
	The queue is empty.
Printing queue 0x7fff5e6874a8
	The index for the front of the queue is 0
	The index for the back of the queue is 1
		Entry[0] = "hello"
Printing queue 0x7fff5e6874a8
	The index for the front of the queue is 0
	The index for the back of the queue is 2
		Entry[0] = "hello"
		Entry[1] = "world"
Dequeue: hello
Printing queue 0x7fff5e6874a8
	The index for the front of the queue is 1
	The index for the back of the queue is 2
		Entry[1] = "world"
 */

/*** STEP #2: read in from the input file (argc/argv stuff). 
     NewString is likely helpful here.
 ***/

    char line[256];
    while (fgets(line, sizeof(line), filename))
    {
        if (line[0] == 'R')
        {
            if (line[2] == 'F')
            {
            Enqueue(&female_recipients, line + 4);
            }
            else if (line[2] == 'M') 
            {
            Enqueue(&male_recipients, line + 4);
            }
        }
        else if (line[0] == 'D')
            {
                if (line[2] == 'F')
                {
                    Enqueue(&female_donors, line + 4);
                }
                else if (line[2] == 'M')
                {
                    Enqueue(&male_donors, line + 4);
                }
            }
        else if (line[0] == 'H')
            {
                Enqueue(&hospitals, line + 2);
            }
        while (!IsEmptyQueue(&hospitals)) 
        {
            if (!IsEmptyQueue(&female_donors) && !IsEmptyQueue(&female_recipients)) 
            {
                char *hospital = Dequeue(&hospitals);
                char *donor = Dequeue(&female_donors);
                char *recipient = Dequeue(&female_recipients);
                printf("MATCH: %s donates to %s at hospital %s\n", donor, recipient, hospital);
                free(donor);
                free(recipient);
                free(hospital);
            } 
            else if (!IsEmptyQueue(&male_donors) && !IsEmptyQueue(&male_recipients)) 
            {
                char *hospital = Dequeue(&hospitals);
                char *donor = Dequeue(&male_donors);
                char *recipient = Dequeue(&male_recipients);
                printf("MATCH: %s donates to %s at hospital %s\n", donor, recipient, hospital);
                free(donor);                    free(recipient);
                free(hospital);
            } 
            else 
            {
            // No more matches can be made
            break;
            }
        }
    }
    
    fclose(filename);

/*** STEP #3: *after* your queue code works and *after* you can read the file, 
     implement the prompt ***/

    // MatchDonors(&male_donors, &female_donors, &male_recipients, &female_recipients, &hospitals);

// Here are some helpful lines of code:

// My print statement for a match:

    // printf("MATCH: %s donates to %s at hospital %s\n", donor, recip, hosp);


//My print statements for the contents of the queues at the end:

    PrettyPrintQueue(&female_donors, "female donors");
    PrettyPrintQueue(&female_recipients, "female recipients");
    PrettyPrintQueue(&male_donors, "male donors");
    PrettyPrintQueue(&male_recipients, "male recipients");
    PrettyPrintQueue(&hospitals, "hospitals");

    return 0;
}
