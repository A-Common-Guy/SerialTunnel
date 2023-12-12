#include "multiserial.h"

void SerialTunnel::addSendChannel(int type, int* variable) {
  sendVariables[numSendChannels] = variable;
  sendChannelList[numSendChannels] = type;
  numSendChannels++;
}

void SerialTunnel::addRecvChannel(int type, int* variable) {
  recvVariables[numRecvChannels] = variable;
  recvChannelList[numRecvChannels] = type;
  numRecvChannels++;
}


void SerialTunnel::recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= BUFFERSIZE) {
          ndx = BUFFERSIZE - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        //Serial.println("data received");

        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

int SerialTunnel::parseData() {  // split the data into its parts
  strcpy(tempChars, receivedChars);
  char* strtokIndx;  // this is used by strtok() as an index

  for (int i = 0; i < numRecvChannels; i++) {

    //Serial.println("starting processing");
    if(i==0){
      strtokIndx = strtok(tempChars, divider);
    }
    else{
      strtokIndx = strtok(NULL, divider);
    }
    if (!strtokIndx) {
      //Serial.println("Bad");
      return 1;
    }

    if (recvChannelList[i] == STRING) {
      /*
      strtokIndx = strtok(tempChars, "-");  // get the first part - the string
      if (!strtokIndx) return 1;
      strcpy(command, strtokIndx);  // copy it to messageFromPC
      */
    } else if (recvChannelList[i] == INT) {


      *(recvVariables[i]) = atoi(strtokIndx);  // convert this part to an integer
      //result = atoi(strtokIndx);
      //Serial.print("result:");
      //Serial.println(*(recvVariables[i]));
    }
  }
  return 0;
}

void SerialTunnel::replyToPython() {
  if (newData == true) {
    Serial.print(startMarker);

    for(int i=0;i< numSendChannels;i++){
      Serial.print(*(sendVariables[i]));
      if(!(i==(numSendChannels-1))){
        Serial.print(divider);
      }
    }


    Serial.print(endMarker);
    newData = false;
  }
}

void SerialTunnel::begin() {
  Serial.println("<Arduino is ready>");
}

void SerialTunnel::handleTunnel() {
  if (Serial.available() > 0) {
    //Serial.println("Starting data processing");
    recvWithStartEndMarkers();
    parseData();
    replyToPython();
  }
}
