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
char*get_txt_field (char *lineaArchivo, int i);

// principal

int main(){
    // mapa Global donde guardamos los archivos
    Map* mapaGlobal = createMap(isEqualString);

    // creamos la lista
    List* listaArchivos = createList();

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
            case 1:cargarDocumentos(mapaGlobal, listaArchivos);break;
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

void* cargarDocumentos(Map *mapaGlobal, List* ListaArchivos){

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

    char lineaArchivo[1024];
    int cont = 0;
    
    // sacamos las palabras de la cadena y contamos 
    // en el for lo recomendable es leer hasta el salto de linea
    //funcion determina palabras que hay en la cadena

    while(fgets(lineaArchivo, 1024, file) != NULL){
        for(int i = 0; i < 12; i++){
            char * aux = get_txt_field(lineaArchivo, i);
            printf("aux = %s\n",aux);
        }
        cont++;

        // contador es la cantidad de cadenas que hay en el archivo
        // crear funcion que determine la cantidad de cadenas
        if(cont == 1) break;
    }
    /*
    while(!feof(file)){
        fgets(aux,150,file);
        printf("%s \n",aux);
    }
    /*
    
    /*
    archivo en la lista
    pregunta se llama a la funcion, sino su return loco.

    char*  confirmacion = (char*)malloc(3*sizeof(char));
    confirmacion = "SI";
     pregunta se llama a la funcion, sino su return loco.
    */
}
// obtener palabras de txt

char*get_txt_field (char * tmp, int k){
    int open_mark = 0;
    char* ret=(char*) malloc(100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }
        if(open_mark || tmp[i]!= ' '){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }
        if(tmp[i]== ' '){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }
        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}