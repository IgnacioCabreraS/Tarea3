#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"
#include "treemap.h"

typedef struct{
    char * nombreDocumento;
    int cantidadPalabras;
    int cantidadCaracteres;
}documento;

typedef struct{
    int cantidadLetras;
    char* nombrePalabra;
    int frecuencia;
    List* datosDoc;
    int cantidadDocs;
}palabra;

typedef struct{
    char * nombreDoc;
    int frecPalabraDoc;
}datosDoc;

typedef struct{
    char * nombre;
    double relevancia;
}palRelevante;


#define MAX_ARGS 90 // cantidad palabras en la linea
#define MAX_CADENA 100 //  cantidad de cadenas en el archivo
#define MAXIMA_LONGITUD_CADENA 50

void* cargarDocumentos(Map* mapaGlobal, List* listaArchivos, int *numeroDocs);
int isEqualString(void* key1, void*key2);
int extrae_argumentos(char *orig, char args[][MAX_CADENA], int max_args);
int contLetras(char args[]);
void minus(char* linea);
void* documentoOrdenados(List* listaDocs);
void* buscarPorPalabra(Map* mapaGlobal);
int compara_flotantes(const void* p, const void* q);
void*mostarPalabrasFrecuencia(Map* mapaGlobal, List* listaDocs);
void* mostrarPalabrasRelevantes(Map* mapaGlobal, List* listaDocs, int* numeroDocs);
void* buscarPalabraDoc(Map* mapaGlobal, List* listaDocs);


int main(){
    
    Map* mapaGlobal = createMap(isEqualString);
    List* listaDocs = createList();
    int numeroDocs = 0;
    
    cargarDocumentos(mapaGlobal, listaDocs, &numeroDocs);

    int opcion=1;

    while(opcion!=0){
        printf("============== DOCUMENTOS =============\n");
        printf("1. Mostrar documentos ordenados.\n");
        printf("2. Buscar por palabra.\n");
        printf("3. Palabras con mayor frecuencia.\n");
        printf("4. Palabras mas relevantes.\n");
        printf("5. Buscar palabra en documento.\n");
        printf("0. Salir.\n");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:documentoOrdenados(listaDocs);break;
            case 2:buscarPorPalabra(mapaGlobal);break;
            case 3:mostarPalabrasFrecuencia(mapaGlobal, listaDocs);break;
            case 4:mostrarPalabrasRelevantes(mapaGlobal,listaDocs,&numeroDocs);break;
            case 5:buscarPalabraDoc(mapaGlobal,listaDocs);break;
        }
    }

    return 0;
}

//Funcion para comparar claves de tipo string retorna 1 si son iguales

