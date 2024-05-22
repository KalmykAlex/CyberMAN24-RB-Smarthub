BEGIN TRANSACTION;

-- Create User table
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS rooms (
    roomid INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL 
);

CREATE TABLE IF NOT EXISTS devices (
    id INTEGER PRIMARY KEY,
    device_name TEXT NOT NULL UNIQUE,
    ip_address TEXT NOT NULL,
    room_id INTEGER NOT NULL,
    device_type TEXT NOT NULL,
    FOREIGN KEY(room_id) REFERENCES rooms(roomid) 
);


INSERT INTO rooms(name, description) VALUES ("F0-101", "Human resources");

INSERT INTO rooms(name, description) VALUES ("F0-102", "Financial");

INSERT INTO rooms(name, description) VALUES ("F0-103", "Data center");

INSERT INTO rooms(name, description) VALUES ("S1-101", "Security");

INSERT INTO rooms(name, description) VALUES ("S1-102", "IT");

INSERT INTO rooms(name, description) VALUES ("S1-103", "Executive board");

INSERT INTO rooms(name, description) VALUES ("S1-104", "Meeting room");

INSERT INTO rooms(name, description) VALUES ("X2-101", "Secret room");



INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_000", "192.168.1.100", (SELECT roomid FROM rooms WHERE name = "F0-101"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_001", "192.168.1.101", (SELECT roomid FROM rooms WHERE name = "F0-101"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_002", "192.168.1.102", (SELECT roomid FROM rooms WHERE name = "F0-101"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_003", "192.168.1.103", (SELECT roomid FROM rooms WHERE name = "F0-102"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_004", "192.168.1.104", (SELECT roomid FROM rooms WHERE name = "F0-102"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_005", "192.168.1.105", (SELECT roomid FROM rooms WHERE name = "F0-102"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_006", "192.168.1.106", (SELECT roomid FROM rooms WHERE name = "F0-103"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_007", "192.168.1.107", (SELECT roomid FROM rooms WHERE name = "F0-103"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_008", "192.168.1.108", (SELECT roomid FROM rooms WHERE name = "F0-103"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_009", "192.168.1.109", (SELECT roomid FROM rooms WHERE name = "S1-101"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_0010", "192.168.1.110", (SELECT roomid FROM rooms WHERE name = "S1-101"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_0011", "192.168.1.111", (SELECT roomid FROM rooms WHERE name = "S1-101"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_0012", "192.168.1.112", (SELECT roomid FROM rooms WHERE name = "S1-102"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_0013", "192.168.1.113", (SELECT roomid FROM rooms WHERE name = "S1-102"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_0014", "192.168.1.114", (SELECT roomid FROM rooms WHERE name = "S1-102"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_0015", "192.168.1.115", (SELECT roomid FROM rooms WHERE name = "S1-103"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_0016", "192.168.1.116", (SELECT roomid FROM rooms WHERE name = "S1-103"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_0017", "192.168.1.117", (SELECT roomid FROM rooms WHERE name = "S1-103"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_0018", "192.168.1.118", (SELECT roomid FROM rooms WHERE name = "S1-104"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_0019", "192.168.1.119", (SELECT roomid FROM rooms WHERE name = "S1-104"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_0020", "192.168.1.120", (SELECT roomid FROM rooms WHERE name = "S1-104"), "Air_Quality_Monitor");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_0021", "10.16.227.148", (SELECT roomid FROM rooms WHERE name = "X2-101"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Bulb_0022", "192.168.1.122", (SELECT roomid FROM rooms WHERE name = "X2-101"), "Smart_Bulb");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Smart_Lock_0023", "192.168.1.123", (SELECT roomid FROM rooms WHERE name = "X2-101"), "Smart_Lock");

INSERT INTO devices(device_name, ip_address, room_id, device_type) VALUES ("Air_Quality_Monitor_0024", "192.168.1.124", (SELECT roomid FROM rooms WHERE name = "X2-101"), "Air_Quality_Monitor");



INSERT INTO users(username, password) VALUES ("sophia.smith", "pufKZGVEUl");

INSERT INTO users(username, password) VALUES ("liam.johnson", "waDJRritSE");

INSERT INTO users(username, password) VALUES ("isabella.miller", "NEloMWZpcv");

INSERT INTO users(username, password) VALUES ("ethan.jones", "eSGZjBPwzD");

INSERT INTO users(username, password) VALUES ("ava.brown", "KwwWMDHQBy");

INSERT INTO users(username, password) VALUES ("noah.davis", "etFcukubXm");

INSERT INTO users(username, password) VALUES ("mia.williams", "yRwPtkQTor");

INSERT INTO users(username, password) VALUES ("mason.wilson", "BGVESENBlV");

INSERT INTO users(username, password) VALUES ("olivia.moore", "UUyiBWpScc");

INSERT INTO users(username, password) VALUES ("edison.taylor", "zVsSZPxJgy");

INSERT INTO users(username, password) VALUES ("admin", "DdAcgyVHce");

INSERT INTO users(username, password) VALUES ("john.snow", "********");


COMMIT;