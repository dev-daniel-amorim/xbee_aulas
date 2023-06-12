#include <SoftwareSerial.h>
SoftwareSerial mySerial (16, 17); //RX e TX do esp32


byte dados[50]; // Array para armazenar os dados recebidos
int indice = 0; // Índice para controlar a posição no array
int lenPacote;  // Tamanho do pacote recebido
int iniDadosA;  // Inicio do byte de dados
int fimDadosA;  // Fim do byte de dados

byte endMac[4];                           //Lista para receber os MSB do MAC xbee
byte mac1[4] = {0x41, 0x49, 0x6C, 0x66};  //Xbee 01 MAC
byte mac2[4] = {0x41, 0x49, 0x6C, 0xA0};  //Vbee 02 MAC


void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() {
  if (mySerial.available() > 15) {
    indice=0;
    if (mySerial.read() == 0x7E) {          //Detectar byte de inicio do pacote
      while (mySerial.available()) {        //Enquanto pacote disponivel
        dados[indice] = (mySerial.read());  //Armazenas os bytes recebidos na lista DADOS
        Serial.print(dados[indice], HEX);
        Serial.print(", ");
        indice++;
      }
      Serial.println();

      lenPacote = indice;
      iniDadosA = lenPacote -3;
      fimDadosA = lenPacote -2;
      Serial.print("Tamanho do pacote: ");
      Serial.println(lenPacote);

      int dadosLidos = (dados[iniDadosA] << 8) | dados[fimDadosA];
      int cont = 0;
      for(int i = 7; i < 11; i++){
        endMac[cont] = dados[i];
        cont++;
      }
      //Verifica o MAC de quem enviou o pacote
      bool xbee1 = true;
      for (int i = 0; i < 4; i++) {
        if (mac1[i] != endMac[i]) {
            xbee1 = false;
            break;        
        }
      }
      //Verifica o MAC de quem enviou o pacote
      bool xbee2 = true;
      for (int i = 0; i < 4; i++) {
        if (mac2[i] != endMac[i]) {
            xbee2 = false;
            break;        
        }
      }

      

      if (xbee1){
        Serial.println("Xbee1 enviou:");
      }
      if (xbee2){
        Serial.println("Xbee2 enviou:");
      }
      
      Serial.print("Valor Lido em DEC: ");
      Serial.print(dadosLidos);
      Serial.print(" em HEX: ");
      Serial.println(dadosLidos, HEX);
      Serial.println("-----------------------------------------------------------------------------------");
    }
  }
}