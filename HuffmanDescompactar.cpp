#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;  

int main ()
{
   //LE ARQUIVO
   FILE *arquivo;
   char caracter[2]="";
   if((arquivo = fopen("saida.txt","r")) == NULL)   printf("Erro ao abrir arquivo!!!\n\n");
   int tamanho_tabela=0, estado=1;
   string consulta="";
   //MAQUINA DE ESTADOS
   //NO ESTADO 1, ELE VAI LER APENAS O PRIMEIRO NUMERO DO ARQUIVO, QUE É O NUMERO DE LINHAS DA TABELA

   do{
        caracter[0] = getc(arquivo);
        if (caracter[0]!= EOF) {
           if (estado==1){
              if (caracter[0] =='\n'){
                 estado=2;
                 tamanho_tabela=atoi(consulta.c_str());
                 consulta="";
                 break;
              }
              consulta+=string(caracter);
           }
        }
   } while(caracter[0] != '\n'); //Esse do-while inteiro apenas lê a primeira linha do arquivo


    int matriz[tamanho_tabela][3];
    int i=0, j=0;
    do{
        caracter[0] = getc(arquivo);
        if (caracter[0]!= EOF) {
           if (estado==2){
              if (caracter[0] == ' '){
                 estado=3;
                 matriz[i][0]=atoi(consulta.c_str());
                 consulta="";
                 j=0;
              }
              else consulta+=string(caracter);
           }
           else if (estado==3){
              if (caracter[0] == '\n'){
                 estado=2;
                 matriz[i][1]=atoi(consulta.c_str());
                 matriz[i][2]=j;
                 consulta="";
                 i++;
              }
              else{
                 consulta+=string(caracter);
                 j++;
              }
           }
        }
        //putchar(caracter);
   } while(i<tamanho_tabela); //Esse laço armazena a tabela
   for (i=0; i<tamanho_tabela; i++) printf("%d\t %d\t %d\n", matriz[i][0], matriz[i][1], matriz[i][2]);
   estado=4;
   int quant_letras=0;
      do{
        caracter[0] = getc(arquivo);
        if (caracter[0]!= EOF) {
           if (estado==4){
              if (caracter[0] =='\n'){
                 estado=5;
                 quant_letras=atoi(consulta.c_str());
                 consulta="";
                 break;
              }
              consulta+=string(caracter);
           }
        }
   } while(caracter[0] != '\n'); //Esse do-while inteiro apenas lê a prox linha do arquivo
   printf("%d\n", quant_letras);
   char tipo=getc(arquivo);
   getc(arquivo); //Só pra passar um ponteiro
   estado=5;
   string saida="";
   i=0;
   j=0;
   int k=0, m=0;
   FILE *p = fopen("entrada.txt","w");
   //fprintf(p,"%c", matriz[m][0]);

   do{
        caracter[0] = getc(arquivo);
        if (caracter[0]!= EOF) {
           if (estado==5){
              k=atoi(consulta.c_str());
              for (m=0; m<tamanho_tabela; m++) if((matriz[m][2]==j) && (matriz[m][1]==k)){
                  i=1;
                  break;
              }
              if (i){
                 
                 i=0;
                 j=0;
                 fprintf(p,"%c", matriz[m][0]);
                 //printf("%c", matriz[m][0]);
                 consulta="";
                 quant_letras--;
              }
                   consulta+=string(caracter);
                   j++;
           }
        }
        else if (quant_letras!=0) {
             k=atoi(consulta.c_str());
             for (m=0; m<tamanho_tabela; m++) if((matriz[m][2]==j) && (matriz[m][1]==k)){
                  fprintf(p,"%c", matriz[m][0]);
                  //COLOCAR NO ARQUIVO
                  break;
             }
        }
        //putchar(caracter);
   } while(caracter[0]!= EOF); //Esse laço armazena a tabela

   //system("PAUSE");
   return 0;
}

