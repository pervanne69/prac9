/* ----------------------------------------------------*/
/* ИМЯ: Pogosian Samvel, ID пользователя: 23К0180      */
/* ДАТА: 02/11/2025                                    */
/* НАЗНАЧЕНИЕ ПРОГРАММЫ:                               */
/* Демонстрация получения системных данных через msgq.  */
/* Процесс мониторинга получает сообщения от sensor.c,  */
/* отображает их на экране и завершает работу при "end".*/
/* ИМЯ ФАЙЛА: monitor.c                                */
/* ----------------------------------------------------*/
/* ФУНКЦИИ:                                             */
/* main() – подключается к существующей очереди         */
/*          сообщений, получает и выводит данные до     */
/*          получения сигнала завершения ("end").       */
/* ----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0644
#define MSG_KEY 0x1234

struct sys_msg {
    long mtype;
    char mtext[200];
};

int main(void) {
    struct sys_msg msg;
    int msqid;
    key_t key = MSG_KEY;

    if ((msqid = msgget(key, PERMS)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Monitor: ready to receive system info...\n");

    while (1) {
        if (msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        if (strcmp(msg.mtext, "end") == 0)
            break;
        printf("Monitor: received -> %s", msg.mtext);
    }

    printf("Monitor: done receiving.\n");
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
