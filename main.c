#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

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

#define MAX_ARGS 90
#define MAX_CADENA 100

void* cargarDocumentos(Map* mapaGlobal, List* listaArchivos, int *numeroDocs);
int isEqualString(void* key1, void*key2);
int extrae_argumentos(char *orig, char args[][MAX_CADENA], int max_args);
int contLetras(char args[]);
void minus(char* linea);
void* documentoOrdenados(List* listaDocs);
void* buscarPorPalabra(Map* mapaGlobal);
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
            case 3:printf("NO HECHA.\n");break;
            case 4:mostrarPalabrasRelevantes(mapaGlobal,listaDocs,&numeroDocs);break;
            case 5:buscarPalabraDoc(mapaGlobal,listaDocs);break;
        }
    }

    return 0;
}

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

int contLetras(char args[]){
    const char* aux= (const char*)args;
    int cont = 0;
    for(int f = 0; f < strlen(aux); f++){
        cont++;
    }
    return cont;
}


void minus(char* linea){
    
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
    nuevoDoc->nombreDocumento = aux_nombreArchivo;

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
    P = firstList(listaDocs);
    while(P != NULL){
        printf("Doc: %s\n",P->nombreDocumento);
        printf("Cantidad palabras: %i\n",P->cantidadPalabras);
        printf("Cantidad caracteres: %i\n",P->cantidadCaracteres);
        P = nextList(listaDocs);
    }

    /*
        ---------------------POSIBLES METODOS PARA ORDENAR LISTAS-----------------------------------

        


    */
}

void * buscarPorPalabra(Map * mapaGlobal){
    char * palabraIngresada = (char*)malloc(50 * sizeof(char)); //  modificacion cantidad 
    printf("Ingrese la palabra a buscar: ");
    scanf("%s", palabraIngresada);
    const char * M = searchMap(mapaGlobal,palabraIngresada);

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
void* mostrarPalabrasRelevantes(Map* mapaGlobal, List* listaDocs, int* numeroDocs){

    char* nombreTxt = (char*) malloc(50 * sizeof(char));
    printf("Ingrese el nombre del documento deseado: ");
    scanf("%s", nombreTxt);

    //Buscar si nombreDoc existe en listaDocs.
    // verificacion si existe el documento en nuestra lista
    bool valido = false;
    //documento * docList = (documento*)malloc(sizeof(documento));
    documento * docList;
    docList = firstList(listaDocs);
    while(docList != NULL){
        printf("ALOA: %s\n",docList->nombreDocumento);
        if(strcmp(nombreTxt, docList->nombreDocumento) == 0){
            
            printf("Documento si esta\n");
            valido = true;
            break;
        }
        printf("ALO: %s\n",docList->nombreDocumento);
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

    while(M != NULL){
        //printf("1\n");
        // Recorrer mapaGlobal, solo nos importan las palabras que tengan el doc buscado asociado a su datosDoc.
        
        // Si encuentra una palabra que tiene el documento buscado:
        List * listAux = M->datosDoc;
        datosDoc * datosAux = firstList(listAux);
        while(datosAux != NULL){
            //printf("2\n");
            if(strcmp(datosAux->nombreDoc,blocDeNotas)==0){
                //printf("3\n");
                // - Se crea un struct "relevantes", nombrePalabraR = nombre de la palabra
                palRelevante * palabraRelevante = (palRelevante*)malloc(sizeof(palRelevante));

                char* aux = (char*)malloc(50*sizeof(char));

                strcpy(aux, M->nombrePalabra);
                
                palabraRelevante->nombre = aux;
                // - RELEVANCIA = calculo de la formula ( RELEVANCIA debe ser de tipo long long int para guardar decimales)
                double primeraParte = ((double)(datosAux->frecPalabraDoc)/(docList->cantidadPalabras));
                
                //printf("primera parte: %.2lf\n",primeraParte);

                //int kikiDuYouLoveME = M->cantidadDocs;
                //printf("cantidad docs = %d\n", kikiDuYouLoveME);

                //printf("Numero Docs: %i\n", numeroDocs);
                int segundaParte = (*numeroDocs/M->cantidadDocs);

                //printf("segunda parte: %i\n",segundaParte);

                palabraRelevante->relevancia = (primeraParte * (double)segundaParte);
                //printf("Nombre palabra: %s\n",palabraRelevante->nombre);
                //printf("Relevancia: %.2lf\n",palabraRelevante->relevancia);
                pushBack(listaPR,palabraRelevante);

            }
            datosAux = nextList(listAux);
            if(datosAux == NULL)break;
        }

        M = nextMap(mapaGlobal);
        if(M == NULL)break;
    }
    
    // Cuando ya se tengan todas las palabras posibles guardadas:
    palRelevante * palabraR = (palRelevante*)malloc(sizeof(palRelevante));
    palabraR = firstList(listaPR);
    // - Mostrar maximo 10 datos RELEVANCIA de la misma forma que mostramos los datos en buscarPorPalabra
    

}

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

        while(fgets(lineaArchivo, 1024, txt) != NULL){
            minus(lineaArchivo);
            int nargs = extrae_argumentos(lineaArchivo,args,MAX_ARGS);
            int i;
            if(nargs > MAX_ARGS){
                printf ("Se han devuelto más palabras del máximo\n");
                nargs = MAX_ARGS;
            }
            
            for(i = 0; i < nargs; i++){
                char* aux = (char*) malloc(50*sizeof(char));
                strcpy(aux, args[i]);
                if(strcmp(aux,nombrePalabra) == 0){
                    printf("Cadena: %s\n", lineaArchivo);
                }
            }       
        }

    }else{
        printf("Doc no encontrado.\n");
    }
    

}