int isEqualString(void * key1, void * key2){
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

int extrae_argumentos(char *orig, char args[][MAX_CADENA], int max_args){
    
    char *tmp;
    int num=0;
    /* Reservamos memoria para copiar la candena ... pero la memoria justa */
    char *str = malloc(strlen(orig)+1);
    strcpy(str, orig);
    char tokens[] = " ',.;:-_!?¿¡(){}[]|@#~%&/\n\\";

    /* Extraemos la primera palabra */
    tmp=strtok(str, tokens);
  
    do{
        if (num==max_args){
            return max_args+1;  /* Si hemos extraído más cadenas que palabras devolvemos */
        }
        /* El número de palabras máximo y salimos */
        strcpy(args[num], tmp);   /* Copiamos la palabra actual en el array */
        num++;
        /* Extraemos la siguiente palabra */
        tmp=strtok(NULL, tokens);
    }while (tmp!=NULL);

  return num;
}

// funcion que se le trasapasa la palabra y se calcula la cantidad de letras
int contLetras(char args[]){
    const char* aux= (const char*)args;
    int cont = 0;
    // recorremos la palabra caracter por caracter
    for(int f = 0; f < strlen(aux); f++){
        cont++;
    }
    return cont; // retornamos la cantidad de letras que hay dentro de la palabra
}

// funcion que se les trasapasa la linea y se pasan a minuscula.
void minus(char* linea){
    
    // utilizamos la funcion strlen(linea) para ir caracter por caracter usando la funcion
    // tolower 
    for(int i = 0; i < strlen(linea); i++){
        linea[i] = tolower(linea[i]);
    }
}


void* cargarDocumentos(Map *mapaGlobal, List* listaDocs, int *numeroDocs){
    
    char archivo[1024];
    char aux_nombreArchivo[100];
    FILE * file;
    
    do{
        printf("Ingresar nombre archivo: ");
        scanf("%s", &archivo);
        strcpy(aux_nombreArchivo, archivo);
        strcat(archivo, ".txt");
        file = fopen(archivo, "r");
    }while(!file);

    
    
    documento * doc = (documento*)malloc(sizeof(documento));
    doc = firstList(listaDocs);

    while(doc != NULL){
        
        if(strcmp(doc->nombreDocumento,aux_nombreArchivo) == 0){
            printf("Este txt ya esta cargado, escriba otro.\n");
            fclose(file);
            cargarDocumentos(mapaGlobal,listaDocs, numeroDocs);
        }
        doc=nextList(listaDocs);
        if(doc==NULL)break;
    }

    (*numeroDocs)+=1;
    /////////// DATOS DEL DOCUMENTOS ///////////

    documento * nuevoDoc = (documento*)malloc(sizeof(documento));
    char * auxaux = (char *)malloc(50*sizeof(char));
    strcpy(auxaux,aux_nombreArchivo);
    nuevoDoc->nombreDocumento = auxaux;

    char ch;
    int caracteres=0, palabras=0;

    while((ch = fgetc(file)) != EOF){
        caracteres++;
        if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0'){
            palabras++;
        }
    }
    fclose(file);

    if(caracteres > 0){
        palabras++;
    }
    nuevoDoc -> cantidadPalabras = palabras;
    nuevoDoc -> cantidadCaracteres = caracteres;
    pushBack(listaDocs, nuevoDoc);

    /////////// DATOS PALABRAS ///////////
    
    char args[MAX_ARGS][MAX_CADENA];
    char lineaArchivo[1024];
    file = fopen(archivo, "r");

    while(fgets(lineaArchivo, 1024, file) != NULL){
        minus(lineaArchivo);
        int nargs = extrae_argumentos(lineaArchivo,args,MAX_ARGS);
        int i;
        
        if(nargs > MAX_ARGS){
            printf ("Se han devuelto más palabras del máximo\n");
            nargs = MAX_ARGS;
        }
        printf("Cadena: %s\n", lineaArchivo);
        for(i = 0; i < nargs; i++){
            //printf("Palabra %d: %s\n", i, args[i]);
            palabra* nuevaPalabra = (palabra*) malloc (sizeof(palabra));
            //const char* M = (const char*)malloc(sizeof(const char));
            //palabra* M = (palabra*) malloc (sizeof(palabra));
            //printf("1.\n");
            char* auxmil = (char*) malloc(sizeof(char));
            strcpy(auxmil, args[i]);
            nuevaPalabra->nombrePalabra = auxmil;
            Map * M = searchMap(mapaGlobal,nuevaPalabra->nombrePalabra);
            printf("BUSCANDO EN MAPA LA PALABRA: %s\n",nuevaPalabra->nombrePalabra);
            nuevaPalabra->frecuencia = 0;
            if(M==NULL){ //NO EXISTE PALABRA EN MAPA
                printf("Nueva palabra encontrada:     ");
                
                nuevaPalabra->frecuencia++;

                datosDoc * datosDocumento = (datosDoc*)malloc(sizeof(datosDoc));
                List * L = createList();
                //printf("2.\n");
                datosDocumento->frecPalabraDoc=1;
                char* aux = (char*) malloc(sizeof(char));
                strcpy(aux,aux_nombreArchivo);
                datosDocumento->nombreDoc = aux;
                //datosDocumento->nombreDoc =aux_nombreArchivo;
                pushFront(L,datosDocumento);
                nuevaPalabra->datosDoc = L;

                nuevaPalabra->cantidadDocs = 1;
                
                //printf("3.\n");
                int cantidadDeLetras = contLetras(args[i]);  
                //printf("%i\n", cantidadDeLetras);
                nuevaPalabra->cantidadLetras = cantidadDeLetras;

                printf("Palabra: %s. Frecuencia: %d. Cantidad de letras: %d.\n", nuevaPalabra->nombrePalabra,nuevaPalabra->frecuencia,nuevaPalabra->cantidadLetras);
                printf("Palabra viene de documento nuevo ---- Nombre txt: %s. Frecuencia palabra en doc: %d.\n",datosDocumento->nombreDoc,datosDocumento->frecPalabraDoc);
                

                insertMap(mapaGlobal,nuevaPalabra->nombrePalabra,nuevaPalabra);
                
            }
            else{ // EXISTE PALABRA EN MAPA

                printf("Palabra ya existe en mapa:     ");
                
                palabra * palabraExiste = (palabra*)searchMap(mapaGlobal,nuevaPalabra->nombrePalabra);
                palabraExiste->frecuencia++;
                

                printf("Palabra: %s. Frecuencia: %d. Cantidad de letras: %d.\n", palabraExiste->nombrePalabra,palabraExiste->frecuencia,palabraExiste->cantidadLetras);

                List * list = createList();
                list = palabraExiste->datosDoc;

                datosDoc * datosDocumento = (datosDoc*)malloc(sizeof(datosDoc));
                datosDocumento = firstList(palabraExiste->datosDoc);
                bool existe = false;

                char* aux1 = (char*) malloc(sizeof(char));
                strcpy(aux1,aux_nombreArchivo);

                while(datosDocumento!=NULL){
                    if(strcmp(aux1,datosDocumento->nombreDoc) == 0){
                        //printf("Nombre del documento ya existe en datosDoc de palabra.\n");
                        //printf("Palabra: %s\n", palabraExiste->nombrePalabra);
                        datosDocumento->frecPalabraDoc++;
                        printf("Palabra viene de documento EXISTENTE ---- Nombre txt: %s. Frecuencia palabra en doc: %d.\n",datosDocumento->nombreDoc,datosDocumento->frecPalabraDoc);
                        existe = true;
                        break;
                    }    
                    datosDocumento=nextList(palabraExiste->datosDoc);
                    if(palabraExiste->datosDoc == NULL)break;
                }
                
                // Nombre de doc no está en datosDoc de palabra, o sea el archivo de donde sale la palabra
                // es nuevo.
                if(existe == false){
                    //printf("Palabra proviene de archivo nuevo\n");
                    //printf("Palabra: %s\n", palabraExiste->nombrePalabra);
                    palabraExiste->cantidadDocs += 1;
                    datosDoc * datos = (datosDoc*)malloc(sizeof(datosDoc));
                    datos->frecPalabraDoc=1;
                    char* aux = (char*) malloc(sizeof(char));
                    strcpy(aux,aux_nombreArchivo);
                    datos->nombreDoc = aux;
                    //datos->nombreDoc = aux_nombreArchivo;
                    printf("Palabra viene de documento NUEVO ---- Nombre txt: %s. Frecuencia palabra en doc: %d.\n",datos->nombreDoc,datos->frecPalabraDoc);
                    pushFront(list,datos);
                }

                //printf("DatosDoc ---- Nombre txt: %s. Frecuencia palabra en doc: %d.\n",datosDocumento->nombreDoc,datosDocumento->frecPalabraDoc);
            }
            
            printf("\n\n");
        }
    }

    char * palabra = (char*)malloc(3*sizeof(char));
    char * confirmacion = (char*)malloc(3*sizeof(char));
    confirmacion = "SI";
        
    printf("Si desea cargar otro archivo escriba 'SI': ");
    scanf("%s", palabra);
    fclose(file);
    if(strcmp(palabra, confirmacion) == 0){
        cargarDocumentos(mapaGlobal,listaDocs, numeroDocs);
    }
    printf("Archivos cargados\n");
}

