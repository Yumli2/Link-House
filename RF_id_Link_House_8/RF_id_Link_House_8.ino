// TESTE RFID ID12.ino

//Inclusão de bibliotecas
#include "SoftwareSerial.h"
#include <SD.h>
#define botao 2
SoftwareSerial RFID(12, 11);

//Variaceis Strigs e Char-------------------------------------------------------
String RFleitura="",cartaogravado="",opcao="",Leite="";
char leicartao, leiturabutao,funcaolida;
char lei[17]={};
char valorlido[17]={};

//Classe da biblioteca Sd que ajuda na manipulação de arquivos------------------
//Classe File
File arquivo;
File Doc;
//Condicao para ir para o loop se o cartao sd estiver ok
byte ok;

//Inicio da função Setup--------------------------------------------------------
void setup(){ 
  pinMode(botao,INPUT_PULLUP);
  Serial.begin(9600);
  RFID.begin(9600);
   
  //Não vai pro loop, se não tiver cartão----------------------------------------
  ok=1;
  if(!SD.begin(4)){
    Serial.println("Erro ao iniciar cartao SD");
    Serial.println("Diagnostico: Sem cartao SD");
    ok=1;
    return;
  }
  else{
    Serial.println("Cartao SD Iniciado");
  }
  Serial.println("Passe o cartao de acesso!");
}// Fecha Setup----------------------------------------------------------------

void Cadastro(){
  Serial.println("Passe o cartao novo.");
  while(1){
    if(RFID.available()){
      for(int i=0;i<=15;i++){
        lei[i]=RFID.read();
        delay(10);
      }
      if (sizeof(lei)>=15){
        Leite=lei;
        arquivo=SD.open("cadas.doc",FILE_WRITE);
        if(arquivo){
          arquivo.println(Leite.substring(1,13));
          Serial.print("Cartao salvo com sucesso!: ");
          Serial.println(Leite.substring(1,13));
          //Temos que fechar o arquivo o mais cedo possivel para liberar o ponteiro do arquivo     
          arquivo.close();
          break;
        }
        else{
          Serial.println("Erro ao abrir o arquivo para escrita");
        }
      }
    }//Fecha If RFID available*/
  }//Fecha While flag
 
}//Fecha Funcao Cadastro------------------------------------------------------

void Desabilitar(){

 SD.remove("cadas.doc");
  if(!SD.exists("cadastro.txt")){
    Serial.println("Cartoes apagados com sucesso");
    cartaogravado="";
  }
  else{
    Serial.println("Erro ao apagar os cartoes, tente novamente!");
  } 
}//Fecha Desabilitar-----------------------------------------------------------

void Ativos(){
 arquivo = SD.open("cadas.doc",FILE_READ);
  if (arquivo) {
    Serial.println("Cartoes cadastrado: ");
    while (arquivo.available()) {
    	Serial.write(arquivo.read());
      	
      	

    }
    arquivo.close();
  }
  else{
    Serial.println("Nenhum arquivo encontrado");
  }
}//Fecha Ativos------------------------------------------------------------------- 

void Menu(){
  
  if (digitalRead(botao)==HIGH) {
    //Entra no modo função
    Serial.println("Qual a funcao do menu voce deseja?");
    Serial.println();
    Serial.println("1 - Cadastro de novo cartao.");
    Serial.println();
    Serial.println("2 - Desabilitar cartao.");
    Serial.println();
    Serial.println("3 - Cartoes ativos.");
    Serial.println();
    delay(100);
  }//Fecha if HIGH-------------------------------------------------------------
  delay(10);
  //while(leiturabutao==HIGH){
    while (Serial.available()){
      funcaolida = Serial.read();
      switch (funcaolida) {
        case '1': Cadastro();
        return;
        break;
        case '2': Desabilitar();
        return;
        break;
        case '3': Ativos();
        return;
        break;
        default: Serial.println("Funcao invalida");
        return;
      }//Fecha Switch funcaolida-------------------------------------------------
    }//Fecha While Serial--------------------------------------------------------
  //}//Fecha While HIGH------------------------------------------------------------
}//Fecha Leiturabotao()----------------------------------------------------------

void Verificar(){
 // Faz as comparações com o que esta gravado no SD e com o ponteiro char* cartoes[]  
  AbrirSD(); 
  boolean flag=false; 
  for(int m=0;m<686;m+=14){ 
  	if(RFleitura.substring(1,13).equals(cartaogravado.substring(m,m+12))){
  		flag=true;
  		Serial.print(m); Serial.print(",");
		  Serial.println(m+12);	
  		break;
  	}
  }
  if (flag){
   	
  
    Serial.println("Usuario Valido!");
    Serial.println("Acesso Liberado!");
    Menu();
    //Cargas();
  }//Fecha if -----------------------------------------------------------------
  else{
    Serial.println("Acesso Negado");
    Serial.println("Fale com a Administracao");
    Menu();
    //Recadastro();
  }
}
void Cargas(){
return;
}
void AbrirSD(){
//Abrir arquivo SD -------------------------------------------------------- 
  arquivo=SD.open("cadas.doc");
  if(arquivo){
    while(arquivo.available()){
      leicartao = arquivo.read();                    //leicartao é char
      cartaogravado += leicartao;                    //cartaogravado String
    }//fecha while Sd open---------------------------------------------------
    arquivo.close();
  }//Fecha if arquivo--------------------------------------------------------
  else{
    //Serial.println("Erro ao abrir o arquivo de leitura");
  }
}

void Passatag(){
  //Inicio das leituras dos cartoes ou tags-------------------------------------
  while(RFID.available()){
    for(int j=0;j<=15;j++){
      valorlido[j]= RFID.read();                         //Valolido é char
      delay(10);
    }
    if (sizeof(valorlido)>=15){                          //Verifica o tamanho de valorlido
     Serial.print("Numero do Cartao: ");
      for(int j=1;j<=14;j++){                           //Faz a leitura do array valor lido sem lixo
        Serial.print(valorlido[j]);                      //A posição [0] e [15] são espaços em branco
        RFleitura=valorlido;                             //Armazena na Strnig RFleitura para posterior comparação com cartões guardados no SD     
      }       
      Verificar();
    }//Fecha SizaOf-------------------------------------------------------------
  }//Fecha primeiro While-------------------------------------------------------
}

void loop(){ 
  if(ok){
    Passatag();
    Menu(); 
  }//Fecha Inicio_ok
}//Fecha loop-------------------------------------------------------------------