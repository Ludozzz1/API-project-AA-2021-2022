#include <stdio.h>
#include <string.h>  
#include <stdbool.h>     
#include <stdlib.h>   
#include <assert.h>
#include <time.h>

unsigned long TABLE_SIZE;
int lung;
int lung_entrata;

typedef struct node{
    char *parola;
    struct node *next;
    struct node *prior;
}parola; 

parola** hash_table;

typedef struct nodo{
    int *contatore_filtro;
    int **comparsa;
    int *max_count;
    char *filtro;
}filtraggio;

typedef struct node_t{
    parola* globale;
    parola* locale;
}dizionarii;

unsigned long
int funz( char *str){ 
    unsigned long hash = 5381;
    unsigned int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash;
}

parola* hash_table_insert(char* parol_inserimento,parola *testa){ 
    parola* p = malloc(sizeof(parola));
    p->parola = parol_inserimento;
    p->next = testa;
    p->prior = NULL;
    if(testa != NULL){
        testa->prior = p;
    }
    unsigned long index = funz(p->parola);
    for (int i = 0;i<TABLE_SIZE;i++){
        unsigned long try = ((i*index+index)^ 0xdeadbeefc0debabeULL) % TABLE_SIZE;
        if(hash_table[try] == NULL){
            hash_table[try] = p;
            return p;
        }
    }
    assert(false);
}

bool hash_table_lookup(char *parola){ 
    unsigned long index = funz(parola);
    for(int i = 0;i<TABLE_SIZE;i++){
        unsigned long try = ((i*index+index)^ 0xdeadbeefc0debabeULL) % TABLE_SIZE;
        if(hash_table[try] == NULL){
            return false;
        }
        if(hash_table[try] != NULL && 
        strncmp(hash_table[try]->parola,parola,lung)==0){
            return true;
        }
    }
    return false;
}

void array_contatore(int* ascii,char * stringa){ 
    int number_ascii;
    for(int i = 0;i < lung;i++){
        number_ascii = stringa[i];
        ascii[number_ascii] +=1;
    }
}

void confronto_stringhe(char *stringa_1,char *stringa_2,char *stringa_3,int *ascii,int* ascii_due){ 
    int number_ascii;
    int count_due[128] = {0};
    int count[128] = {0};
    for(int k = 0;k<lung;k++){
        if (stringa_1[k] == stringa_2[k]){
            number_ascii = stringa_2[k];
            count[number_ascii]++;
        }
    }
    for (int i = 0; i<lung;i++){
        if (stringa_1[i] == stringa_2[i]){
            stringa_3[i] = '+';
            number_ascii = stringa_2[i];
            if(ascii[number_ascii] != 0){
                ascii[number_ascii]--;
            }
        }
        else if(stringa_1[i] != stringa_2[i] ){
            number_ascii = stringa_2[i];
            if(ascii[number_ascii] == 0){
                stringa_3[i] = '/';
            }
            else{
                ascii[number_ascii]--;
                if(ascii_due[number_ascii]-count[number_ascii]<=count_due[number_ascii]){
                    stringa_3[i] = '/';
                    count_due[number_ascii]++;
                }
                else{
                    stringa_3[i] = '|';
                    count_due[number_ascii]++;
                }
            }
        }
    }
}

void non_compare(char*stringa_1,char*stringa_2,int **controllo){ 
    int number_ascii;
    for(int i = 0;i < lung;i++){
        number_ascii = stringa_1[i];
        if(stringa_2[i] == '/' || stringa_2[i] == '|'){
            controllo[i][number_ascii] = -1;
        }
        else{
            controllo[i][number_ascii] = 1;
        }
    }
}


void produci_info(char *stringa_finale,char* stringa_confronto,int * ascii_confronto,int* max_count){ 
    int num_ascii;
    for(int i = 0;i<lung;i++){
        if(stringa_finale[i] == '+' || stringa_finale[i] == '|'){
            num_ascii = stringa_confronto[i];
            ascii_confronto[num_ascii]++;
        }
        else{
            num_ascii = stringa_confronto[i];
            max_count[num_ascii] = 1;
        }
    }
}

