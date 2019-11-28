build:
	gcc account.c -o account -L /usr/local/lib -lhiredis

clean:
	rm account