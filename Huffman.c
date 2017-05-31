#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct no{ 
	int representacao; 
	int probabilidade;
	struct no *prox;
	struct no *esq;
	struct no *dir;
	int huffinho;
	int altura;
}tNo;

typedef struct lista{
	struct no *cabeca;
	int tamanho;
}tLista;

typedef tNo *tArvBin;

void criaArv (tArvBin *T); //Cria uma �rvore Bin�ria
void cria (tLista *lista); //Cria uma lista
int vazia (tLista lista); //Verifica se a lista est� vazia
int tamanho (tLista lista); //Verifica o tamanho da lista
int elemento (tLista lista, int pos, int *representacao, int *probabilidade); //Retorna dados de um elemento
tNo *enderecoNo (tLista lista, int representacao); //Retorna o endereco do n�, de acordo com a sua representacao na ASCII
int insere (tLista *lista, int representacao, int probabilidade); //Insere um n� na lista ordenamente
int insereListaVazia (tLista *lista, int representacao, int probabilidade);
int insereInicioLista(tLista *lista, int representacao, int probabilidade);
int insereMeioLista(tLista *lista, int pos, int representacao, int probabilidade);
int insereFimLista(tLista *lista, int representacao, int probabilidade);
int removeInicioLista(tLista *lista, int *representacao, int *probabilidade); //Remove o primeiro item da lista
tNo* removeFimLista(tLista *lista); //Remove o �ltimo item da lista, e retorna seu endere�o, para colocar na �rvore
int removeMeioLista(tLista *lista, int pos, int *representacao, int *probabilidade);
int remover(tLista *lista, int pos, int *representacao, int *probabilidade);  //Remove um elemento pela posi��o
void exibe (tLista lista); //Imprime o tamanho da lista, e todos os seus membros
void exibeInOrdem(tArvBin T); //Imprime a �rvore In Ordem
void achaHuffinhos(tArvBin T, int matriz[][3], int caracteres, int *i); //Acha os n�meros correspondentes de cada caracter, usando Pre Ordem
char bintoASCII(char binario[8]);

int main(int argc, char** argv) {
	
	char caracter, teste;
    FILE *arquivo;
    int freq[128];
    int i;

    //Zerando vetor de frequencia
    for( i=0; i< 128; i++){
         freq[i]=0;;
    }
	    
    //L� Arquivo
    if((arquivo = fopen("entrada.txt","r")) == NULL){
        printf("Erro ao abrir arquivo!!!\n\n"); 
      };
    
    //Conta Quantidade De Vezes Que Caractere Eh Repetido, Jogando No Array
    int total_caracteres=0;
    
    printf("\n");
    printf("Conteudo do Arquivo lido: ");
    do
      {
        caracter = getc(arquivo);
        if (caracter != EOF) {
           total_caracteres++;
           freq[(int)caracter]++;
           int d = caracter;
           char c = d;
           printf("%c", c); //Imprime os caracteres lido no arquivo de entrada!
        }
      }
    while(caracter != EOF); //EOF (Fim de Arquivo)
    fseek ( arquivo , 0 , SEEK_SET ); //Zera o ponteiro que getc() incrementou

    //Probabilidade em porcentagem
    float freq_porcentagem [128];
    for( i=0; i< 128; i++){
         freq_porcentagem[i]= (float) freq[i]/ (float) total_caracteres;
        if(freq_porcentagem[i] != 0){
            //printf("%.2f%%\n", freq_porcentagem[i] * 100); //IMPRIME NO TERMINAL
        }
    }
    
    //Passando dados para uma lista
    tLista *lista = malloc(sizeof(tLista));

	cria(lista);
	for(i=0; i<128; i++){
		if (freq[i]!=0) insere (lista, i, freq[i]);
	}
    const int caracteres_diferentes = tamanho(*lista);
    //Criando a �rvore e passando os itens da lista para a �rvore
    tNo* No3;
    
    printf("\n============ TABELA ===============\n");
    exibe (*lista);
    printf("===================================\n\n");

    while(tamanho(*lista)!=1){
        tNo* No1= removeFimLista(lista);
        tNo* No2 = removeFimLista(lista);
        insere(lista, -1, No1->probabilidade + No2->probabilidade);
        No3= enderecoNo(*lista, -1);
        No3->representacao=-2;
        No3->esq=No1;
        No3->dir=No2;
    }
    
    tArvBin huffman;
    huffman = No3;
    int matriz[caracteres_diferentes][3];
    int checkup[caracteres_diferentes];
    for (i=0; i<caracteres_diferentes; i++) checkup[i]=0;
    i=0;
    achaHuffinhos(huffman, matriz, caracteres_diferentes, &i);

    //At� aqui, armazenado com sucesso na matriz
    
    //Vai armazenar os c�digos no novo arquivo
    int binario[8];
    int pointer=0, pointer2=0;
    FILE *p = fopen("saida.txt","w");
    fprintf(p,"%d", caracteres_diferentes);         
    int k=0;
    //Imprime A Tabela No Arquivo, Usando A Matriz
    for (i=0; i<caracteres_diferentes; i++){
        int huffinho=matriz[i][1];
        int quantidade_bytes=matriz[i][2];
        int temp[quantidade_bytes]; //Vetor De Binario Com O Codigo De Huffman
        //Transforma Para Binario
        for(k=quantidade_bytes-1; k>=0; k--){
            if(pow(2,k)>huffinho) temp[k]=0;
            else{
                 huffinho-=pow(2,k);
                 temp[k]=1;
            }
        }
        //Aqui temos temp[] que � o binario que representa o caracter,  por huff
        fprintf(p,"\n%d \t", matriz[i][0]);
        for (k=quantidade_bytes-1; k>-1; k--) 
            fprintf(p,"%d", temp[k]);
    }
    //Imprime O Total De Caracteres E Escolhe O Modo T (texto)
    fprintf(p,"\n%d\nT\n", total_caracteres);
    //Logo Abaixo Imprime Os Bits No Arquivo De Sa�da Em Modo De Texto 
    do{
        caracter = getc(arquivo);
        if (caracter!= EOF) {
           for (i=0; i<caracteres_diferentes; i++) if ( (char)matriz[i][0]==caracter) break;
           int quantidade_bytes=matriz[i][2];
           int huffinho=matriz[i][1];
           int temp[quantidade_bytes];
           int j=quantidade_bytes-1;
           for(i=j; i>=0; i--){
                 if(pow(2,i)>huffinho) temp[i]=0;
                 else{
                      huffinho-=pow(2,i);
                      temp[i]=1;
                 }
           }
           //for (i=0; i<=j; i++){ printf("%d", temp[i]); } //Imprime no terminal em modo texto
           pointer=0;
           pointer2=quantidade_bytes;
           while(1){
                 pointer++;
                 pointer2--;
                 fprintf(p,"%d", temp[pointer2]); //Aqui Faz A Impressao Dos Bits No Modo Texto
                 if (pointer==8){ //se encher o byte, armazena no arquivo, e enche de novo
                      //Coloca No Novo Arquivo Como Binario
                      pointer=0;
                 }
                 if (pointer2==0) break; //Hora De Pegar Outro Caracter
           } //Ver Caso Que Sobrou Bits
        }//Fim do if maior
      }while(caracter != EOF);

    exibeInOrdem(huffman);
    
    fclose(arquivo);
    //system("PAUSE");
    
    printf("\n\n"); //S� PRA ORGANIZAR
    
    return(0);
}//Fim do main()


