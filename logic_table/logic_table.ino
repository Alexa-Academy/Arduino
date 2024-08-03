/*************************************************************
Arduino sketch that read a JSON file with the confeguration
of inputs and output of a logic funcion ad create the logic
table with all the combination of inputs

Paolo Godino © 2024
Alexa Academy
***************************************************************/
#include <ArduinoJson.h>
#include "json.h"

typedef struct {
  String name;
  int pin;
} connection_t;

int num_inputs=0;
int num_outputs=0;

connection_t input_conn[10];
connection_t output_conn[5];

void readJsonAndSetupPins() {
  JsonDocument doc;
  deserializeJson(doc, json);

  JsonArray input = doc["input"];

  int i=0;
  for (JsonObject el : input) {
    String name = el["name"];
    String pin = el["pin"];

    pinMode(pin.toInt(), OUTPUT);

    input_conn[i].name = name;
    input_conn[i].pin = pin.toInt();
    ++i;
  }
  num_inputs = i;

  i=0;
  JsonArray output = doc["output"];
  for (JsonObject el : output) {
    String name = el["name"];
    String pin = el["pin"];

    pinMode(pin.toInt(), INPUT);

    output_conn[i].name = name;
    output_conn[i].pin = pin.toInt();
    ++i;
  }
  num_outputs=i;
}

void printHeader() {
  for (int i=0; i<num_inputs; ++i) {
    Serial.print(input_conn[i].name + "  ");
  }
  Serial.print(" ");
  for (int i=0; i<num_outputs; ++i) {
    Serial.print(output_conn[i].name + "  ");
  }

  Serial.println();
}

void printHeaderLine() {
  for (int i=0; i<num_inputs; ++i) {
    for(int j=0; j< input_conn[i].name.length(); ++j) {
      Serial.print("-");
    }
    Serial.print("--");
  }
  Serial.print("-");
  for (int i=0; i<num_outputs; ++i) {
    for(int j=0; j< output_conn[i].name.length(); ++j) {
      Serial.print("-");
    }
    if (i < num_outputs-1)
      Serial.print("--");
  }

  Serial.println();
}

void setup() {
  Serial.begin(9600);

  Serial.println();

  readJsonAndSetupPins();

  printHeader();
  printHeaderLine();
  
  for (int i=0; i<pow(2,num_inputs); ++i) {
    for (int k=0; k<num_inputs; ++k) {
      int val = i >> (num_inputs-k-1) & 1;
      digitalWrite(input_conn[k].pin, val);
      Serial.print(val);
      for(int j=0; j< input_conn[k].name.length()-1; ++j) {
        Serial.print(" ");
      }
      Serial.print("  ");
    }

    delay(50);

    Serial.print(" ");
    for (int k=num_outputs-1; k>=0; --k) {
      int val = digitalRead(output_conn[k].pin);
      Serial.print(val);
      for(int j=0; j< output_conn[k].name.length()-1; ++j) {
        Serial.print(" ");
      }
      Serial.print("  ");
    }
    
    Serial.println();
  }
}

void loop() {
}
