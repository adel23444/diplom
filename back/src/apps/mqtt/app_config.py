import base64
import paho.mqtt.client as mqqt
from django.conf import settings

MAIN_TOPIC = 'esp8266/sensor/'
def on_connect(mqtt_client, userdata, flags, rc):
    if rc == 0:
        print("Успешное подключение")
        mqtt_client.subscribe(MAIN_TOPIC)
    else:
        print("Подключение не удалось", rc)


def on_disconnect(mqtt_client, userdata, rc):
    if rc != 0:
        print("Произошло отключение!")

def on_message(mqtt_client, userdata, msg):
    base64_payload = ''
    try:
        base64_payload = base64.b64decode(msg.payload).decode('UTF-8')
    except:
        base64_payload = msg.payload.decode('UTF-8')


    if "\r\n" in base64_payload:
        base64_payload = base64_payload.split('\r\n')[0]
    msg_dict = {
        'topic': msg.topic,
        'payload': base64_payload
    }
    print(msg_dict)

    # if msg.topic == LEFT_TOPIC:
    #     if "SENL" in base64_payload:
    #         code_name = 'SENL'
    #         index = base64_payload.find(code_name)
    #         print({
    #             'topic': msg.topic,
    #             'payload': base64_payload[index:]
    #         })
    # elif msg.topic == RIGHT_TOPIC:
    #     if "SENR" in base64_payload:
    #         code_name = 'SENR'
    #         index = base64_payload.find(code_name)
    #         print({
    #             'topic': msg.topic,
    #             'payload': base64_payload[index:]
    #         })
    # elif msg.topic == PRIMARY_TOPIC:
    #     if "SENP" in base64_payload:
    #         code_name = 'SENP'
    #         index = base64_payload.find(code_name)
    #         print({
    #             'topic': msg.topic,
    #             'payload': base64_payload[index:]
    #         })


client = mqqt.Client()
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message
client.connect(
    host=settings.MQTT_SERVER,
    port=settings.MQTT_PORT,
    keepalive=settings.MQTT_KEEPALIVE
)