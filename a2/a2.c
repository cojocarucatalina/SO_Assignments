#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>


#define NR_THREADS_2 4
#define NR_THREADS_4 40
#define NR_THREADS_6 4
#define MAX_THREADS 6

int contor = 1;
pthread_t array[4];

pthread_t bariera;
int counter=0;

pthread_mutex_t mutex;

typedef struct{
    int thread;
    int process;
} TH_STRUCT;


sem_t sem1, sem2, sem3,sem4;
sem_t *sem62, *sem24, *sem63;

void notif_end(int i,int j){

    info(END,i,j);

}

void notif_begin(int i, int j){
    info(BEGIN,i,j);

}

void fcn_2(void *params){


    TH_STRUCT *str=(TH_STRUCT*) params;

        info(BEGIN,str->process,str->thread);
        sem_post(&sem1);

        // sem_post(&sem2);
         sem_wait(&sem4);
        info(END,str->process,str->thread);

}

void fcn_3(void *params){

    TH_STRUCT *str=(TH_STRUCT*) params;

  //   sem_wait(&sem2);
        sem_wait(&sem1);
        info(BEGIN,str->process,str->thread);
      //  sem_post(&sem3);
      //  sem_post(&sem4);
        info(END,str->process,str->thread);
        sem_post(&sem4);
        sem_post(&sem1);
}


void *threadFn(void *params)
{

    TH_STRUCT *str=(TH_STRUCT*) params;

    if(str->thread == 1){
        info(BEGIN,str->process,str->thread);
       // sem_post(&sem1);
       // sem_post(&sem3);
        info(END,str->process,str->thread);
    }

     if(str->thread == 2){
       
    //     info(BEGIN,str->process,str->thread);
    //     sem_wait(&sem1);

    //     sem_post(&sem2);
    //     sem_wait(&sem4);
    //     info(END,str->process,str->thread);

        fcn_2(params);

     }

    if(str->thread == 3){
        // sem_wait(&sem2);
        // info(BEGIN,str->process,str->thread);
        // sem_post(&sem3);
        // sem_post(&sem4);
        // info(END,str->process,str->thread);
        fcn_3(params);

    }

    if(str->thread ==4){
        
        sem_wait(sem62);
        info(BEGIN,str->process,str->thread);
        //sem_wait(&sem3);
        //sem_post(sem24);
        info(END,str->process,str->thread);
        sem_post(sem63);
    }
    // info(BEGIN,str->process,str->thread);
     //info(END,str->process,str->thread);

   return NULL;
}


void thread_create()
{
    // TH_STRUCT s[NR_THREADS_2];

    // for(int i=1;i<=NR_THREADS_2;i++){
    //     s[i].process=2;
    //     s[i].thread=1;
    //     pthread_create(&array[i],NULL,threadFn,&s[i]);
    //     info(BEGIN,2,i);
    // }

    // pthread_join(array[1],NULL);
    // pthread_join(array[3],NULL);

    // for(int i=1;i<=NR_THREADS_2;i++){
    //     pthread_join(array[i],0);
    //     info(END,2,i);
    // }

    TH_STRUCT params[NR_THREADS_2];
    pthread_t threads[NR_THREADS_2];
    int i;

    for(i=0;i<NR_THREADS_2;i++){

        params[i].process=2;
        params[i].thread=i+1;
        pthread_create(&threads[i],NULL,threadFn,&params[i]);
       // info(BEGIN,2,i);
    }

    for(i=0;i<NR_THREADS_2;i++){

        pthread_join(threads[i],NULL);
        //info(END,2,i);
    }

}
//             PANA AICI E 2.2                  --2.2
int ok=0;


//  --2.4
sem_t sem6;

sem_t sem5, sem7;
int activeTH=0;
pthread_mutex_t mutex;
int finish=0;
int verif =0;


void function(void *ss){

 TH_STRUCT *s=(TH_STRUCT*) ss;

        while(finish);
        sem_wait(&sem7);
        verif++;
        activeTH--;
        
        notif_end(s->process,s->thread);
        sem_post(&sem7);
}

int rezult(){
    if(activeTH == MAX_THREADS || verif == 39)
    return 1;
    else return 0;
}

void* threadFcn4(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;

 //   info(BEGIN,s->process,s->thread);

    // pthread_mutex_lock(&mutex);
    // counter++;
    // if(s->thread == 13 && (counter==6))
    // pthread_mutex_unlock(&mutex);

    // pthread_mutex_lock(&mutex);
    // counter--;

    // info(END, s->process,s->thread);
    // if(s->thread==13 && ok){
    //     info(END,s->process,s->thread);
    // }
    // pthread_mutex_unlock(&mutex);

    /*int value;

   sem_wait(&sem5); //astept sa fie numa 6 thread-uri
    
   // sem_post(&sem7);

   info(BEGIN,s->process,s->thread);
  //activeTH++;
    // if (s->thread == 14 ){
    //     if(activeTH != MAX_THREADS){
    //     sem_wait(&sem7);
    //     }
  
    sem_getvalue(&sem5,&value);

    printf("\n\n %d \n\n",value);

   if (s->thread == 14) {
        if(value == MAX_THREADS){    
        // sem_wait(&sem7);
        // finish=1;
        info(END,s->process,s->thread);
        activeTH--;
        sem_post(&sem5);
        return NULL;
    }   
   } 

  activeTH--;
    info(END,s->process,s->thread); 
    sem_post(&sem5);*/

    sem_wait(&sem5);

    info(BEGIN,s->process,s->thread);

    sem_wait(&sem7);
    activeTH++;
    sem_post(&sem7);

    if(s->thread == 14){

        finish=1;
        for(;;){
            sem_wait(&sem7);
            if(rezult()){
            notif_end(s->process,s->thread);
            sem_post(&sem7);
            finish=0;
            activeTH--;
                       
            break;
            }
            sem_post(&sem7);
        }
    } else {
        function(s);
    }

sem_post(&sem5);
//sem_destroy(&sem5);

return NULL;

}

