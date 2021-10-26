# Tarea3
Tarea 3 Estructura de Datos GC

1.- Analisis de tarea y como lo haremos.

El problema nos pide crear un programa que lea distintos archivos txt y que almacene las palabras con sus respectivas caracteristicas, y asi
realizar distintas busquedas dentro de los mismos documentos o palabras.
Crearemos 3 structs:

- documento: guarda el nombre del documento, la cantidad de palabras y caracteres.
- palabra: guarda el nombre de la palabra, la frecuencia(cantidad de veces que aparece en todos los txt), la cantidad de letras,
la cantidad de docs (en cuantos docs aparece la palabra) y una lista de documentos, esta lista se llena con la siguiente struct.
- datosDoc: guarda el nombre del documento (nombreDoc) donde se encuentra la palabra y la frecuencia de la palabra en el doc (cuantas veces aparece
en el doc señalado).

Usaremos un mapa y un treeMap:
- mapaGlobal: tendra todos los datos de tipo palabra que se vayan cargando, se usara a lo largo de todo el codigo.
Key = nombre de la palabra. Value = struct palabra.
- treeMapDocs: contiene todos los datos de tipo documento, servirá para saber si un txt fue cargado previamente.
Key = nombre del txt. Value = struct documento.

Además, tenemos una variable llamada "numeroDocs" en el main para poder saber cuando docs se cargaron en total sin necesidad de buscar mucho.

2.- Cómo compilar y ejecutar la tarea.

R: Para compilar y ejecutar nuestra tarea debemos abrir la terminal de visual estudio y escribir los siguientes comandos:

gcc -g main.c list.c map.c treemap.c -o Tarea3
./Tarea3.exe

Usamos la ultima version de MinGW para poder hacer la tarea.

3.- Las opciones que funcionan correctamente y las que no lo hacen indicando posibles causas:

R: Todas la funciones compilan y muestran las salidas esperadas. Eso si, en el cargar, nuestro codigo no substrae los archivos desde una cadena, sino que los va pidiendo uno a uno.
Hay ocasiones donde se detiene el programa al cargar el documento, pero cuando utilizamos el gdb --args ./Tarea3 funciona de manera correcta. Ademas, si al obtener la cadena esta es 
unicamente de espacios el cargar termina su ejecucion en ese momento.
A veces cuando se realiza la carga de archivos, se para algun punto de este, no sabemos por que pasa realmente pero pensamos que puede ser algo sobre la reserva de memoria.
Muchas veces las comillas (") se registran como palabras, o algunas palabras vienen con una comilla adelante o atras.

4.- Aspectos positivos y a mejorar por cada uno de los integrantes (coevaluación);

Manuel Cabrera: Aporto decentemente al codigo con algunos errores entre otras cosas, pero falto centrarse el ejercicio en si y dedicarle mas tiempo

Ignacio Cabrera: Ayudó en todas las funciones del codigo, falta aprender mas sobre los treemaps.

Alexander Guillaume: ayudo en algunas funciones,a implementar datos y a comentar la tarea, pero le falta mas practica y 
mas aplicación en los mapas ya que le cuesta bien el uso y cuando se debe aplicar.

Marco Rojas: Participo bien dentro del codigo, en los aspectos a mejorar falta profundizar en conocimientos sobre los mapas.

5.- Puntos de premio/castigo a cada integrante dependiendo del trabajo realizado.

1) Marco Rojas: 0 
2) Alexander Guillaume: 0 
3) Manuel Cabrera : 0 
4) Ignacio Cabrera : 0