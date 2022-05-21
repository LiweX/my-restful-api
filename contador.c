#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <arpa/inet.h>
#include "tools.h"

#define PORT 8081

int valor=0;

int callback_increment (const struct _u_request * request, struct _u_response * response, void * user_data) {
    struct sockaddr_in *address = (struct sockaddr_in *)(request->client_address);
    char *ip = inet_ntoa(address->sin_addr);
    (void)user_data;
    valor++;
    json_t * body = json_object();
    json_t * code = json_integer(200);
    json_t * value = json_integer(valor);
    int value1 = json_object_set(body,"code",code);
    int value2 = json_object_set(body,"description",value);
    if(value1 !=0 || value2 !=0) printf("json set object error.");
    ulfius_set_json_body_response(response,200,body);

    //rotate_log_check();

    y_log_message(Y_LOG_LEVEL_INFO,"Contador incrementado desde: %s",ip);


    return U_CALLBACK_CONTINUE;
}

int callback_value (const struct _u_request * request, struct _u_response * response, void * user_data) {
    request=request;
    (void)user_data;
    json_t * body = json_object();
    json_t * code = json_integer(200);
    json_t * value = json_integer(valor);
    int value1 = json_object_set(body,"code",code);
    int value2 = json_object_set(body,"description",value);
    if(value1 !=0 || value2 !=0) printf("json set object error.");
    ulfius_set_json_body_response(response,200,body);
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

  y_init_logs("contadordeusuarios.com",Y_LOG_MODE_FILE,Y_LOG_LEVEL_INFO,"/tmp/lab6_logs.log","initializing info logs");  

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "POST", "contador/increment", NULL, 0, &callback_increment, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "contador/value", NULL, 0, &callback_value, NULL);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    while(-1);
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}
