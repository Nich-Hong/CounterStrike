//so there are 40 pixels total
//each row starts with 0b but the remanin 1/0 are on and off
//#define byte uint8_t

byte firstbar[8] = {
  0b01111,
  0b10000,
  0b10111,
  0b10111,
  0b10111,
  0b10111,
  0b10000,
  0b01111
};

byte midbar[8] = {
  0b11111,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b11111
};

byte endbar[8]={
  0b11110,
  0b00001,
  0b11101,
  0b11101,
  0b11101,
  0b11101,
  0b00001,
  0b11110
};

byte firstempty[8] = {
  0b01111,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b01111
};
byte endempty[8]={
  0b11110,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b00001,
  0b11110
};
byte midempty[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
