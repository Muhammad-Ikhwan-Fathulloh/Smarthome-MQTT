import time
from umqtt.simple import MQTTClient
import network

# Wi-Fi
ssid = "ssid"
password = "pass"

# MQTT
mqtt_server = "public.cloud.shiftr.io"
client_id = "esp32"
mqtt_user = "public"
mqtt_pass = "public"

# Callback untuk menerima pesan
def message_received(topic, msg):
    print(topic + ": " + msg)

# Fungsi untuk menghubungkan ke Wi-Fi
def connect_wifi():
    station = network.WLAN(network.STA_IF)
    station.active(True)
    station.connect(ssid, password)
    while not station.isconnected():
        pass
    print("Connected to Wi-Fi")

# Fungsi untuk menghubungkan ke broker MQTT
def connect_mqtt():
    client = MQTTClient(client_id, mqtt_server, user=mqtt_user, password=mqtt_pass)
    client.set_callback(message_received)
    client.connect()
    print("Connected to MQTT broker")
    client.subscribe(b"hello")
    return client

# Setup
connect_wifi()
mqtt_client = connect_mqtt()

# Loop utama
while True:
    try:
        mqtt_client.check_msg()
        time.sleep(1)
    except OSError:
        connect_wifi()
        mqtt_client = connect_mqtt()
