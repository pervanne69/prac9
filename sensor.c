/* ----------------------------------------------------*/
/* ИМЯ: Pogosian Samvel, ID пользователя: 23К0180      */
/* ДАТА: 02/11/2025                                    */
/* НАЗНАЧЕНИЕ ПРОГРАММЫ:                               */
/* Демонстрация отправки системных данных (через msgq). */
/* Процесс сенсора получает информацию о памяти через   */
/* команду vm_stat и передаёт её через очередь сообщений*/
/* для последующего чтения процессом monitor.c.         */
/* ИМЯ ФАЙЛА: sensor.c                                 */
/* ----------------------------------------------------*/
/* ФУНКЦИИ:                                             */
/* main() – создаёт очередь сообщений, формирует и      */
/*          отправляет системные данные каждые 2 секунды*/
/*          до 5 раз, после чего посылает сигнал "end". */
/* ----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

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

    if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    msg.mtype = 1;
    for (int i = 0; i < 5; i++) {
        FILE *fp = popen("vm_stat | grep 'free'", "r");
        if (fp == NULL) {
            perror("popen");
            exit(1);
        }
        fgets(msg.mtext, sizeof(msg.mtext), fp);
        pclose(fp);

        if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1)
            perror("msgsnd");
        else
            printf("Sensor: sent system info -> %s", msg.mtext);

        sleep(2);
    }

    strcpy(msg.mtext, "end");
    msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0);
    printf("Sensor: done sending.\n");
    return 0;
}
