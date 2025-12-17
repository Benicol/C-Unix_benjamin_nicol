
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

ALL = calcul_sequentiel calcul_parallele compteur_dangereux compteur_securise \
      test_mutex producteur_consommateur test_semaphore cache_rwlock test_rwlock

all: $(ALL)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(ALL)