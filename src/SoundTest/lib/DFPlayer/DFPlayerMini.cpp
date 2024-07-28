#include "DFPlayerMini.hpp"

bool DFPlayerMini::HandleMessage()
{
    if(!validateMessage()) return false;

    uint8_t handleCommand = *(_receiveBuffer + Message_Pos_Command);
    if (handleCommand == 0x41) { 
      //handle the 0x41 ack feedback as a spcecial case, in case the pollusion of _handleCommand, _handleParameter, and _handleType.
#ifdef _DEBUG
      Serial.println(F("ack received"));
#endif
      _isSending = false;
      return false;
    }

    auto handleParameter = arrayToUint16(_receiveBuffer + Message_Pos_Parameter);

    switch (handleCommand) {
      case 0x3C:
      case 0x3D:
        handleMessage(PlayerMessage::PlayFinished, handleParameter);
        return true;
      case 0x3F:
        if (handleParameter & 0x01) {
          _hasUsb = true;
          handleMessage(PlayerMessage::Online, handleParameter);
        }
        else if (handleParameter & 0x02) {
          _hasCard = true;
          handleMessage(PlayerMessage::Online, handleParameter);
        }
        else if (handleParameter & 0x03) {
          _hasUsb = true;
          _hasCard = true;
          handleMessage(PlayerMessage::Online, handleParameter);
        }
        return true;
      case 0x40:
        handleMessage(PlayerMessage::Error, handleParameter);
        return true;
      case 0x3E:
      case 0x42:
      case 0x43:
      case 0x44:
      case 0x45:
      case 0x46:
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4B:
      case 0x4C:
      case 0x4D:
      case 0x4E:
      case 0x4F:
        handleMessage(PlayerMessage::FeedBack, handleParameter);
        return true;
    }

    return false;
}

uint16_t DFPlayerMini::calculateCheckSum(uint8_t *buffer){
  uint16_t sum = 0;
  for (int i=Message_Pos_Version; i<Message_Pos_CheckSum; i++) {
    sum += buffer[i];
  }
  return -sum;
}

bool DFPlayerMini::validateMessage(){
  return calculateCheckSum(_receiveBuffer) == arrayToUint16(_receiveBuffer+Message_Pos_CheckSum);
}

uint16_t DFPlayerMini::arrayToUint16(uint8_t *array){
  uint16_t value = *array;
  value <<=8;
  value += *(array+1);
  return value;
}

void DFPlayerMini::uint16ToArray(uint16_t value, uint8_t *array){
  *array = (uint8_t)(value>>8);
  *(array+1) = (uint8_t)(value);
}

void DFPlayerMini::sendMessage() {
  if(_isSending) {
    waitAvailable();
  }
  
  if(_sendBuffer[Message_Pos_ACK] == 0x01)   {
    _isSending = true;
#ifdef _DEBUG
    Serial.println("sending is set true");
#endif
  }

#ifdef _DEBUG
  Serial.print(F("sending:"));
  for (int i=0; i<DFPLAYER_SEND_LENGTH; i++) {
    Serial.print(_sendBuffer[i],HEX);
    Serial.print(F(" "));
  }
  Serial.println();
#endif

  _serial->write(_sendBuffer, DFPLAYER_SEND_LENGTH);
}

void DFPlayerMini::sendMessage(uint8_t command){
  sendMessage(command, 0);
}

void DFPlayerMini::sendMessage(uint8_t command, uint16_t argument)
{
  _sendBuffer[Message_Pos_Command] = command;
  uint16ToArray(argument, _sendBuffer+Message_Pos_Parameter);
  
  //Query Comands hast no Ack
  if(_sendBuffer[Message_Pos_Command] >= 0x3C ) {
    _sendBuffer[Message_Pos_ACK] = 0x00;
  } else if (_ackMode)
  {
    _sendBuffer[Message_Pos_ACK] = 0x01;
  }

  uint16ToArray(calculateCheckSum(_sendBuffer), _sendBuffer+Message_Pos_CheckSum);
  sendMessage();
}

void DFPlayerMini::sendMessage(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow)
{
  uint16_t buffer = argumentHigh;
  buffer <<= 8;
  buffer = buffer | argumentLow;
  sendMessage(command, buffer);
}

