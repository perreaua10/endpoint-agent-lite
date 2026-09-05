// mock_udp_server.js
const dgram = require('dgram');
const server = dgram.createSocket('udp4');
var count = 0;
server.on('message', (msg, rinfo) => {
    count++;
    console.log(`Received request ${count} from ${rinfo.address}:${rinfo.port}: ${msg.toString()}`);
});

server.on('listening', () => {
    const address = server.address();
    console.log(`Mock UDP server listening on ${address.address}:${address.port}`);
});

server.bind(9999); // match whatever port your C++ sender targets