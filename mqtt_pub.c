#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int main(void)
{
    struct mosquitto *mqtt_d;
    int i, mqtt_c;
    char str[32];

    signal(SIGINT, onExit);
    signal(SIGTERM, onExit);

    mosquitto_lib_init();
    mqtt_d=mosquitto_new("mqtt_pub_example", true, 0);

    if(!mqtt_d) {
        printf("MQTT init error.\n");
        return 0;
    }

    mosquitto_connect_callback_set(mqtt_d, mqtt_connect_cb);

    mqtt_c=mosquitto_connect(mqtt_d, "127.0.0.1", 1883, 60);

    if(mqtt_c!=MOSQ_ERR_SUCCESS) {
        printf("MQTT connect error.\n");
        return 0;
    }

    mosquitto_loop_start(mqtt_d);

    i=0;
    run=1;

    while(run) {
        sprintf(str, "Message #%d.", ++i);

        if(mosquitto_publish(mqtt_d, NULL, "/test/event", strlen(str), str, 0, 0)!=MOSQ_ERR_SUCCESS) {
            mosquitto_reconnect(mqtt_d);
            printf("MQTT reconnect \n");
        }
        else printf("MQTT message sent \n");

        sleep(1);
    }

    mosquitto_disconnect(mqtt_d);
    mosquitto_loop_stop(mqtt_d, 1);
		
    mosquitto_destroy(mqtt_d);
    mosquitto_lib_cleanup();

    return 0;
}
