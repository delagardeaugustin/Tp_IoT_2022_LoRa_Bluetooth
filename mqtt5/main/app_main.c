#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "lora.h"

static const char *TAG = "MQTT_LoRa_Token";

// Stockage du token en ASCII
static char* token = "-------";  // Initialisation avec des tirets pour signaler un token non reçu



/**
 * @brief Gestionnaire d'événements MQTT
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Connecte au broker MQTT");
        msg_id = esp_mqtt_client_subscribe(client, "SALAD", 1);
        ESP_LOGI(TAG, "Abonnement a SALAD envoye, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Deconnecte du broker MQTT");
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Nouveau message MQTT recu");
        ESP_LOGI(TAG, "Topic: %.*s %s", event->topic_len, event->topic, event->data);

        token=event->data;
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "Erreur MQTT detectee");
        break;

    default:
        ESP_LOGI(TAG, "Autre evenement recu: %d", event->event_id);
        break;
    }
}

/**
 * @brief Tâche d'envoi LoRa toutes les 5 secondes avec le token en ASCII
 */
void task_tx(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
	while(1) {
		int send_len = sprintf((char *)buf, "%.8s", token);
		lora_send_packet(buf, send_len);
		ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent...", send_len);
		int lost = lora_packet_lost();
		if (lost != 0) {
			ESP_LOGW(pcTaskGetName(NULL), "%d packets lost", lost);
		}
		vTaskDelay(pdMS_TO_TICKS(5000));
	} // end while
}

/**
 * @brief Initialise et démarre le client MQTT
 */
static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

/**
 * @brief Fonction principale de l'application
 */
void app_main(void) {
    ESP_LOGI(TAG, "Demarrage de l'application...");
    ESP_LOGI(TAG, "IDF Version: %s", esp_get_idf_version());

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

    // Initialisation de LoRa
	if (lora_init() == 0) {
		ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
		while(1) {
			vTaskDelay(1);
		}
	}


	ESP_LOGI(pcTaskGetName(NULL), "Frequency is %dMHz", 868);
	long frequency = 868 * 1000000;
	lora_set_frequency(frequency);

	lora_enable_crc();

	int cr = 1;
	int bw = 7;
	int sf = 7;

	lora_set_coding_rate(cr);
	//lora_set_coding_rate(CONFIG_CODING_RATE);
	//cr = lora_get_coding_rate();
	ESP_LOGI(pcTaskGetName(NULL), "coding_rate=%d", cr);

	lora_set_bandwidth(bw);
	//lora_set_bandwidth(CONFIG_BANDWIDTH);
	//int bw = lora_get_bandwidth();
	ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);

	lora_set_spreading_factor(sf);
	//lora_set_spreading_factor(CONFIG_SF_RATE);
	//int sf = lora_get_spreading_factor();
	ESP_LOGI(pcTaskGetName(NULL), "spreading_factor=%d", sf);

    
    // Démarrer MQTT
    mqtt_app_start();

    // Démarrer la tâche LoRa
    xTaskCreate(task_tx, "task_tx", 4096, NULL, 5, NULL);
}
