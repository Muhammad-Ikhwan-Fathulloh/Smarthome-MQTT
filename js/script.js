const client = mqtt.connect('wss://public:public@public.cloud.shiftr.io', {
    clientId: 'javascript'
});

let jumlah_kontak = 4
let jumlah_data_sensor = 2

client.on('connect', function() {
    console.log('connected!');
    client.subscribe('koalawan/iot/message');
    client.subscribe('koalawan/iot/distance');
    client.subscribe('koalawan/iot/temperature');
    client.subscribe('koalawan/iot/humidity');
    client.subscribe('koalawan/iot/status_temp');
    client.subscribe('koalawan/iot/status_humd');

    let subscribe_kontak = []
    for (let i = 1; i <= jumlah_kontak; i++) {
        subscribe_kontak[i] = `koalawan/iot/kontak${i}`
        client.subscribe(subscribe_kontak[i]);
    }
});

client.on('message', function(topic, message) {
    console.log(topic + ': ' + message.toString());
    for (let i = 1; i <= jumlah_kontak; i++) {
        if (topic == `koalawan/iot/kontak${i}`) {
            if (message.toString() == '1') {
                console.log(`Kontak ${i} ON`);
            }else{
                console.log(`Kontak ${i} OFF`);
            }
        }
    }

    if (topic == `koalawan/iot/message`) {
      
    }
    if (topic == `koalawan/iot/distance`) {
        let distance = document.getElementById(`distance`);
        distance.innerHTML = `${message.toString()} CM` 
    }
    if (topic == `koalawan/iot/temperature`) {
        let temperature = document.getElementById(`temperature`);
        temperature.innerHTML = `${message.toString()} °C` 
    }
    if (topic == `koalawan/iot/humidity`) {
        let humidity = document.getElementById(`humidity`);
        humidity.innerHTML = `${message.toString()} %` 
    }
    if (topic == `koalawan/iot/status_temp`) {
        let status_temp = document.getElementById(`status_temp`);
        status_temp.innerHTML = `${message.toString()}` 
    }
    if (topic == `koalawan/iot/status_humd`) {
        let status_humd = document.getElementById(`status_humd`);
        status_humd.innerHTML = `${message.toString()}` 
    }
});

let kontak = []
let control_kontak = []
for (let i = 1; i <= jumlah_kontak; i++) {
    kontak[i] = 0
    control_kontak[i] = document.getElementById(`kontak${i}`);
    control_kontak[i].addEventListener('click', function() {
        if (kontak[i] == 0) {
            kontak[i]++
            client.publish(`koalawan/iot/kontak${i}`, '1')
            control_kontak[i].setAttribute('class', 'btn btn-success')
            control_kontak[i].innerHTML = 'ON'
        }else{
            kontak[i]--
            client.publish(`koalawan/iot/kontak${i}`, '0')
            control_kontak[i].setAttribute('class', 'btn btn-danger')
            control_kontak[i].innerHTML = 'OFF'
        }
    });
}