void criaArv (tArvBin *T) {
     *T = NULL;
}

void cria (tLista *lista){
	lista->cabeca=NULL;
	lista->tamanho=0;
}

int vazia (tLista lista){
	if(lista.tamanho==0) return 1;
	else return 0;
}

int tamanho (tLista lista){
	return lista.tamanho;
}

int elemento (tLista lista, int pos, int *representacao, int *probabilidade){
	tNo *p = lista.cabeca;
	int n=1;
	if (lista.cabeca==NULL) return 0;
	while ((p !=NULL) && (n < pos)){
		p=p->prox;
		n++;
	}
	if (p==NULL) return 0;
	*representacao = p->representacao;
	*probabilidade = p->probabilidade;
}

tNo *enderecoNo (tLista lista, int representacao){
    if( lista.cabeca ==NULL) return NULL;
    tNo *p = lista.cabeca;
    while(p!=NULL){
                   if (p->representacao == representacao) return (p);
                   p=p->prox;
    }
}


int insere (tLista *lista, int representacao, int probabilidade){ //Ja Insere O Item Ordenando
	tNo *novoNo;
	if (vazia(*lista)) return (insereListaVazia(lista, representacao, probabilidade));
	novoNo = lista->cabeca;
	int posicao=1;
	int ret;
	while (novoNo != NULL){
		if (probabilidade<=(novoNo->probabilidade)){
			posicao++;
		}
		else break;
		novoNo = novoNo->prox;
	}
	if (posicao==1){
		ret = insereInicioLista(lista, representacao, probabilidade);
		return ret;
	}
	if (posicao==lista->tamanho+1){
		ret= insereFimLista(lista, representacao, probabilidade);
		return ret;
	}
	else{
		ret = insereMeioLista(lista, posicao, representacao, probabilidade);
		return ret;
	}
}

int insereListaVazia (tLista *lista, int representacao, int probabilidade){ //Representa��o e Percentual
	tNo *novoNo;
	novoNo = malloc(sizeof(tNo));
	if (novoNo ==NULL) return 0;
	novoNo->representacao = representacao;
	novoNo->probabilidade = probabilidade;
	novoNo->prox = NULL;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->huffinho=0;
	novoNo->altura=0;
	lista->cabeca=novoNo;
	lista->tamanho++;
	return 1;
}

int insereInicioLista(tLista *lista, int representacao, int probabilidade){
	tNo *novoNo;
	novoNo = (tNo *) malloc (sizeof(tNo));
	if (novoNo == NULL) return 0;
	novoNo->representacao = representacao;
	novoNo->probabilidade = probabilidade;
	novoNo->prox = lista->cabeca;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->huffinho=0;
	novoNo->altura=0;
	lista->cabeca = novoNo;
	lista->tamanho++;
	return 0;
}

