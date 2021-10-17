#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "list.h"

// estructuras

typedef struct{
    char* nombreDocumento;
    int cantidadPalabras;
    int cantidadCaracteres;
}documento;

typedef struct{
    int cantidadLetras;
    char* nombrePalabra;
    int frecuencia;
    List* nombreDelDocumento;
}palabra;

// define
#define MAX_ARGS 90
#define MAX_CADENA 100

// prototipos
void* cargarDocumentos(Map* mapaGlobal, List* listaArchivos);
int isEqualString(void* key1, void*key2);
int extrae_argumentos(char *orig, char args[][MAX_CADENA], int max_args);
int contLetras(char args[]);

// principal

int main(){
    // mapa Global donde guardamos los archivos
    Map* mapaGlobal = createMap(isEqualString);

    // creamos la lista
    List* listaDocs = createList();

    int opcion=1;
    printf("============== DOCUMENTOS =============\n");

    while(opcion!=0){
        printf("1. Cargar documento.\n");
        printf("2. Mostrar documentos ordenados.\n");
        printf("3. Buscar por palabra.\n");
        printf("4. Palabras con mayor frecuencia.\n");
        printf("5. Palabras mas relevantes.\n");
        printf("6. Buscar palabra en documento.\n");
        printf("0. Salir.\n");
        scanf("%d", &opcion);

        switch(opcion){
            case 1:cargarDocumentos(mapaGlobal, listaDocs);break;
            case 2:printf("NO HECHA.\n");break;
            case 3:printf("NO HECHA.\n");break;
            case 4:printf("NO HECHA.\n");break;
            case 5:printf("NO HECHA.\n");break;
            case 6:printf("NO HECHA.\n");break;   
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

    documento * nuevoDoc = (documento*)malloc(sizeof(documento));
    nuevoDoc->nombreDocumento = aux_nombreArchivo;
    printf("Nombre archivo : %s\n",nuevoDoc->nombreDocumento);
    
    ////////////////////////////DATOS PALABRAS////////////////////////////////////////
    
    char args[MAX_ARGS][MAX_CADENA];
    char lineaArchivo[1024];

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

                List * L = createList();
                pushFront(L,aux_nombreArchivo);
                nuevaPalabra->nombreDelDocumento = L;
                
                int cantidadDeLetras = contLetras(args[i]);
                printf("%i\n", cantidadDeLetras);
                nuevaPalabra->cantidadLetras = cantidadDeLetras;

                insertMap(mapaGlobal,nuevaPalabra->nombrePalabra,nuevaPalabra);
                
            }
            else{ // EXISTE PALABRA EN MAPA
                //printf("Entra en el else\n");
                palabra * palabraExiste = (palabra*)searchMap(mapaGlobal,args[i]);
                palabraExiste->frecuencia++;
                //printf("creamos la lista y buscamos en el mapa\n");
                //List * L = (List*)searchMap(mapaGlobal,args[i]);
                //printf("posicionamos la lista en la primera posicion\n");
                char* documento = firstList(palabraExiste->nombreDelDocumento);
                //printf("estamos en la primera posicion de la lista\n");

                while(documento != NULL){
                    
                    printf("entra a while\n");
                    if(documento == aux_nombreArchivo){
                        printf("documento ya existe en lista de palabra existente\n");
                        break;
                    }
                    documento = nextList(palabraExiste->nombreDelDocumento);
                    if(palabraExiste->nombreDelDocumento==NULL)break;
                }
                
                if(documento != aux_nombreArchivo){
                    printf("sapea\n");
                    pushFront(palabraExiste->nombreDelDocumento,aux_nombreArchivo);
                }
                
            }
        }
    }
    ///////////////////////////DATOS DEL DOCUMENTOS////////////////////////////////////////
    char ch;
    int caracteres=0, palabras=0, lineas=0;

    while((ch = fgetc(file)) != EOF){
        caracteres++;
    
        if(ch == '\n' || ch == '\0'){
            lineas++;
        }
        
        if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0'){
            palabras++;
        }
        
    }
    fclose(file);


    if(caracteres > 0){
        palabras++;
        lineas++;
    }
    
    nuevoDoc -> cantidadPalabras = palabras;
    nuevoDoc -> cantidadCaracteres = caracteres;
    
    //archivo en la lista

    pushBack(listaDocs, nuevoDoc);

    char * palabra = (char*)malloc(10*sizeof(char));
    char * confirmacion = (char*)malloc(3*sizeof(char));
    confirmacion = "SI";
        
    printf("Si DESEA cargar otro archivo escribra 'SI': ");
    scanf("%s", palabra);
    fclose(file);
    if(strcmp(palabra, confirmacion) == 0){
        cargarDocumentos(mapaGlobal,listaDocs);
    }

    printf("Archivos cargados\n");
}