void * documentoOrdenados(List* listaDocs){

    documento * P = (documento*)malloc(sizeof(documento));

    char temporal[MAXIMA_LONGITUD_CADENA];
    
    P = firstList(listaDocs);
    
    // guardar primera posicion y la posicion siguiente.
    
    //printf("Doc: %s\n",P->nombreDocumento); 
    
    while(P != NULL){

        // crear aux;
        printf("Doc: %s\n",P->nombreDocumento);
        
        documento* aux = (documento*) malloc(sizeof(documento));
        
        aux = firstList(listaDocs);
        aux = nextList(listaDocs);

        //printf("Doc: %s\n",aux->nombreDocumento);

        while(aux != NULL){
            
            if (strcmp(P->nombreDocumento, aux->nombreDocumento) <= 0)
            {
                // Movemos la cadena actual a la temporal
                memcpy(temporal,P->nombreDocumento,MAXIMA_LONGITUD_CADENA);
                //printf("Doc: %s\n",P->nombreDocumento); 
                // Movemos al actual el siguiente elemento
                memcpy(P->nombreDocumento, aux->nombreDocumento, MAXIMA_LONGITUD_CADENA);
                //printf("Doc: %s\n",P->nombreDocumento); 
                //printf("Doc: %s\n",aux->nombreDocumento);
                // Y en el siguiente elemento, lo que había antes en el actual pero ahora está en temporal
                memcpy(aux->nombreDocumento, temporal, MAXIMA_LONGITUD_CADENA);
                //printf("Doc: %s\n",P->nombreDocumento); 
                //printf("Doc: %s\n",aux->nombreDocumento);
            }

            
            aux = nextList(listaDocs);
            if(aux == NULL)break;
            
        } 

        P = nextList(listaDocs);
        if(P == NULL)break;
    }

    /*
        ---------------------POSIBLES METODOS PARA ORDENAR LISTAS-----------------------------------
        
        #define MAXIMA_LONGITU_CADENA 50
        
        necesitamos recorrer la lista
        
        char temporal[MAXIMA_LONGITUD_CADENA];
        aux = posicion siguiente de la lista
        if (strcmp(P->nombreDoc, aux->nombreDoc(siguiente)) <= 0)
        {
            // Movemos la cadena actual a la temporal
            memcpy(temporal,P->nombreDoc,aux->nombreDoc(siguiente));
            // Movemos al actual el siguiente elemento
            memcpy(P->nombreDoc, aux->nombreDoc(siguiente), MAXIMA_LONGITUD_CADENA);
            // Y en el siguiente elemento, lo que había antes en el actual pero ahora está en temporal
            memcpy(aux->nombreDoc(siguiente), temporal, MAXIMA_LONGITUD_CADENA);
        }

    */
}

