#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <mosquitto.h>

int run;

void onExit(int s)
{
	run=0;
}

void mqtt_connect_cb(struct mosquitto *mqtt_d, void *obj, int result)
{
	printf("connected.\n");
}

void mqtt_subscribe_cb(struct mosquitto *mqtt_d, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	printf("subscribed.\n");
}

void mqtt_message_cb(struct mosquitto *mqtt_d, void *userdata, const struct mosquitto_message *message)
{
    printf("Got message:\n");
	printf("  Topic: %s\n", message->topic);
    printf("  Message Length: %d\n", message->payloadlen); 
    printf("  Message: %s\n", message->payload);
}

int main(void)
{
    struct mosquitto *mqtt_d;
    int mqtt_c;

    signal(SIGINT, onExit);
    signal(SIGTERM, onExit);

    mosquitto_lib_init();
    mqtt_d=mosquitto_new("mqtt_sub_example", true, 0);

    if(!mqtt_d) {
        printf("MQTT init error.\n");
        return 0;
    }

    mosquitto_connect_callback_set(mqtt_d, mqtt_connect_cb);
    mosquitto_message_callback_set(mqtt_d, mqtt_message_cb);
    mosquitto_subscribe_callback_set(mqtt_d, mqtt_subscribe_cb);

    mqtt_c=mosquitto_connect(mqtt_d, "127.0.0.1", 1883, 60);

    if(mqtt_c!=MOSQ_ERR_SUCCESS) {
        printf("MQTT connect error.\n");
        return 0;
    }

    mosquitto_subscribe(mqtt_d, NULL, "/test/event", 0);
    mosquitto_loop_start(mqtt_d);

    run=1;

    while(run) sleep(0);

    mosquitto_disconnect(mqtt_d);
    mosquitto_loop_stop(mqtt_d, 1);
		
    mosquitto_destroy(mqtt_d);
    mosquitto_lib_cleanup();

    return 0;
}
