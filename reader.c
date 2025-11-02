/* ----------------------------------------------------*/
/* ИМЯ: Pogosian Samvel, ID пользователя: 23К0180      */
/* ДАТА: 02/11/2025                                    */
/* НАЗНАЧЕНИЕ ПРОГРАММЫ:                               */
/* Демонстрация чтения данных из разделяемой памяти.    */
/* Процесс reader ожидает обновлений и выводит          */
/* полученные строки, завершая работу при "end".        */
/* ИМЯ ФАЙЛА: reader.c                                 */
/* ----------------------------------------------------*/
/* ФУНКЦИИ:                                             */
/* main() – подключается к разделяемой памяти, ждёт     */
/*          обновлений флага updated, выводит данные и  */
/*          завершает работу после получения "end".     */
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
    int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    struct shmseg *shmp = shmat(shmid, NULL, 0);
    if (shmp == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Reader: waiting for updates...\n");
    while (1) {
        if (shmp->updated) {
            if (strcmp(shmp->data, "end") == 0)
                break;
            printf("Reader: read '%s'\n", shmp->data);
            shmp->updated = 0;
        }
        sleep(1);
    }

    shmdt(shmp);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Reader: finished.\n");
    return 0;
}
