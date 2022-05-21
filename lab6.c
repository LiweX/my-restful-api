#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <regex.h>
#include "tools.h"

#define PORT 8082

json_t * data;

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

    //creacion de request para hacer get al contador de usuarios
    struct _u_request * aux_request = malloc(sizeof(struct _u_request));
    value = ulfius_init_request(aux_request);
    if(value!=0) printf("json error");
    aux_request->http_verb=strdup("POST");
    aux_request->http_url=strdup("http://localhost:8081/contador/increment");
    aux_request->timeout = 20;

    struct _u_response * aux_response= malloc(sizeof(struct _u_response));
    
    value = ulfius_init_response(aux_response);
    
    if(value!=0) printf("json error");
    value = ulfius_send_http_request(aux_request,aux_response);
    if(value!=0) printf("json error");

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


    json_t * data_object = json_object();
    value = json_object_set_new(data_object,"user_id",json_integer(id));
    if(value!=0) printf("json user_id error");
    value = json_object_set_new(data_object,"username",json_string(user));
    if(value!=0) printf("json usernaame error");
    value = json_array_append_new(data,data_object);
    if(value!=0) printf("json array error");

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

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", "/api/users", NULL, 0, &callback_post_users, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "/api/users", NULL, 0, &callback_get_users, NULL);
  data = json_array();
  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}