/*funcion para buscar palabra, en caso de que la palabra ingresada sea erronea o no exista se muestra por pantalla */
void * buscarPorPalabra(Map * mapaGlobal){
    char * palabraIngresada = (char*)malloc(50 * sizeof(char)); //  modificacion cantidad 
    printf("Ingrese la palabra a buscar: ");
    scanf("%s", palabraIngresada); 
    const char * M = searchMap(mapaGlobal,palabraIngresada);
     //
    if(M == NULL){ 
        printf("Palabra buscada no existe.\n");
    }
    else{
        printf("Palabra encontrada.\n");
        palabra * palabraExiste = (palabra*)searchMap(mapaGlobal,palabraIngresada);
        datosDoc * datosDocumento = (datosDoc*)malloc(sizeof(datosDoc));
        datosDocumento = firstList(palabraExiste->datosDoc);

        int cont = 0;
        while(datosDocumento != NULL){
            cont++;
            //printf("%s\n",datosDocumento->nombreDoc);
            datosDocumento=nextList(palabraExiste->datosDoc);
        }

        int numeroDocs = cont;
        datosDocumento = firstList(palabraExiste->datosDoc);
        cont=0;
        while(numeroDocs != 0){
            cont++;
            if(cont = datosDocumento->frecPalabraDoc){
                printf("Documento: %s. Frecuencia palabra '%s': %d. Cantidad caracteres: %d.\n",datosDocumento->nombreDoc, palabraIngresada, datosDocumento->frecPalabraDoc,palabraExiste->cantidadLetras);
                numeroDocs--;
            }
            datosDocumento=nextList(palabraExiste->datosDoc);
            if(datosDocumento==NULL){
                datosDocumento = firstList(palabraExiste->datosDoc);
            }
            if(numeroDocs <= 0){
                break;
            }
        }
    }

}

/*
    Funcion nucleo de QSORT la cual indica como hacer la comparacion y como ordenar los datos
*/

