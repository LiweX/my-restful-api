### Lab6 Sistemas Operativos II
## Ingeniería en Computación - FCEFyN - UNC
# Sistemas Embebidos
## Desarrollo
Para cumplir con las consignas dadas primero se modifico el laboratorio anterior para que su salida fuera en formato json. Logrado esto, se instalaron las herramientas necesarias: `ngnix`, `logrotate`, `whois`,etc...

Patiendo del laboratiorio anterior, en un binario aparte se crearon 2 nuevos endpoint que crean y listan los usuarios respectivamente. Para el `POST` se comprueba con una regex que el usuario y contraseña solo contengan minuculas y que el usuario no exista.

Haciendo uso de archivos `.service` se configuraron los servicios necesarios para que el `systemd` pueda levantar los binarios y dejarlos corriendo en segundo plano.

Teniendo los servicios funcionando se configuro el `ngnix` como reverse proxy para que escuche el puerto 80 y redireccione las request al servicio correspondiente usando archivos `.conf` 

Para testear que todo funciona bien, se loguean los usuarios creados con el endpoint usando `ssh`.


