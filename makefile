CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11

all: lab6 contador

lab6: lab6.c tools.o
	gcc $(CFLAGS) lab6.c -o lab6 tools.o -lulfius -ljansson -lyder

contador: contador.c tools.o
	gcc $(CFLAGS) contador.c -o contador tools.o -lulfius -ljansson -lyder

tools.o: tools.c tools.h
	gcc $(CFLAGS) -c tools.c 

install:
	sudo cp lab6 /bin/lab6
	sudo chmod +x /bin/lab6
	sudo cp contador /bin/contador
	sudo chmod +x /bin/contador
	sudo cp lab6.service /etc/systemd/system/lab6.service
	sudo cp contador.service /etc/systemd/system/contador.service
	sudo chmod 644 /etc/systemd/system/lab6.service
	sudo chmod 644 /etc/systemd/system/contador.service
	sudo systemctl daemon-reload
	sudo systemctl start lab6
	sudo systemctl daemon-reload
	sudo systemctl start contador
	sudo systemctl enable lab6
	sudo systemctl enable contador
	sudo cp reverse-proxy.conf /etc/nginx/conf.d/reverse-proxy.conf
	sudo service nginx configtest
	sudo service nginx restart

uninstall:
	sudo systemctl stop lab6
	sudo systemctl stop contador
	sudo rm /bin/lab6
	sudo rm /bin/contador
	sudo rm /etc/systemd/system/lab6.service
	sudo rm /etc/systemd/system/contador.service
	sudo rm /etc/nginx/conf.d/reverse-proxy.conf
	sudo service nginx configtest
	sudo service nginx restart