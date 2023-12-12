#ifndef MULTISERIAL_H
#define MULTISERIAL_H

#define MAXCHANNEL 20
#define BUFFERSIZE 128

#include <Arduino.h>

class SerialTunnel {

public:
  //buffers
  char receivedChars[BUFFERSIZE];
  char tempChars[BUFFERSIZE];
  boolean newData = false;


  //supported types
  static const int INT = 0;
  static const int STRING = 1;

  int result=88;

  //channel divider
  char divider[2] = "-";
  char startMarker = '<';
  char endMarker = '>';


  //configuration function
  void addSendChannel(int, int*);
  void addRecvChannel(int, int*);
  void setDivider(char);
  void begin();
  void handleTunnel();

  //containers
  int* sendVariables[MAXCHANNEL];
  int* recvVariables[MAXCHANNEL];

private:
  int recvChannelList[MAXCHANNEL];
  int numRecvChannels = 0;

  int sendChannelList[MAXCHANNEL];
  int numSendChannels = 0;

    //functional blocks
  void recvWithStartEndMarkers();
  int parseData();
  void replyToPython();
  
};




#endif