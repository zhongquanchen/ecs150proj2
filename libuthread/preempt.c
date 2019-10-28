#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

//define a static sigaction
static struct sigaction sa;


#define HZ 100
#define INTERVAL 1000000/100

void timer_handler (int sig_num)
{
    uthread_yield();
}


void preempt_disable(void)
{
    sigdelset(&sa.sa_mask, SIGVTALRM);
}

void preempt_enable(void)
{
  	sigaddset(&sa.sa_mask, SIGVTALRM);
}

void preempt_start(void)
{
    /* Initialize sigaction
     * 1.define a timer for alarm setting
     * 2.empty sa using memset
     * 3.set sa.handler to the address of handler function
     * 4.empyt the set that will be blocked in sa_mask */
    struct itimerval timer;
    //memset(&sa, 0, sizeof(sa));
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);

    /* detect if sigaction has created succussful */
    if( sigaction(SIGVTALRM, &sa, NULL) == -1 ) {
        perror("signal create fial\n");
        exit(1);
    }
    /* Initialize timer set up alarm */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = INTERVAL;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = INTERVAL;

    /* detect if alarm has created succussful */
    if( setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1 ) {
        perror("create alarm fail\n");
        exit(1);
    }
}