int compara_flotantes(const void* p, const void* q){
    double x,y;
    x = *(double *) p;
    y = *(double *) q;
    //si x es mayor que y retorna -1    

    if(x > y) return -1;
    // si x es igual a y returna 0
    if(x == y) return 0;
    return 1;
}

/*
    Funcion que revisa en un documento, las frecuencias de sus palabras, para luego mostrar las 10 palabras (si es que la hay)
    con mayor frecuencia del documento
*/

void* mostarPalabrasFrecuencia(Map* mapaGlobal, List* listaDocs){

    char* nombreTxt = (char*) malloc(50 * sizeof(char));
    printf("Ingrese el nombre del documento deseado: ");
    scanf("%s", nombreTxt);

    //Buscar si nombreDoc existe en listaDocs.
    // verificacion si existe el documento en nuestra lista
    bool valido = false;
    //documento * docList = (documento*)malloc(sizeof(documento));
    documento * docList = (documento*)malloc(sizeof(documento));
    docList = firstList(listaDocs);
    while(docList != NULL){
        
        if(strcmp(nombreTxt, docList->nombreDocumento) == 0){
            
            printf("Documento si esta\n");
            valido = true;
            break;
        }
        //printf("ALO: %s\n",docList->nombreDocumento);
        docList = nextList(listaDocs);
        if(docList == NULL){
            printf("Documento no es\n");
            valido = false;
            break;
        }
    }
    int auxiliarNP = docList->cantidadPalabras;

    FILE * txt;
    char * blocDeNotas = (char*)malloc(50*sizeof(char));
    strcpy(blocDeNotas,nombreTxt);
    strcat(nombreTxt,".txt");
    txt = fopen(nombreTxt, "r");

    // Crear lista con struct "relevantes" (RELEVANCIA Y nombrePalabraR)
    
    TreeMap * mapFR = createTreeMap(isEqualString);

    palabra * M;
    M = firstMap(mapaGlobal);
    //printf("FUERA DEL PRIMER WHILE\n");

    //double mayor=0;
    //int contFR;
    while(M != NULL){

        // Recorrer mapaGlobal, solo nos importan las palabras que tengan el doc buscado asociado a su datosDoc.
        // Si encuentra una palabra que tiene el documento buscado:
        List * listAux = M->datosDoc;
        datosDoc * datosAux = firstList(listAux);

        while(datosAux != NULL){

            if(strcmp(datosAux->nombreDoc,blocDeNotas) == 0){
                
                //palFrecuente * palabraFR = (palFrecuente*)malloc(sizeof(palFrecuente));
                char * PAAAP = (char*)malloc(50*sizeof(char));
                strcpy(PAAAP, (M->nombrePalabra));
                //palabraFR->nombrePFR = PAAAP;
                
                double wea;
                wea = ((double)datosAux->frecPalabraDoc)/((double)auxiliarNP);
                double* frecuencia = &wea;
                printf("%.2lf\n", *frecuencia);
                /*
                if(palabraFR->frecuencia>mayor){
                    mayor = datosAux->frecPalabraDoc;
                }
                */
                //contFR+=1;

                insertTreeMap(mapFR,PAAAP,frecuencia);
                
                
            }
            datosAux = nextList(listAux);
            if(datosAux == NULL)break;
        }

        M = nextMap(mapaGlobal);
        if(M == NULL)break;
    }

    
    Pair * sexo = firstTreeMap(mapFR);
    
    int contDiez = 10;
    
    while(sexo != NULL){
        printf("Palabra: %s. Frecuencia: %lf.\n",sexo->key,&sexo->value);
        contDiez -= 1;

        sexo = nextTreeMap(mapFR);

        if(sexo == NULL)break;
        if(contDiez == 0)break;
    }
    

}