void creare_threaduri(){

    TH_STRUCT ss[NR_THREADS_4];

    pthread_t tids[NR_THREADS_4];
    int i;

    for(i=0;i<NR_THREADS_4;i++){

        ss[i].process=4;
        ss[i].thread=i+1;
        pthread_create(&tids[i],NULL,threadFcn4,&ss[i]);

    }

    for(i=0;i<NR_THREADS_4;i++){

        pthread_join(tids[i],NULL);
    }


    // sem_destroy(&sem5);
    // sem_destroy(&sem7);

}


///              DE AICI            -- 2.5

void TT1(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;
  //  sem_wait(sem62);
    info(BEGIN,s->process,s->thread);
    info(END,s->process,s->thread);
   // sem_post(sem63);

}

void TT2(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;
    info(BEGIN,s->process,s->thread);
    info(END,s->process,s->thread);
    sem_post(sem62);
  
}

void TT3(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;
    sem_wait(sem63);
    info(BEGIN,s->process,s->thread);
    info(END,s->process,s->thread);

}

void TT4(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;
    info(BEGIN,s->process,s->thread);
    info(END,s->process,s->thread);

}

void* threadFcn6(void *ss){

    TH_STRUCT *s=(TH_STRUCT*) ss;

    if(s->thread == 1){
        TT1(ss);
    }

    if(s->thread == 2){
        TT2(ss);
    }

      if(s->thread == 3){
        TT3(ss);
    }

      if(s->thread == 4){
        TT4(ss);
    }

    return NULL;
}

void threaduri_process6(){

    TH_STRUCT ss[NR_THREADS_6];
    //pthread_barrier_init(&bariera, NULL, NR_THREADS_4);
    //pthread_mutex_init(&mutex, NULL);
    pthread_t tts[NR_THREADS_6];
    int i;

    sem_init(&sem6,0,MAX_THREADS);

    for(i=0;i<NR_THREADS_6;i++){

        ss[i].process=6;
        ss[i].thread=i+1;
        pthread_create(&tts[i],NULL,threadFcn6,&ss[i]);

    }

    for(i=0;i<NR_THREADS_6;i++){

        pthread_join(tts[i],NULL);
    }
}


int main()
{
//   --- 2.3
    init();

    info(BEGIN, 1, 0);

    sem_init(&sem1,0,0);
    sem_init(&sem2,0,0);
    sem_init(&sem3,0,0);
    sem_init(&sem4,0,0);
    sem_init(&sem5,0,MAX_THREADS);
    sem_init(&sem7,0,1);
    // sem_init(&sem5,0,0);
    // sem_init(&sem7,0,0);

    pthread_mutex_init(&mutex,NULL);

    sem62=sem_open("s62",O_CREAT,0644,0);
    sem24=sem_open("s24",O_CREAT,0644,0);
    sem63=sem_open("s63",O_CREAT,0644,0);

    pid_t pid2 = -1, pid3 = -1, pid4 = -1, pid5 = -1, pid6 = -1, pid7 = -1, pid8 = -1;

    pid2 = fork();
    if (pid2 == 0)
    {
        info(BEGIN, 2, 0);

         thread_create();  
    //        pthread_t threads[NR_THREADS_2+1];
    // int i;

    // for(i=0;i<NR_THREADS_2;i++){

    //     pthread_create(&threads[i+1],NULL,threadFn,NULL);
    //     info(BEGIN,2,i+1);
    // }

    // for(i=0;i<NR_THREADS_2;i++){

    //     pthread_join(threads[i+1],NULL);
    //     info(END,2,i+1);
    // }
            info(END, 2, 0);
        }
        else
        {

            pid3 = fork();
            if (pid3 == 0)
            {
                info(BEGIN, 3, 0);
                pid7 = fork();
                if (pid7 == 0)
                {
                    info(BEGIN, 7, 0);
                    info(END, 7, 0);
                }
                else
                {
                    waitpid(pid7, 0, 0);
                    info(END, 3, 0);
                }
            }
            else
            {
                pid4 = fork();
                if (pid4 == 0)
                {
                    info(BEGIN, 4, 0);

                    creare_threaduri();



                    pid5 = fork();
                    if (pid5 == 0)
                    {
                        info(BEGIN, 5, 0);
                        pid6 = fork();
                        if (pid6 == 0)
                        {
                            info(BEGIN, 6, 0);

                            threaduri_process6();

                           //functionnnn();

                            pid8 = fork();
                            if (pid8 == 0)
                            {
                                info(BEGIN, 8, 0);
                                info(END, 8, 0);
                            }
                            else
                            {
                                waitpid(pid8, 0, 0);
                                info(END, 6, 0);
                            }
                        }
                        else
                        {
                            waitpid(pid6, 0, 0);
                            info(END, 5, 0);
                        }
                    }
                    else
                    {
                        waitpid(pid5, 0, 0);
                        info(END, 4, 0);
                    }
                }
                else
                {
                    waitpid(pid2, 0, 0);
                    waitpid(pid3, 0, 0);
                    waitpid(pid4, 0, 0);
                    info(END, 1, 0);
                }
            }
        }

        sem_destroy(&sem1);
        sem_destroy(&sem2);
        sem_destroy(&sem3);
        sem_destroy(&sem4);
        sem_destroy(&sem5);
        sem_destroy(&sem7);

        sem_close(sem62);
        sem_close(sem24);
        sem_close(sem63);

        pthread_mutex_destroy(&mutex);

        sem_unlink("s62");
        sem_unlink("s63");
        return 0;
    }