int insereMeioLista(tLista *lista, int pos, int representacao, int probabilidade){
	tNo *p = lista->cabeca;
	tNo *novoNo;
	int n = 1;
	while ((n<pos-1)&&(p!=NULL)){
 		p = p->prox;
		n++;
	}
	if (p == NULL) {return 0;} // pos. inv�lida
	novoNo = malloc(sizeof(tNo));
	if (novoNo == NULL) {return (0); }// mem. insuf
	novoNo->representacao = representacao;
	novoNo->probabilidade = probabilidade;
	novoNo->prox = p->prox;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->huffinho=0;
	novoNo->altura=0;
	p->prox = novoNo;
	lista->tamanho++;
	return 1;
}

int insereFimLista(tLista *lista, int representacao, int probabilidade){
	tNo *novoNo;
	novoNo = (tNo *) malloc(sizeof(tNo));
	if (novoNo == NULL) return -1;
	novoNo->representacao = representacao;
	novoNo->probabilidade = probabilidade;
	novoNo->esq = NULL;
	novoNo->huffinho=0;
	novoNo->altura=0;
	novoNo->dir = NULL;
	tNo *final = lista->cabeca;
	while(final->prox != NULL)	final = final->prox;
	final->prox = novoNo;
	novoNo->prox = NULL;
	lista->tamanho++;
	return 0;
}


int remover(tLista *lista, int pos, int *representacao, int *probabilidade){
	int ret;
	if (vazia(*lista)) return (0);
	if (pos == 1){
		ret = removeInicioLista(lista, representacao, probabilidade);
		return ret;
	}
	else{
		ret = removeMeioLista(lista, pos, representacao, probabilidade);
		return ret;
	}
}

int removeInicioLista(tLista *lista, int *representacao, int *probabilidade){
	tNo *p = lista->cabeca;
	*representacao = p->representacao; // dado recebe o dado removido
	*probabilidade = p->probabilidade;
	lista->cabeca = p->prox;
	lista->tamanho--;
	return 1;
}

tNo* removeFimLista(tLista *lista){
     tNo *aux, *p = lista->cabeca;
     int n=1;
     while((n<=tamanho(*lista)-1)&&(p!=NULL)){
		aux = p;
		p = p -> prox;
		n++;
	}
	aux->prox = NULL;
	lista->tamanho--;
	return p;
}

int removeMeioLista(tLista *lista, int pos, int *representacao, int *probabilidade){
	tNo *aux, *p = lista->cabeca;
	int n = 1;
	while((n<=pos-1)&&(p!=NULL)){
		aux = p;
		p = p -> prox;
		n++;
	}
	if (p == NULL) {return (0);} // pos. inv�lida
	*representacao = p->representacao; // dado recebe o dado removido
	*probabilidade = p->probabilidade;
	aux->prox = p->prox;
	lista->tamanho--;
	return (1);
}


void exibe (tLista lista){ 
	int t = tamanho(lista);
	printf("Caracteres sem repeticoes: %d \n\n", t);
	int i, representacao, probabilidade;
	for (i=1; i<=t; i++){
		elemento (lista, i, &representacao, &probabilidade);
        int d = representacao;
        char c = d;
        if(representacao == '\n'){ 
            printf("Simbolo:\\n\tFrequencia: %d \n", probabilidade);
            continue;
        }
        printf("Simbolo: %c\tFrequencia: %d \n", c, probabilidade);
	}
}

void exibeInOrdem(tArvBin T) {
     if (T == NULL)
        return ;
     if (T->esq != NULL){
        exibeInOrdem(T->esq);
     }
     printf("%d ", T->representacao);
     if (T->dir != NULL)
        exibeInOrdem(T->dir);
}

void achaHuffinhos(tArvBin T, int matriz[][3], int caracteres, int *i){
     if (T == NULL)
        return ;
     if (T->esq != NULL){
        (T->esq)->altura = (T->altura) +1;
        (T->esq)->huffinho = 2*(T->huffinho);
        if((T->esq)->representacao!=-2) {
             matriz[*i][0]=(T->esq)->representacao;
             matriz[*i][1]=(T->esq)->huffinho;
             matriz[*i][2]=(T->esq)->altura;
             (*i)++;
        }
        achaHuffinhos(T->esq, matriz, caracteres, i);
     }
     if (T->dir != NULL){
        (T->dir)->altura = (T->altura) +1;
        (T->dir)->huffinho = 2*(T->huffinho) +1;
        if((T->dir)->representacao!=-2){
             matriz[*i][0]=(T->dir)->representacao;
             matriz[*i][1]=(T->dir)->huffinho;
             matriz[*i][2]=(T->dir)->altura;
             (*i)++;

        }
        achaHuffinhos(T->dir, matriz, caracteres, i);
     }
}

//bin�rio para ascII
char bintoASCII(char binario[8]){
    int i;
    unsigned char value = 0;
    for(i = 0; i < 8; i++)
    {
        if(binario[i] == '1')
            value += pow(2, 7-i);
    }
    return value;
}