void crea_filtro(char*filtro,char*stringa_finale,char*stringa_confrontata){ 
    for(int i = 0;i<lung;i++){
        if(stringa_finale[i] == '+'){
            filtro[i] = stringa_confrontata[i];
        }
        else{
            filtro[i] = '.';
        }
    }
}

void confronta_filtro(char*filtro,char*stringa_finale,char*stringa_confrontata){ 
    for(int i = 0;i<lung;i++){
        if(stringa_finale[i] == '+'){
            filtro[i] = stringa_confrontata[i];
        }
    }   
}

void confronta_contatore(int*contatore_filtro,int*max_count,int*contatore_filtro_conf,int*max_count_conf,char* stringa_confronto){ 
    int num_ascii;
    for(int i = 0;i<lung;i++){
        num_ascii = stringa_confronto[i];
        if(max_count[num_ascii] == 0 && max_count_conf[num_ascii] == 1){
            max_count[num_ascii] = 1;
            contatore_filtro[num_ascii] = contatore_filtro_conf[num_ascii];
        }
        else if(max_count[num_ascii] == 0 && max_count_conf[num_ascii] == 0){
            if(contatore_filtro[num_ascii] < contatore_filtro_conf[num_ascii]){
                contatore_filtro[num_ascii] = contatore_filtro_conf[num_ascii];
            }
        }
    }
}

int filtra_parole(filtraggio*dati_filtrare,char *stringa_filtrare,char *risultato){ 
    int num_ascii;
    int num_ascii_2;
    int contatore_presenze[128] = {0};
    for(int i = 0;i<lung;i++){
        num_ascii = stringa_filtrare[i];
        contatore_presenze[num_ascii]++;
        if(dati_filtrare->filtro[i] != '.' && stringa_filtrare[i] != dati_filtrare->filtro[i]){
            return 0;
        } 
        if(dati_filtrare->comparsa[i][num_ascii] == -1){
                return 0;
            }
    }
    for(int j = 0;j<lung;j++){
        num_ascii = risultato[j];
        num_ascii_2 = stringa_filtrare[j];
        if(dati_filtrare->max_count[num_ascii] == 1 && contatore_presenze[num_ascii] != dati_filtrare->contatore_filtro[num_ascii]){
            return 0;
        }
        else if(dati_filtrare->max_count[num_ascii] == 0 && contatore_presenze[num_ascii] < dati_filtrare->contatore_filtro[num_ascii]){
            return 0;
        }
        else if(dati_filtrare->max_count[num_ascii_2] == 1 && contatore_presenze[num_ascii_2] != dati_filtrare->contatore_filtro[num_ascii_2]){
            return 0;
        }
    }
    return 1;
}

parola *inserisci_testa(char*stringa, parola* head){ 
    parola* nuova_testa = malloc(sizeof(parola));
    nuova_testa->parola = stringa;
    nuova_testa->next = head;
    nuova_testa->prior = NULL;
    if(head != NULL){
        head->prior = nuova_testa;
    }
    return nuova_testa;
}

parola* dividi(parola* lista){  
    parola* veloce = lista;  
    parola* lento = lista;
    while(veloce->next && veloce->next->next){
        veloce = veloce->next->next;
        lento = lento->next;
    }
    parola* temporaneo = lento->next;
    lento->next->prior = NULL;
    lento->next = NULL;
    return temporaneo; 
}

parola* merge(parola* primo, parola* secondo){ 
    if(!primo){
        return secondo;
    }
    if(!secondo){
        return primo;
    }
    parola* tmp = NULL;
    if(strcmp(primo->parola,secondo->parola) > 0){
        tmp = secondo;
        secondo = primo;
        primo = tmp;
    }
    parola* start1 = primo;
    parola* start2 = secondo;
    while(start1->next != NULL && start2 != NULL){
        if(strcmp(start1->next->parola,start2->parola) > 0){
            tmp = start2->next;
            start2->next = start1->next;
            start1->next->prior = start2;
            start1->next = start2;
            start2->prior = start1;
            start2 = tmp;
            if(start2 != NULL){
                start2->prior = NULL;
            }
        }
        start1 = start1->next;
    }
    if(start1->next == NULL){
        start1->next = start2;
        start2->prior = start1;
    }
    return primo;
}

