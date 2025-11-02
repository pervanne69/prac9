# -----------------------------------------------------
# Makefile для Практической работы №9
# Автор: Pogosian Samvel (23К0180)
# Дата: 02/11/2025
# -----------------------------------------------------

CC = gcc

# Цели по умолчанию
all: sensor monitor writer reader sharmem shm_write shmem_read msgq_send msgq_recv

# Сборка каждой программы
sensor: sensor.c
	$(CC) sensor.c -o sensor

monitor: monitor.c
	$(CC) monitor.c -o monitor

writer: writer.c
	$(CC) writer.c -o writer

reader: reader.c
	$(CC) reader.c -o reader

sharmem: sharmem.c
	$(CC) sharmem.c -o sharmem

shm_write: shm_write.c
	$(CC) shm_write.c -o shm_write

shmem_read: shmem_read.c
	$(CC) shmem_read.c -o shmem_read

msgq_send: msgq_send.c
	$(CC) msgq_send.c -o msgq_send

msgq_recv: msgq_recv.c
	$(CC) msgq_recv.c -o msgq_recv

# Очистка скомпилированных файлов
clean:
	rm -f sensor monitor writer reader sharmem shm_write shmem_read msgq_send msgq_recv msgq.txt