/*
    Funcion  que verifica si el documento ingreaso se encuentra en la lista docs. Dentro de este se crea una lista que guarda las palabras
    relevantes (se utiliza una formula para calcular la relevancia de las palabras), se ordenan por relevancia y se muestra por pantalla.
*/
void* mostrarPalabrasRelevantes(Map* mapaGlobal, List* listaDocs, int* numeroDocs){

    char* nombreTxt = (char*) malloc(50 * sizeof(char));
    printf("Ingrese el nombre del documento deseado: ");
    scanf("%s", nombreTxt);

    //Buscar si nombreDoc existe en listaDocs.
    // verificacion si existe el documento en nuestra lista

    bool valido = false;

    //documento * docList = (documento*)malloc(sizeof(documento));

    documento * docList;

    // recorremos la lista que contiene los docs ingresados para verificar si exites o, mejor dicho, si se ha ingresado.

    docList = firstList(listaDocs);
    while(docList != NULL){
        //printf("ALOA: %s\n",docList->nombreDocumento);
        if(strcmp(nombreTxt, docList->nombreDocumento) == 0){
            
            printf("Documento si esta\n");
            valido = true;
            break;
        }
        //printf("ALO: %s\n",docList->nombreDocumento);
        docList = nextList(listaDocs);
        if(docList == NULL){
            printf("Documento no es\n");
            valido = false;
            break;
        }
    }

    
    FILE * txt;
    char * blocDeNotas = (char*)malloc(50*sizeof(char));
    strcpy(blocDeNotas,nombreTxt);
    strcat(nombreTxt,".txt");
    txt = fopen(nombreTxt, "r");

    // Crear lista con struct "relevantes" (RELEVANCIA Y nombrePalabraR)
    
    List * listaPR = createList();
    palabra * M;
    M = firstMap(mapaGlobal);
    //printf("FUERA DEL PRIMER WHILE\n");
    int contPR=0;
    while(M != NULL){

        // Recorrer mapaGlobal, solo nos importan las palabras que tengan el doc buscado asociado a su datosDoc.
        
        // Si encuentra una palabra que tiene el documento buscado:
        List * listAux = M->datosDoc;
        datosDoc * datosAux = firstList(listAux);
        while(datosAux != NULL){

            if(strcmp(datosAux->nombreDoc,blocDeNotas)==0){

                // - Se crea un struct "relevantes", nombrePalabraR = nombre de la palabra
                palRelevante * palabraRelevante = (palRelevante*)malloc(sizeof(palRelevante));

                char* aux = (char*)malloc(50*sizeof(char));

                strcpy(aux, (M->nombrePalabra));
                
                palabraRelevante->nombre = aux;
                printf("Nombre palabra: %s\n",palabraRelevante->nombre);
                // - RELEVANCIA = calculo de la formula ( RELEVANCIA debe ser de tipo double para guardar decimales)
                double primeraParte = ((double)(datosAux->frecPalabraDoc)/(docList->cantidadPalabras));
                
                printf("datusAux->frecPalabraDoc: %i - ",datosAux->frecPalabraDoc);
                printf(" docList->cantidadPalabras: %i\n",docList->cantidadPalabras);
                printf("Primera parte: %.2lf\n",primeraParte);
                
                int kikiDuYouLoveME = M->cantidadDocs;
                //printf("cantidad docs = %d\n", kikiDuYouLoveME);

                //printf("Numero Docs: %i\n", *numeroDocs);
                double segundaParte = log(((double)*numeroDocs)/M->cantidadDocs);
                //printf("numeroDocs: %i\n",*numeroDocs);
                //printf("M->cantidadDocs: %i\n",M->cantidadDocs);
                printf("Segunda parte: %.2lf\n",segundaParte);

                //printf("segunda parte: %i\n",segundaParte);

                palabraRelevante->relevancia = (primeraParte * segundaParte);
                contPR +=1;
                printf("Relevancia: %.2lf\n",palabraRelevante->relevancia);
                pushBack(listaPR,palabraRelevante);
            }
            datosAux = nextList(listAux);
            if(datosAux == NULL)break;
        }

        M = nextMap(mapaGlobal);
        if(M == NULL)break;
    }

    printf("1\n");
    List* auxito = createList();
    auxito = listaPR;

    // Cuando ya se tengan todas las palabras posibles guardadas:

    palRelevante * palabraR = (palRelevante*)malloc(sizeof(palRelevante));
    // - Mostrar maximo 10 datos RELEVANCIA de la misma forma que mostramos los datos en buscarPorPalabra
    
    // Cuando ya se tengan todas las palabras posibles guardadas:
    palabraR = firstList(auxito);
    printf("2\n");
    // Ordenamos los elementos que hay dentro de la lista
    printf("Contador PR: %i\n",contPR);
    //qsort(palabraR->nombre,contPR,sizeof(palRelevante),compara_flotantes);
    printf("3\n");
    // Mostrar maximo 10 datos RELEVANCIA de la misma forma que mostramos los datos en buscarPorPalabra
    int cont = 0;
    while(palabraR != NULL){
        printf("%s \n", palabraR->nombre);
        cont+=1;
        if(cont == 10)break;
        palabraR = nextList(auxito);
        if(palabraR == NULL)break;
    }
    
}