void DFPlayerMini::handleMessage(PlayerMessage message, uint16_t parameter) {
  _playerMessage = message;
  _playerMessageParameter = parameter;

  //Todo PlayFinish parameter is the Tracknumber
  
#ifdef _DEBUG
  Serial.print(F("Message "));
  Serial.print(message);
  Serial.print(F(" with "));
  Serial.print(parameter);
  Serial.println();
#endif

  if(message == PlayerMessage::Error) {
    switch (parameter)
    {
      case 0x04:
        Serial.println(F("checksum error"));
        break;
      
      default:
        Serial.println(F("unkown error"));
        break;
    }
  }
}

bool DFPlayerMini::waitAvailable(unsigned long duration){
  unsigned long timer = millis();
  if (!duration) {
    duration = _timeOutDuration;
  }
  while (!available()){
    if (millis() - timer > duration) {
      //return handleError(TimeOut);
      return false;
    }
    delay(0);
  }
  return true;
}

DFPlayerMini::DFPlayerMini()
{
    _posInReadStream = 0;
    _serial = nullptr;
    _isSending = false;
    _timeOutDuration = 500;
    _hasCard = false;
    _hasUsb = false;
    _ackMode = true;
}

DFPlayerMini::~DFPlayerMini()
{
}

bool DFPlayerMini::begin(Stream* serial, bool isACK, bool doReset)
{
  _serial = serial;

  if (isACK) {
    _ackMode = true;
    //enableACK();
  }
  else{
    _ackMode = false;
    //disableACK();
  }
  
  if (doReset) {
    reset();
    waitAvailable(2000);
    delay(200);
  }
  else {
    _playerMessage = PlayerMessage::Online;
  }

  return _playerMessage == PlayerMessage::Online;
}

bool DFPlayerMini::available(){
  bool newLine = false;
    while (_serial->available() > 0) {
#ifdef _DEBUG
      if(!newLine) {
        Serial.print(F("received:"));
        newLine = true;
      }
#endif
        delay(0);
        
        auto currentInput = _serial->read();
#ifdef _DEBUG
        Serial.print(currentInput,HEX);
        Serial.print(F(" "));
#endif
        if(currentInput == MessageStart) {
            _posInReadStream = 0;
            _receiveBuffer[_posInReadStream] = MessageStart;
            _posInReadStream++;
            continue;
        } 

        _receiveBuffer[_posInReadStream] = currentInput;
        _posInReadStream++;

        if(_posInReadStream > DFPLAYER_RECEIVED_LENGTH)
        {
            Serial.print(F("Buffer overrun reset buffer"));
            _posInReadStream = 0;
#ifdef _DEBUG
            Serial.println();
            newLine = false;
#endif
            continue;
        }

        if(currentInput == MessageEnd) {
#ifdef _DEBUG
            Serial.println();
#endif
            newLine = false;
            _posInReadStream = 0;
            return HandleMessage();
        }
    }
#ifdef _DEBUG
    if(newLine) Serial.println();
#endif
    return false;
}

void DFPlayerMini::reset(){
  sendMessage(0x0C);
}

void DFPlayerMini::playMp3Folder(uint16_t fileNumber) {
  sendMessage(0x12, fileNumber);
}

PlayerMessage DFPlayerMini::getMessage(uint16_t& parameter) {
  parameter = _playerMessageParameter;
  return _playerMessage;
}

void DFPlayerMini::next() {
  sendMessage(0x01);
}

int DFPlayerMini::readCurrentFileNumber(PlayerDevice device){
  switch (device) {
    case PlayerDevice::Usb:
      sendMessage(0x4B);
      break;
    case PlayerDevice::Card:
      sendMessage(0x4C);
      break;
    case PlayerDevice::Flash:
      sendMessage(0x4D);
      break;
    default:
      return -1;
  }
  if (waitAvailable()) {
    if (_playerMessage == PlayerMessage::FeedBack) {
      return _playerMessageParameter;
    }
    else{
      return -1;
    }
  }
  else {
    return -1;
  }
}

int DFPlayerMini::firmwareVersion(){
  sendMessage(0x46);
  if (waitAvailable()) {
    if (_playerMessage == PlayerMessage::FeedBack) {
      return _playerMessageParameter;
    }
    else{
      return -1;
    }
  }
  else {
    return -1;
  }
}

void DFPlayerMini::volume(uint8_t volume) {
  if(volume > 30) {
    volume = 30;
  }
  sendMessage(0x06, volume);
}