parola* merge_sort(parola *lista){ 
    if(!lista || !lista->next){
        return lista;
    }
    parola* secondo = dividi(lista);
    lista = merge_sort(lista);
    secondo = merge_sort(secondo);
    return merge(lista,secondo);
}



void stampa_lista(parola* list){ 
    while(list != NULL){
        printf("%s\n",list->parola);
        list = list->next;
    }
}

dizionarii* inserisci_inizio(parola* testa, parola*locale, filtraggio*prova,int primo_inserimento,char*stringa_finale){ 
    char*input = (char*)malloc(lung_entrata+1);
    dizionarii* coppia = malloc(sizeof(dizionarii));
    while(1){
        assert(scanf("%s",input)==1);
        if(input[0] ==  '+'){
            break;
        }
        else{
            testa = hash_table_insert(input,testa);
            if(primo_inserimento == 1){
                if(filtra_parole(prova,input,stringa_finale) == 1){
                    locale = inserisci_testa(input,locale);
                }
            }
        }
        input = (char*)malloc(lung_entrata+1);
    }
    coppia->globale = testa;
    coppia->locale = locale;
    free(input);
    return coppia;
}

parola * crea_ammissibili(parola* dizionario,filtraggio*prova,char* stringa_finale){ 
    parola* testa_disponibili = NULL;
    int count = 0;
    while(dizionario != NULL){
        if(filtra_parole(prova,dizionario->parola,stringa_finale) == 1){
            testa_disponibili = inserisci_testa(dizionario->parola,testa_disponibili);
            count++;
        }
        dizionario = dizionario->next;
    }
    printf("%d\n",count);
    return testa_disponibili;
}

void delete_node(parola** testa,parola*da_cancellare){ 
    assert(da_cancellare != NULL);
    if(*testa == da_cancellare){
        *testa = da_cancellare->next;    
    }
    if(da_cancellare->next != NULL){
        da_cancellare->next->prior = da_cancellare->prior;
    }
    if(da_cancellare->prior != NULL){
        da_cancellare->prior->next = da_cancellare->next;
    }
    free(da_cancellare);
    return;
}

parola* filtra_ammissibili(parola*testa_disponibili_origine,filtraggio*prova,char*stringa_finale){ 
    parola*testa_disponibili = testa_disponibili_origine;
    parola* temp = malloc(sizeof(parola));
    int count = 0;
    if(testa_disponibili_origine == NULL){
        printf("%d\n",0);
        return NULL;
    }
    while(testa_disponibili != NULL){
        if(filtra_parole(prova,testa_disponibili->parola,stringa_finale) == 0){
            temp = testa_disponibili;
            testa_disponibili = testa_disponibili->next;
            delete_node(&testa_disponibili_origine,temp);
        }
        else{
            count++;
            testa_disponibili = testa_disponibili->next;
        }
    }
    printf("%d\n",count);
    return testa_disponibili_origine;
}

