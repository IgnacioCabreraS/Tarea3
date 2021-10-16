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
    char* NombrePalabras;
    int frecuenciaPalabras;
    List*  nombreDelDocumento;
}palabra;

// prototipos
void* cargarDocumentos(Map* mapaGlobal, List* listaArchivos);
int isEqualString(void* key1, void*key2);
char*get_txt_field (char* lineaArchivo, int i);

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

    char ch;
    //DATOS DEL nuevoDoc
    int caracteres=0, palabras=0, lineas=0;
    palabra * nuevaPalabra = (palabra*)malloc(sizeof(palabra));

    while((ch = fgetc(file)) != EOF){
        caracteres++;

        if(ch == '\n' || ch == '\0'){
            lineas++;
        }
        
        if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0'){
            palabras++;

        }
        
    }

    if(caracteres > 0){
        palabras++;
        lineas++;
    }

    printf("\n");
    printf("Total characters: %d\n", caracteres);
    printf("Total words: %d\n", palabras);
    printf("Total lines: %d\n", lineas);
    
    nuevoDoc -> cantidadPalabras = palabras;
    nuevoDoc -> cantidadCaracteres = caracteres;
    //archivo en la lista
    pushBack(listaDocs, nuevoDoc);

    char * palabra = (char*)malloc(10*sizeof(char));
    char * confirmacion = (char*)malloc(3*sizeof(char));
    confirmacion = "SI";
        
    printf("Si desea cargar otro archivo escribra 'SI': ");
    scanf("%s", palabra);

    if(strcmp(palabra, confirmacion) == 0){
        cargarDocumentos(mapaGlobal,listaDocs);
    }
    printf("Archivos cargados\n");
}
