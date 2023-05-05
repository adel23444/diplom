import base64
import paho.mqtt.client as mqqt
from django.conf import settings

def on_connect(mqtt_client, userdata, flags, rc):
    if rc == 0:
        print("Успешное подключение")
        mqtt_client.subscribe('robot/test')
    else:
        print("Подключение не удалось", rc)


def on_message(mqtt_client, userdata, msg):
    base64_payload = ''
    try:
        base64_payload = base64.b64decode(msg.payload).decode('UTF-8')
    except:
        base64_payload = msg.payload
    msg_dict = {
        'topic': msg.topic,
        'payload': base64_payload
    }
    print(msg_dict)

client = mqqt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)
client.connect(
    host=settings.MQTT_SERVER,
    port=settings.MQTT_PORT,
    keepalive=settings.MQTT_KEEPALIVE
)