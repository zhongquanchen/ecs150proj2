#include <uthread.h>
#include <stdio.h>
#include <stdlib.h>

/* for this function we will have to end it maunally
 * purpose for this test is to see if the program will
 * yield automatically. To End Program, hti ctrl + c */

int thread2(void* arg)
{
    while(2)
        printf("2\n");
    return 0;
}

int thread1(void* arg)
{
    uthread_create(thread2, NULL);
    while(1){
        printf("i am in thread1\n");
    }
    return 0;
}

int main(void)
{
    /* it will test the preemption */
    /* don't know how to test it yet */
    uthread_t t1 = uthread_create(thread1, NULL);
    uthread_join(t1, NULL);
    printf("come back from t1\n");
    return 0;
}
