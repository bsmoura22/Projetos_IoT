//Inclui as bibliotecas do SPI e do RC522
#include <SPI.h>
#include <MFRC522.h>

//Definição dos pinos de conexão do projeto 
#define PINO_SS 9
#define PINO_RST 8
#define led_buzzer 13
#define led_n 7
#define led_a 6

int cont = 0;

//Cria o item para configurar o módulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST); 

void setup() {
   Serial.begin(9600); // Inicializa a serial
   SPI.begin();// Inicializa a comunicação SPI
   mfrc522.PCD_Init(); // Inicializa o módulo MFRC522
   
   pinMode(led_buzzer, OUTPUT);
   pinMode(led_n, OUTPUT);
   pinMode(led_a, OUTPUT);
   Serial.println("Sistema Inicializado: Aproxime o token");
   
}
void loop() {
   digitalWrite(led_n, HIGH);
   digitalWrite(led_a, LOW);
   
   if (!mfrc522.PICC_IsNewCardPresent()) return;// Aguarda a aproximação do token
   
   if (!mfrc522.PICC_ReadCardSerial()) return; // Seleciona qual token vai ser utilizado 
   
   Serial.print("UID da tag:"); // Mostra UID do token na serial
   String conteudo= ""; //Cria uma variável vazia, do tipo string
   byte letra;
   
   for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10){
      Serial.print(" 0");
    }
    else{
    Serial.print(" ");
    }
    
    Serial.print(mfrc522.uid.uidByte[i], HEX);// Printa a mensagem convertida em hexadecimal
    if(mfrc522.uid.uidByte[i] < 0x10){
      conteudo.concat(String(" 0"));
    }
    else{
      conteudo.concat(String(" "));
    }
   conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
   }
   
   Serial.println();
   conteudo.toUpperCase();//Coloca todas as letras da string em maiúscula
   
   if (conteudo.substring(1) == "13 4E AA A2") {
      digitalWrite(led_n, LOW);
      digitalWrite(led_a, HIGH);
      Serial.println("Acesso Permitido!");
      delay(10000);
      cont = 0;
   }else{
      cont = cont + 1;
      Serial.print("Cartão inválido! Tentativa: ");
      Serial.println(cont);
      
      if( cont == 5 ){
        Serial.println("SISTEMA BLOQUEADO");
        digitalWrite(led_buzzer, HIGH);
        cont = 0;
        
        for(int j = 0; j < 10; j++){
          digitalWrite(led_n, LOW);
          delay(1000);
          digitalWrite(led_n, HIGH);
          delay(1000);
        }

         digitalWrite(led_buzzer, LOW);
         return;
      }
      
      for(int j = 0; j < 3; j++){
        digitalWrite(led_n, LOW);
        delay(1000);
        digitalWrite(led_n, HIGH);
        delay(1000);

      }
      
   }
   
   delay(1000);
}
