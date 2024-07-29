#pragma once

#ifdef UNIT_TEST
#include "ArduinoFake.h"
#else
#include "Arduino.h"
#endif

#define MessageStart 0x7E
#define MessageVersion 0xFF
#define MessageEnd 0xEF
#define DFPLAYER_RECEIVED_LENGTH 10
#define DFPLAYER_SEND_LENGTH 10

#define Message_Pos_Header 0
#define Message_Pos_Version 1
#define Message_Pos_Length 2
#define Message_Pos_Command 3
#define Message_Pos_ACK 4
#define Message_Pos_Parameter 5
#define Message_Pos_CheckSum 7

enum PlayerMessage : uint8_t { PlayFinished, Online, FeedBack, Error };

enum PlayerDevice : uint8_t { Usb, Card, Flash };

class DFPlayerMini
{
  private:
    Stream* _serial;
    uint8_t _posInReadStream;
    uint8_t _receiveBuffer[DFPLAYER_RECEIVED_LENGTH];
    uint8_t _sendBuffer[DFPLAYER_SEND_LENGTH] = { 0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF };
    bool _isSending;
    PlayerMessage _playerMessage;
    uint16_t _playerMessageParameter;
    unsigned long _timeOutDuration;
    bool _hasCard;
    bool _hasUsb;
    bool _ackMode;

    bool HandleMessage();
    uint16_t calculateCheckSum(uint8_t* buffer);
    bool validateMessage();
    uint16_t arrayToUint16(uint8_t* array);
    void uint16ToArray(uint16_t value, uint8_t* array);

    void sendMessage();
    void sendMessage(uint8_t command);
    void sendMessage(uint8_t command, uint16_t argument);
    void sendMessage(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow);

    void handleMessage(PlayerMessage message, uint16_t parameter);
    bool waitAvailable(unsigned long duration = 0);

  public:
    DFPlayerMini();
    ~DFPlayerMini();

    bool begin(Stream* serial, bool isACK = true, bool doReset = true);
    bool available();
    void reset();
    void playMp3Folder(uint16_t fileNumber);
    PlayerMessage getMessage(uint16_t& parameter);
    void next();
    int readCurrentFileNumber(PlayerDevice device);
    int firmwareVersion();
    void volume(uint8_t volume);
    int getCurrentStatus();
};
