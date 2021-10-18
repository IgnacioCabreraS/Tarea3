#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

typedef struct{
    char* nombreDocumento;
    int cantidadPalabras;
    int cantidadCaracteres;
}documento;

typedef struct{
    int cantidadLetras;
    char* nombrePalabra;
    int frecuencia;
    List* datosDoc;
}palabra;

typedef struct{
    char * nombreDoc;
    int frecPalabraDoc;
}datosDoc;

#define MAX_ARGS 90
#define MAX_CADENA 100

void* cargarDocumentos(Map* mapaGlobal, List* listaArchivos);
int isEqualString(void* key1, void*key2);
int extrae_argumentos(char *orig, char args[][MAX_CADENA], int max_args);
int contLetras(char args[]);
void* buscarPorPalabra(Map* mapaGlobal);

int main(){
    
    Map* mapaGlobal = createMap(isEqualString);
    List* listaDocs = createList();

    cargarDocumentos(mapaGlobal, listaDocs);

    int opcion=1;
    printf("============== DOCUMENTOS =============\n");

    while(opcion!=0){
        //printf(". Cargar documento.\n");
        printf("1. Mostrar documentos ordenados.\n");
        printf("2. Buscar por palabra.\n");
        printf("3. Palabras con mayor frecuencia.\n");
        printf("4. Palabras mas relevantes.\n");
        printf("5. Buscar palabra en documento.\n");
        printf("0. Salir.\n");
        scanf("%d", &opcion);

        switch(opcion){
            //case 1:cargarDocumentos(mapaGlobal, listaDocs);break;
            case 1:printf("NO HECHA.\n");break;
            case 2:buscarPorPalabra(mapaGlobal);break;
            case 3:printf("NO HECHA.\n");break;
            case 4:printf("NO HECHA.\n");break;
            case 5:printf("NO HECHA.\n");break;   
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
    char tokens[] = " ',.;:-_!?¿¡(){}[]|@#~%&/\\\n";

    /* Extraemos la primera palabra */
    tmp=strtok(str, tokens);
  
    do{
        if (num==max_args)
        return max_args+1;  /* Si hemos extraído más cadenas que palabras devolvemos */
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

void* cargarDocumentos(Map *mapaGlobal, List* listaDocs){
    
    char archivo[1024];
    char aux_nombreArchivo[1024];
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
            cargarDocumentos(mapaGlobal,listaDocs);
        }
        doc=nextList(listaDocs);
        if(doc==NULL)break;
    }

    documento * nuevoDoc = (documento*)malloc(sizeof(documento));
    nuevoDoc->nombreDocumento = aux_nombreArchivo;
    //printf("Nombre archivo : %s\n",nuevoDoc->nombreDocumento);

    ///////////////////////////DATOS DEL DOCUMENTOS////////////////////////////////////////
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


    ////////////////////////////DATOS PALABRAS////////////////////////////////////////
    
    char args[MAX_ARGS][MAX_CADENA];
    char lineaArchivo[1024];
    file = fopen(archivo, "r");

    while(fgets(lineaArchivo, 1024, file) != NULL){
        int nargs = extrae_argumentos(lineaArchivo,args,MAX_ARGS);
        int i;

        if(nargs > MAX_ARGS){
            printf ("Se han devuelto más palabras del máximo\n");
            nargs = MAX_ARGS;
        }
        printf("Cadena: %s\n", lineaArchivo);
        for(i = 0; i < nargs; i++){
            printf("Palabra %d: %s\n", i, args[i]);
            palabra* nuevaPalabra = (palabra*) malloc (sizeof(palabra));
            const char* M;
            M = searchMap(mapaGlobal,args[i]);
            nuevaPalabra->frecuencia = 0;
            if(M==NULL){ //NO EXISTE PALABRA EN MAPA
                nuevaPalabra->nombrePalabra = args[i];
                nuevaPalabra->frecuencia++;

                datosDoc * datosDocumento = (datosDoc*)malloc(sizeof(datosDoc));
                List * L = createList();
                
                datosDocumento->frecPalabraDoc=1;
                datosDocumento->nombreDoc = aux_nombreArchivo;
                pushFront(L,datosDocumento);
                nuevaPalabra->datosDoc = L;
                
                int cantidadDeLetras = contLetras(args[i]);  
                printf("%i\n", cantidadDeLetras);
                nuevaPalabra->cantidadLetras = cantidadDeLetras;

                insertMap(mapaGlobal,nuevaPalabra->nombrePalabra,nuevaPalabra);
                
            }
            else{ // EXISTE PALABRA EN MAPA
                
                palabra * palabraExiste = (palabra*)searchMap(mapaGlobal,args[i]);
                palabraExiste->frecuencia++;

                List * list = palabraExiste->datosDoc;

                datosDoc * datosDocumento = (datosDoc*)malloc(sizeof(datosDoc));
                datosDocumento = firstList(palabraExiste->datosDoc);
                bool existe = false;

                while(datosDocumento!=NULL){
                    if(strcmp(aux_nombreArchivo,datosDocumento->nombreDoc) == 0){
                        printf("Nombre del documento ya existe en datosDoc de palabra.\n");
                        datosDocumento->frecPalabraDoc++;
                        existe = true;
                        break;
                    }
                    datosDocumento=nextList(palabraExiste->datosDoc);
                    if(palabraExiste->datosDoc == NULL)break;
                }
                
                // Nombre de doc no está en datosDoc de palabra, o sea el archivo de donde sale la palabra
                // es nuevo.
                if(existe == false){
                    printf("Palabra proviene de archivo nuevo\n");
                    datosDoc * datos = (datosDoc*)malloc(sizeof(datosDoc));
                    datos->frecPalabraDoc=1;
                    datos->nombreDoc = aux_nombreArchivo;
                    pushFront(list,datos);
                }
            }
        }
    }

    char * palabra = (char*)malloc(3*sizeof(char));
    char * confirmacion = (char*)malloc(3*sizeof(char));
    confirmacion = "SI";
        
    printf("Si desea cargar otro archivo escriba 'SI': ");
    scanf("%s", palabra);
    fclose(file);
    if(strcmp(palabra, confirmacion) == 0){
        cargarDocumentos(mapaGlobal,listaDocs);
    }
    printf("Archivos cargados\n");
}

void * buscarPorPalabra(Map * mapaGlobal){
    char * palabraIngresada = (char*)malloc(40*sizeof(char));
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
            printf("%s\n",datosDocumento->nombreDoc);
            datosDocumento=nextList(palabraExiste->datosDoc);
        }
        int numeroDocs = cont;

        datosDocumento = firstList(palabraExiste->datosDoc);
        cont=0;
        while(numeroDocs != 0){
            cont++;
            if(cont = datosDocumento->frecPalabraDoc){
                printf("Documento: %s. Frecuencia palabra %s: %d.\n",datosDocumento->nombreDoc, palabraIngresada, datosDocumento->frecPalabraDoc);
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

        //printf("contador de archivo de palabra: %d\n", cont);
    }
}