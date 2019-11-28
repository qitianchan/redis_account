//
// Created by 陈奇峰 on 2019/11/27.
//

#include <stdio.h>
#include </usr/local/include/hiredis/hiredis.h>
#include <time.h>
#define CMD_MAXLENGTH 256
#define REDIS_SERVER_IP "127.0.0.1"//ip地址
#define REDIS_SERVER_PORT 6379//端口号


typedef struct Payment{//入账数据结构
    char use[64];
    unsigned cost;
}Payment;

typedef enum returnState{//返回状态
    SUCCESS = 0,
    ERROR = 1
}returnState;

returnState addPayment(struct Payment *payment);//添加入账
returnState exeRedisCommand(char *cmd);//执行redis命令
returnState queryPayByDate(unsigned date);//通过date查找
unsigned account_time();//入账时间


returnState addPayment(struct Payment *payment)//可改进存储结构，不一定用lpush
{
    unsigned date = account_time();
    char cmd[CMD_MAXLENGTH] = {0};
    if(payment == NULL)
        return ERROR;
    else{
        snprintf(cmd,CMD_MAXLENGTH,"lpush date:%ud %s:%ud",date,payment->use,payment->cost);
    }

    if(exeRedisCommand(cmd) == ERROR)
    {
        printf("add payment error\n");
        return ERROR;
    }
    printf("add payment success\n");
    return SUCCESS;

}

returnState exeRedisCommand(char *cmd){
    if(cmd == NULL)
        return ERROR;
    redisContext *context = redisConnect(REDIS_SERVER_IP,REDIS_SERVER_PORT);
    if(context->err) {
        printf("connect redis server error:%s\n",context->errstr);
        redisFree(context);
        return ERROR;
    }
    printf("connect redis server success\n");
    redisReply *reply = (redisReply *)redisCommand(context,cmd);
    if(reply == NULL)
    {
        printf("execute command:%s error\n",cmd);
        return ERROR;
    }
    //返回执行结果为整型的命令,只有状态为REDIS_REPLY_INTEGER,并且INTEGER是大于0时,才表示这种类型的命令执行成功
    if(!(reply->type == REDIS_REPLY_INTEGER && reply->integer > 0)){
        printf("execute command:%s error\n", cmd);
        freeReplyObject(reply);
        redisFree(context);
        return ERROR;
    }
    freeReplyObject(reply);
    redisFree(context);
    printf("execute command:%s success\n",cmd);
    return SUCCESS;

}
returnState queryPayByDate(unsigned date)
{

    char cmd[CMD_MAXLENGTH];
    if(date<0&&date>31)
    {
        printf("date input error\n");
        return ERROR;
    }
    /*组装执行命令*/
    snprintf(cmd,CMD_MAXLENGTH,"lrange %ud 0 -1",date);
    if(exeRedisCommand(cmd) == ERROR)
    {
        printf("query payment error");
        return ERROR;
    }
    return SUCCESS;
}

unsigned account_time(){
    unsigned date;
    time_t timep;
    struct tm *p;
    time (&timep);
    p=gmtime(&timep);
    date = p->tm_mday;
    return date;
}


int main(){
    int mark,date;
    Payment payment;
    printf("enter number to select function\n1.存入账本\n2.查找账本");
    scanf("%d",&mark);
    switch (mark){
        case 1:
            scanf("%s%ud",payment.use,&payment.cost);
            addPayment(&payment);
            return 0;
        case 2:
            scanf("%ud",&date);
            queryPayByDate(date);
            return 0;
    }
}