/*
    Funcion que se que verifica si la palabra se encuentra en el documentos seleccionado, en el caso de encotrar el doc
    se busca la palabra y se prinftea: la palabra previa, la palabra buscada y la palabra siguiente
*/
void* buscarPalabraDoc(Map* mapaGlobal, List* listaDocs){

    char* nombreDoc = (char*)malloc(50*sizeof(char));
    printf("Ingrese el nombre del documento: ");
    scanf("%s", nombreDoc);
    
    // verificacion si existe el documento en nuestra lista
    bool valido = false;
    //documento * docList = (documento*)malloc(sizeof(documento));
    documento * docList;
    docList = firstList(listaDocs);
    while(docList != NULL){
        //printf("ALOA: %s\n",docList->nombreDocumento);
        if(strcmp(nombreDoc, docList->nombreDocumento) == 0){
            
            //printf("Documento si esta\n");
            valido = true;
            break;
        }
        //printf("ALO: %s\n",docList->nombreDocumento);
        docList = nextList(listaDocs);
        if(docList == NULL){
            //printf("Documento no es\n");
            valido = false;
            break;
        }
    }

    // el documetno fue encontrado

    if(valido == true){
        printf("El documento existe, continua la operacion\n");

        char* nombrePalabra = (char*)malloc(100 * sizeof(char));
        printf("Ingrese el nombre de la palabra: ");
        scanf("%s", nombrePalabra);

        FILE * txt;
        strcat(nombreDoc,".txt");
        txt = fopen(nombreDoc, "r");

        char args[MAX_ARGS][MAX_CADENA];
        char lineaArchivo[1024];

        // obtenemos la linea del archivo
        while(fgets(lineaArchivo, 1024, txt) != NULL){
            // pasamos a minusucla la linea completa con al funcion "minus"
            minus(lineaArchivo);
            int nargs = extrae_argumentos(lineaArchivo,args,MAX_ARGS); // funcion que obtiene las palabras de la linea
            int i;
            // verificamos si la cantidad de palabras es mayor a lo establecido en el define
            if(nargs > MAX_ARGS){
                printf ("Se han devuelto más palabras del máximo\n");
                nargs = MAX_ARGS;
            }
            // nargs representa la cantidad de lineas que hay dentro de la liena y se usa para ir pasando por cada palabra
            for(i = 0; i < nargs; i++){
                // varible que contiene la palabra en la posicion args[i]
                char* aux = (char*) malloc(50*sizeof(char));
                strcpy(aux, args[i]);
                // comprobamos si existe la palabra en la linea del archivo

                if(strcmp(aux,nombrePalabra) == 0){

                    printf("contexto palabra: ");
                    // args[i-1] corresponde a la palabra anterior a la buscada y el if es para comprobar si en esa posicion
                    // no existe nada entonces printfteara que no encontro nada
                    if(args[i-1] == NULL){ 
                        printf("no hay palabra previa a la actual\n");
                    }else printf("%s ", args[i-1]); // si hay palabra se imprime
                    // imprimimos la palabra actual
                    printf("%s ", args[i]);

                    // args[i+1] corresponde a la palabra posterior a la buscada y el if es para comprobar si en esa posicion
                    // no existe nada entonces printfteara que no encontro nada
                    if(args[i+1] == NULL){
                        printf("no hay palabra post a la actual\n");
                    }else printf("%s\n", args[i+1]); // si hay palabra se imprime
                }
            }       
        }

    }else printf("Doc no encontrado.\n");
}