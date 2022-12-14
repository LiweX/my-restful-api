#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <regex.h>
#include "tools.h"

#define PORT 8082

json_t * data;
char ** names;
int users=0;

int callback_post_users (const struct _u_request * request, struct _u_response * response, void * user_data) {
    (void)user_data;
    regex_t reegex;
    int value;

    //parseo de request
    json_t * json_request = ulfius_get_json_body_request(request,NULL);
    json_t * username = json_object_get(json_request,"username");
    json_t * password = json_object_get(json_request,"password");
    const char * user = json_string_value(username);
    const char * pass = json_string_value(password);

    //creación de regex para solo aceptar minuculas
    value = regcomp(&reegex,"[^a-z]",0);
    if(value != 0) printf("RegEx error.");
    
    //comprobación de usuario
    value = regexec(&reegex,user,0,NULL,0);
    if(value == 0){
        json_t * string = json_string("user no tiene solo minusculas.");
        json_t * body = json_object();
        value = json_object_set(body,"mensaje",string);
        if(value != 0) printf("json set object error.");
        ulfius_set_json_body_response(response,200,body);
        return U_CALLBACK_CONTINUE;
    }

    if(usuario_duplicado(strdup(user),users,names)){
        json_t * string = json_string("el usuario ya existe.");
        json_t * body = json_object();
        value = json_object_set(body,"mensaje",string);
        if(value != 0) printf("json set object error.");
        ulfius_set_json_body_response(response,200,body);
        return U_CALLBACK_CONTINUE;
    }

    //comprobación de password
    value = regexec(&reegex,pass,0,NULL,0);
    if(value == 0){
        json_t * string = json_string("pass no tiene solo minusculas.");
        json_t * body = json_object();
        value = json_object_set(body,"mensaje",string);
        if(value != 0) printf("json set object error.");
        ulfius_set_json_body_response(response,200,body);
        return U_CALLBACK_CONTINUE;
    }

    //creacion de request para hacer POST al contador de usuarios
    struct _u_request * aux_request = malloc(sizeof(struct _u_request));
    value = ulfius_init_request(aux_request);
    if(value!=0) printf("json error");
    aux_request->http_verb=strdup("POST");
    aux_request->http_url=strdup("http://contadordeusuarios.com/contador/increment");
    aux_request->timeout = 20;

    struct _u_response * aux_response= malloc(sizeof(struct _u_response));
    
    value = ulfius_init_response(aux_response);
    
    if(value!=0) printf("json error");
    value = ulfius_send_http_request(aux_request,aux_response);
    if(value!=U_OK){
        json_t * string = json_string("fallo del servicio de contador.");
        json_t * body = json_object();
        value = json_object_set(body,"mensaje",string);
        if(value != 0) printf("json set object error.");
        ulfius_set_json_body_response(response,200,body);
        y_log_message(Y_LOG_LEVEL_INFO,"falla del serivcio de contador");
        return U_CALLBACK_CONTINUE;
    }

    //parseo del response obtenido por el request anterior
    json_t * json_aux_request = ulfius_get_json_body_response(aux_response,NULL);
    json_t * json_aux_object = json_object_get(json_aux_request,"description");
    long id = json_integer_value(json_aux_object);

    //preparar el json que sera el response
    json_t * body = json_object();

    char timestamp[100];
    get_timestamp(timestamp);

    value = json_object_set(body,"id",json_integer(id));
    if(value!=0) printf("json id error");
    value = json_object_set(body,"username",json_string(user));
    if(value!=0) printf("json username error");
    value = json_object_set(body,"created_at",json_string(timestamp));
    if(value!=0) printf("json object error");
    ulfius_set_json_body_response(response,200,body);

    //se agrega el usuario a la lista de usuarios.
    json_t * data_object = json_object();
    value = json_object_set_new(data_object,"user_id",json_integer(id));
    if(value!=0) printf("json user_id error");
    value = json_object_set_new(data_object,"username",json_string(user));
    if(value!=0) printf("json username error");
    value = json_array_append_new(data,data_object);
    if(value!=0) printf("json array error");

    //creacion de usuario con pass encriptada
    char cmd[200];
    sprintf(cmd,"useradd -p $(mkpasswd --hash=SHA-512 %s) %s",pass,user);
    system(cmd);
    
    if(users>0){
      int*check = realloc(names,sizeof(char*)*(size_t)users+1);
      if(check == NULL){
        printf("Realloc fail\n");
        exit(EXIT_FAILURE);
      } 
    }
    names[users]=strdup(user);
    users++;

    //rotate_log_check();

    //rutina de logeo
    y_log_message(Y_LOG_LEVEL_INFO,"Usuario %ld creado.",id);

    return U_CALLBACK_CONTINUE;
    
}

int callback_get_users (const struct _u_request * request, struct _u_response * response, void * user_data) {
    request=request;
    (void)user_data;
    int value;
    json_t * json_response = json_object();
    value=json_object_set_new(json_response,"data",data);
    if(value!=0) printf("json data error");
    ulfius_set_json_body_response(response,200,json_response);

    //creacion de request para hacer GET al contador de usuarios
    struct _u_request * aux_request = malloc(sizeof(struct _u_request));
    value = ulfius_init_request(aux_request);
    if(value!=0) printf("json error");
    aux_request->http_verb=strdup("GET");
    aux_request->http_url=strdup("http://localhost:8081/contador/value");
    aux_request->timeout = 20;

    struct _u_response * aux_response= malloc(sizeof(struct _u_response));
    
    value = ulfius_init_response(aux_response);
    
    if(value!=0) printf("json error");
    value = ulfius_send_http_request(aux_request,aux_response);
    if(value!=0) printf("json error");

    //parseo del response obtenido por el request anterior
    json_t * json_aux_request = ulfius_get_json_body_response(aux_response,NULL);
    json_t * json_aux_object = json_object_get(json_aux_request,"description");
    long n_users = json_integer_value(json_aux_object);

    //rotate_log_check();

    //rutina de logeo
    y_log_message(Y_LOG_LEVEL_INFO,"Usuarios creados: %ld.",n_users);
    return U_CALLBACK_CONTINUE;
}

/**
 * main function
 */
int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  names=(char**)malloc(sizeof(char*));

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", "/api/users", NULL, 0, &callback_post_users, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "/api/users", NULL, 0, &callback_get_users, NULL);
  data = json_array();
  // Start the framework
  y_init_logs("laboratorio6.com",Y_LOG_MODE_FILE,Y_LOG_LEVEL_INFO,"/tmp/lab6_logs.log","initializing info logs");
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    while(-1);
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}