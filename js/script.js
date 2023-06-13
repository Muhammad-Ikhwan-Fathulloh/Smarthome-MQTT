const client = mqtt.connect('wss://public:public@public.cloud.shiftr.io', {
    clientId: 'javascript'
});

client.on('connect', function() {
    console.log('connected!');
    client.subscribe('koalawan/iot');
   
});

client.on('message', function(topic, message) {
    console.log(topic + ': ' + message.toString());
});

let count = 0
document.querySelector('button').addEventListener('click', function() {
    count++
    client.publish('koalawan/iot', 'hello '+count);
});

