
//gcc -I../obj -g huge_threads.c ../obj/libst.a -o huge_threads -p thread
//./huge_threads 10000
//./huge_threads 30000
//
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "st.h"
 
#define SLEEP_INTERVAL 30  //in ms
#define LOOP_MAX 1000  // loop times
 
void* do_calc(void* arg){
    long pidx = *(long*)arg;
    printf("sthread [#%ld] start\n", pidx);
    for(int i = 0; i < LOOP_MAX; i++){
        printf("pthread %d st_hread [#%ld] usleep %d\n", pthread_self(), pidx, i);
        st_usleep(SLEEP_INTERVAL * 1000);
    }
    st_thread_self();
    st_thread_exit("st_thread exit\n");
    return NULL;
}
void* thread_func(void* name)
{
    if(st_init() < 0){
        printf("state threads lib runtime init error!");
        return NULL;
    }
 
    long i, count = atoi((const char*)name);
    st_thread_t last_st = NULL;
    for(i = 1; i <= count; i++){
        last_st = st_thread_create(do_calc, (void*)&i, 1, 0);
        if(last_st == NULL){
            printf("create state thread %d failed\n", i);
            return NULL;
        }
        // 由于协成启动较慢，外部变了 i 已经变了，do_calc才执行
        st_usleep(SLEEP_INTERVAL * 1000);
    }
    void* last_join = NULL;
    if(last_st){
        st_thread_join(last_st, &last_join);
        printf("last st thread loop number is %s\n", (char *)last_join);
    }
    pthread_exit("pthread_exit\n");
} 

int main(int argc, char** argv){
    if(argc <= 1){
        printf("Test the concurrence of state-threads!\n"
            "Usage: %s <sthread_count>\n"
            "eg. %s 10000\n", argv[0], argv[0]);
        return -1;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, argv[1]);
    pthread_t tid1;
    pthread_create(&tid1, NULL, thread_func, argv[1]);
    void *join_ret, *join_ret1;
    pthread_join(tid, &join_ret);
    pthread_join(tid1, &join_ret1);
    printf("join_ret = %s", (char *)join_ret);
    printf("join_ret1 = %s", (char *)join_ret1);
 
    return 0;
}
/*
// --------------------- 

作者：雪峰流云 

来源：CSDN 

原文：https://blog.csdn.net/tao_627/article/details/45788013 

版权声明：本文为博主原创文章，转载请附上博文链接！
2.0 version author :zhoulipeng
*/