parola* nuova_partita(parola*testa){
    int tentativi;
    int vinto;
    int var;
    int count_tentativi;
    int primo_inserimento;
    int ascii[128] = {0};
    int appoggio[128] = {0};
    int ascii_confronto[128] = {0};
    int max_count[128] = {0};
    int ascii_conf_2[128] = {0};
    int max_count_2[128] = {0};
    char* riferimento = (char*)malloc(lung+1);
    char* input = (char*)malloc(lung_entrata+1);
    char* risultato = (char*)malloc(lung+1);
    char* filtro = (char*)malloc(lung+1);
    int** controllo = (int **)malloc(lung * sizeof(int*));
    for(int j = 0;j< lung;j++){
        controllo[j] = (int*)calloc(128,sizeof(int));
    }
    parola* tmp = NULL;
    parola* testa_disponibili = NULL;
    filtraggio* prova = malloc(sizeof(filtraggio));
    dizionarii* coppia;
    count_tentativi = 0;
    primo_inserimento = 0;
    vinto = 0;
    assert(scanf("%s",riferimento)== 1);
    assert(scanf("%d",&tentativi) == 1);
    array_contatore(ascii,riferimento);
    memcpy(appoggio,ascii,128*sizeof(128));
    while(count_tentativi < tentativi){
        assert(scanf("%s",input)== 1);
        var = strcmp(riferimento,input);
        if(input[0] == '+'){
            if(strcmp("+inserisci_inizio",input) == 0){
                coppia = inserisci_inizio(testa,testa_disponibili,prova,primo_inserimento,riferimento);
                testa = coppia->globale;
                testa_disponibili = coppia->locale;
                free(coppia);
            }
            else if(strcmp("+stampa_filtrate",input) == 0){
                if(testa_disponibili != NULL){
                    testa_disponibili = merge_sort(testa_disponibili);
                    stampa_lista(testa_disponibili);
                }
                else{
                    testa = merge_sort(testa);
                    stampa_lista(testa);
                }
            }
        }
        else if(var == 0){
            printf("ok\n");
            vinto = 1;
            break;
        }
        else if(hash_table_lookup(input) == false && input[0] != '+'){
            printf("not_exists\n");
        }
        else {
            count_tentativi++;
            confronto_stringhe(riferimento,input,risultato,ascii,appoggio);
            memcpy(ascii,appoggio,128*sizeof(128));
            risultato[lung] = '\0';
            printf("%s\n",risultato);
            if(primo_inserimento == 0){
                primo_inserimento = 1;
                crea_filtro(filtro,risultato,input);
                non_compare(input,risultato,controllo);
                produci_info(risultato,input,ascii_confronto,max_count);
                prova->filtro = filtro;
                prova->max_count = max_count;
                prova->comparsa = controllo;
                prova->contatore_filtro = ascii_confronto;
                testa_disponibili = crea_ammissibili(testa,prova,riferimento);
            }
            else{
                confronta_filtro(filtro,risultato,input);
                non_compare(input,risultato,controllo);
                produci_info(risultato,input,ascii_conf_2,max_count_2);
                confronta_contatore(ascii_confronto,max_count,ascii_conf_2,max_count_2,input);
                prova->filtro = filtro;
                prova->max_count = max_count;
                prova->comparsa = controllo;
                prova->contatore_filtro = ascii_confronto;
                testa_disponibili = filtra_ammissibili(testa_disponibili,prova,riferimento);            
            }
        }
        memset(ascii_conf_2,0,128*sizeof(int));
        memset(max_count_2,0,128*sizeof(int));
    }
    if (vinto != 1){
        printf("ko\n");
    }
    free(riferimento);
    free(risultato);
    free(input);
    free(filtro);
    for(int j = 0; j < lung;j++){
        free(controllo[j]);
    }
    free(controllo);
    while(testa_disponibili != NULL){
        tmp = testa_disponibili;
        testa_disponibili = testa_disponibili->next;
        free(tmp);
    }
    free(prova);
    return testa;
}

int main(){
    TABLE_SIZE = 4400021;
    hash_table = (parola**)malloc(TABLE_SIZE*sizeof(parola*));
    assert(scanf("%d ", &lung)==1);
    if (lung >= 18) {
        lung_entrata = lung;
    }
    else{
        lung_entrata = 17;
    }
    char *input = (char*)malloc(lung_entrata+1);
    parola* testa = NULL;
    parola* tmp = NULL;
    dizionarii* coppia;
    while (1) {
        assert(scanf("%s", input)==1);
        if (input[0] == '+') {
            break;
        } 
        else {
            testa = hash_table_insert(input,testa);
            input = (char*)malloc(lung_entrata+1);
        }
    }
    do {
        if(strcmp("+nuova_partita",input) == 0){
            testa = nuova_partita(testa);
        }
        if(strcmp("+inserisci_inizio",input) == 0){
            coppia = inserisci_inizio(testa,NULL,NULL,0,NULL);
            testa = coppia->globale;
            free(coppia);
        }
    } while (scanf("%s", input) == 1);
    while(testa != NULL){
        tmp = testa;
        testa = testa->next; 
        free(tmp->parola);
        free(tmp);
    }
    free(input);
    return 0;
}
