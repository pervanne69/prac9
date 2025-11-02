/* ----------------------------------------------------*/
/* ИМЯ: Pogosian Samvel, ID пользователя: 23К0180      */
/* ДАТА: 02/11/2025                                    */
/* НАЗНАЧЕНИЕ ПРОГРАММЫ:                               */
/* Демонстрация записи данных в разделяемую память.     */
/* Процесс writer получает текущую дату и время и       */
/* помещает их в сегмент памяти, доступный reader.c.    */
/* ИМЯ ФАЙЛА: writer.c                                 */
/* ----------------------------------------------------*/
/* ФУНКЦИИ:                                             */
/* main() – создаёт разделяемую память, записывает      */
/*          строку с датой/временем каждые 2 секунды,   */
/*          завершает работу при записи "end".          */
/* ----------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define BUF_SIZE 256

struct shmseg {
    int updated;
    char data[BUF_SIZE];
};

int main(void) {
    int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    struct shmseg *shmp = shmat(shmid, NULL, 0);
    if (shmp == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    for (int i = 0; i < 5; i++) {
        FILE *fp = popen("date", "r");
        fgets(shmp->data, BUF_SIZE, fp);
        pclose(fp);
        shmp->updated = 1;
        printf("Writer: wrote '%s'\n", shmp->data);
        sleep(2);
    }

    strcpy(shmp->data, "end");
    shmp->updated = 1;
    shmdt(shmp);
    return 0;
}
