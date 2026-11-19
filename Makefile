CC = gcc
CFLAGS = -Wall -Wextra 

TARGET = ordena

FILES = metodo_int_balanceada_2f.c metodo_int_balanceada_f1.c metodo_quicksort.c main.c base.c

all:
	$(CC) $(FILES) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET) Provao_crescente.bin Provao_decrescente.bin Provao_aleatorio.bin Prova_ordenado.txt ordenado.txt fita11.bin fita1.bin

original:
	cp backup/Provao_aleatorio.bin backup/Provao_crescente.bin backup/Provao_decrescente.bin .