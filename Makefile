CC = gcc
CFLAGS = -Wall -Wextra 

TARGET = ordena

all:
	$(CC) *.c -o $(TARGET) -Wall

clean:
	rm -f $(TARGET) Provao_crescente_ordenado.txt Provao_decrescente_ordenado.txt Provao_aleatorio_ordenado.txt Prova_ordenado.txt
