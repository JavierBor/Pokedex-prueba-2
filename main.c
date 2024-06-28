#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "TDAs/list.h"
#include "TDAs/hashmap.h"
#include "TDAs/extra.h"

// Estructura que representa un Pokémon
typedef struct {
    char nombre[100];
    char numero[5];
    List *tipos;
    char generacion[100];
    char legendario[100];
} Pokemon;

// Función que muestra el menú principal
void mostrarMenu() {
    printf("========================================\n");
    printf("     MENU PRINCIPAL\n");
    printf("========================================\n");
    printf("1. Cargar Pokémon\n");
    printf("2. Buscar por nombre\n");
    printf("3. Buscar por número\n");
    printf("4. Buscar por tipo\n");
    printf("5. Buscar por generación\n");
    printf("6. WHO IS THAT POKEMON?\n");
    printf("7. Salir\n\n");
  }


// Función para verificar si una cadena es un número
int es_numero(const char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit(cadena[i])) {
          return 0; // No es un número
        }
    }
    return 1; // Es un número
  }

// Función para cargar los datos de Pokémon desde un archivo CSV
void cargar_pokemon(HashMap *nombrePokemon, HashMap *tipoPokemon, HashMap *numeroPokemon, HashMap *generacionPokemon) {
    // Abrir el archivo CSV para lectura
    FILE *archivo = fopen("Pokemon.csv", "r");
    if (archivo == NULL) { 
        perror("Error al abrir el archivo");  // Mostrar mensaje de error si no se puede abrir el archivo
        return;  // Salir de la función si ocurre un error al abrir el archivo
    }

    List *listaPokeTipos;  // Lista temporal para almacenar tipos de Pokémon
    char **campos;  // Array para almacenar los campos de cada línea CSV

    // Leer la primera línea del archivo CSV (encabezados) y avanzar al siguiente
    campos = leer_linea_csv(archivo, ',');
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));  // Reservar memoria para un nuevo Pokémon
        if (pokemon == NULL) {
            perror("Error al asignar memoria");  // Mostrar mensaje de error si falla la asignación de memoria
            return;  // Salir de la función si falla la asignación de memoria
        }

        // Copiar campos del archivo CSV al struct Pokemon
        strcpy(pokemon->numero, campos[0]);
        strcpy(pokemon->nombre, campos[1]);

        // Crear una lista para almacenar los tipos del Pokémon y agregar el primer tipo
        pokemon->tipos = list_create();
        list_pushBack(pokemon->tipos, strdup(campos[2]));

        // Buscar el tipo en el HashMap de tipos de Pokémon
        Pair *buscarTipo = searchMap(tipoPokemon, campos[2]);
        if (buscarTipo == NULL || buscarTipo->value == NULL) {
            // Si el tipo no existe en el HashMap, crear una nueva lista de Pokémon y agregarlo
            listaPokeTipos = list_create();
            list_pushBack(listaPokeTipos, pokemon);
            insertMap(tipoPokemon, strdup(campos[2]), listaPokeTipos);
        } else {
            // Si el tipo ya existe, obtener la lista de Pokémon y agregarlo
            listaPokeTipos = (List *)buscarTipo->value;
            list_pushBack(listaPokeTipos, pokemon);
        }

        // Verificar si el campo siguiente no es un número (puede ser un segundo tipo)
        if (!es_numero(campos[3])) {
            // Agregar el segundo tipo a la lista de tipos del Pokémon
            list_pushBack(pokemon->tipos, strdup(campos[3]));

            // Buscar el segundo tipo en el HashMap de tipos de Pokémon
            buscarTipo = searchMap(tipoPokemon, campos[3]);
            if (buscarTipo == NULL || buscarTipo->value == NULL) {
                // Si el segundo tipo no existe en el HashMap, crear una nueva lista de Pokémon y agregarlo
                listaPokeTipos = list_create();
                list_pushBack(listaPokeTipos, pokemon);
                insertMap(tipoPokemon, strdup(campos[3]), listaPokeTipos);
            } else {
                // Si el segundo tipo ya existe, obtener la lista de Pokémon y agregarlo
                List *listaPokeTiposEncontrados = buscarTipo->value;
                list_pushBack(listaPokeTiposEncontrados, pokemon);
            }

            // Copiar la generación y el estado legendario del Pokémon
            strcpy(pokemon->generacion, campos[11]);
            strcpy(pokemon->legendario, campos[12]);

        } else {
            // Si no hay segundo tipo, copiar solo la generación y el estado legendario del Pokémon
            strcpy(pokemon->generacion, campos[10]);
            strcpy(pokemon->legendario, campos[11]);
        }

        // Insertar el Pokémon en el HashMap por nombre
        insertMap(nombrePokemon, pokemon->nombre, pokemon);

        // Insertar el Pokémon en el HashMap por generación
        Pair *buscarGeneracion = searchMap(generacionPokemon, pokemon->generacion);
        if (buscarGeneracion == NULL || buscarGeneracion->value == NULL) {
            // Si la generación no existe en el HashMap, crear una nueva lista de Pokémon y agregarlo
            List *listaPokeGeneracion = list_create();
            list_pushBack(listaPokeGeneracion, pokemon);
            insertMap(generacionPokemon, strdup(pokemon->generacion), listaPokeGeneracion);
        } else {
            // Si la generación ya existe, obtener la lista de Pokémon y agregarlo
            List *listaPokeGeneracion = (List *)buscarGeneracion->value;
            list_pushBack(listaPokeGeneracion, pokemon);
        }

        // Insertar el Pokémon en el HashMap por número
        Pair *buscarNumero = searchMap(numeroPokemon, pokemon->numero);
        if (buscarNumero == NULL || buscarNumero->value == NULL) {
            // Si el número no existe en el HashMap, crear una nueva lista de Pokémon y agregarlo
            List *listaPokeNumero = list_create();
            list_pushBack(listaPokeNumero, pokemon);
            insertMap(numeroPokemon, strdup(pokemon->numero), listaPokeNumero);
        } else {
            // Si el número ya existe, obtener la lista de Pokémon y agregarlo
            List *listaPokenumero = (List *)buscarNumero->value;
            list_pushBack(listaPokenumero, pokemon);
        }
    }

    // Cerrar el archivo CSV después de cargar todos los Pokémon
    fclose(archivo);

    // Mostrar todos los Pokémon cargados correctamente
    Pair *pair = firstMap(nombrePokemon);
    size_t totalPokemones = 0;
    while (pair != NULL) {
        Pokemon *pokemon = (Pokemon *)pair->value;
        printf("-------------------------------\n");
        printf("Nombre: %s\n", pokemon->nombre);
        printf("Numero: %s\n", pokemon->numero);
        char *tipo = (char *)list_first(pokemon->tipos);
        printf("Tipo/s:");
        while (tipo != NULL) {
            printf(" %s", tipo);
            tipo = (char *)list_next(pokemon->tipos);
        }
        printf("\n");
        printf("Generacion: %s\n", pokemon->generacion);
        printf("Legendario: %s\n", pokemon->legendario);

        pair = nextMap(nombrePokemon);
        totalPokemones += 1;
    }
    printf("-------------------------------\n");
    printf("POKÉMON CARGADOS CORRECTAMENTE!!! ツ \n");
    printf("TOTAL DE POKÉMON CARGADOS CORRECTAMENTE: %ld\n", totalPokemones);
}



// Función para buscar un Pokémon por su número en el HashMap proporcionado
void buscarPorNumero(HashMap *numeroPokemon){
    char numero[101];  // Variable para almacenar el número de Pokémon ingresado por el usuario
    printf("Ingrese el numero del Pokémon a buscar: ");
    scanf("%s", numero);  // Leer el número del Pokémon desde la entrada estándar
    printf("\n");

    Pair *pair = searchMap(numeroPokemon, numero);  // Buscar el número del Pokémon en el HashMap
    if (pair != NULL) {
        List *pokemones = (List *)pair->value;  // Obtener la lista de Pokémon correspondiente al número buscado

        printf("Pokémon ENCONTRADOS\n\n");

        Pokemon *pokemon = (Pokemon *)list_first(pokemones);  // Obtener el primer Pokémon de la lista
        while (pokemon != NULL) {
            printf("-------------------------------\n");
            printf("Nombre: %s\n", pokemon->nombre);  // Imprimir el nombre del Pokémon
            printf("Numero: %s\n", pokemon->numero);  // Imprimir el número del Pokémon
            char *tipo = (char *)list_first(pokemon->tipos);  // Obtener el primer tipo del Pokémon
            printf("Tipo/s:");
            while (tipo != NULL){
                printf(" %s", tipo);  // Imprimir cada tipo del Pokémon
                tipo = (char *)list_next(pokemon->tipos);  // Avanzar al siguiente tipo del Pokémon
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);  // Imprimir la generación del Pokémon
            printf("Legendario: %s\n", pokemon->legendario);  // Imprimir el estado legendario del Pokémon
            pokemon = (Pokemon *)list_next(pokemones);  // Avanzar al siguiente Pokémon en la lista
        }
    } else {
        // Si no se encontró ningún Pokémon con el número buscado, mostrar un mensaje
        printf("-------------------------------\n");
        printf("No se encontro el numero en la Pokedex\n");
    }

    printf("-------------------------------\n");
}



// Función para buscar Pokémon por generación en el HashMap proporcionado
void buscarPorGeneracion(HashMap *generacionPokemon) {
    char generacion[101];  // Variable para almacenar la generación ingresada por el usuario
    printf("Ingrese la generación a buscar: ");
    scanf("%s", generacion);  // Leer la generación desde la entrada estándar
    printf("\n");

    size_t totalGeneracion;  // Variable para contar el total de Pokémon encontrados en la generación
    totalGeneracion = 0;

    Pair *pair = searchMap(generacionPokemon, generacion);  // Buscar la generación en el HashMap
    if (pair != NULL) {
        List *pokemones = (List *)pair->value;  // Obtener la lista de Pokémon correspondiente a la generación buscada

        printf("POKÉMON ENCONTRADOS\n\n");

        Pokemon *pokemon = (Pokemon *)list_first(pokemones);  // Obtener el primer Pokémon de la lista
        while (pokemon != NULL) {
            printf("-------------------------------\n");
            totalGeneracion += 1;  // Incrementar el contador de Pokémon encontrados en la generación
            printf("Nombre: %s\n", pokemon->nombre);  // Imprimir el nombre del Pokémon
            printf("Numero: %s\n", pokemon->numero);  // Imprimir el número del Pokémon
            char *tipo = (char *)list_first(pokemon->tipos);  // Obtener el primer tipo del Pokémon
            printf("Tipo/s:");
            while (tipo != NULL){
                printf(" %s", tipo);  // Imprimir cada tipo del Pokémon
                tipo = (char *)list_next(pokemon->tipos);  // Avanzar al siguiente tipo del Pokémon
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);  // Imprimir la generación del Pokémon
            printf("Legendario: %s\n", pokemon->legendario);  // Imprimir el estado legendario del Pokémon
            pokemon = (Pokemon *)list_next(pokemones);  // Avanzar al siguiente Pokémon en la lista
        }
    } else {
        // Si no se encontró ningún Pokémon de la generación buscada, mostrar un mensaje
        printf("-------------------------------\n");
        printf("NO HAY POKÉMON DE LA GENERACIÓN %s\n", generacion);
    }

    printf("-------------------------------\n");
    if (totalGeneracion != 0) {
        // Si se encontraron Pokémon en la generación buscada, imprimir el total encontrado
        printf("\nTOTAL DE POKÉMON DE LA GENERACIÓN %s: %ld\n", generacion, totalGeneracion);
    }
}



// Función para buscar un Pokémon por nombre en el HashMap proporcionado
void buscarPorNombre(HashMap *nombrePokemon) {
    char nombre[101];  // Variable para almacenar el nombre del Pokémon ingresado por el usuario
    printf("Ingrese el nombre del Pokémon a buscar: ");
    scanf("%s", nombre);  // Leer el nombre del Pokémon desde la entrada estándar
    printf("\n");

    Pair *pair = searchMap(nombrePokemon, nombre);  // Buscar el nombre del Pokémon en el HashMap
    if (pair != NULL) {
        Pokemon *pokemon = (Pokemon *)pair->value;  // Obtener el Pokémon encontrado

        printf("POKÉMON ENCONTRADO\n\n");
        printf("-------------------------------\n");
        printf("Nombre: %s\n", pokemon->nombre);  // Imprimir el nombre del Pokémon
        printf("Numero: %s\n", pokemon->numero);  // Imprimir el número del Pokémon
        char *tipo = (char *)list_first(pokemon->tipos);  // Obtener el primer tipo del Pokémon
        printf("Tipo/s:");
        while (tipo != NULL) {
            printf(" %s", tipo);  // Imprimir cada tipo del Pokémon
            tipo = (char *)list_next(pokemon->tipos);  // Avanzar al siguiente tipo del Pokémon
        }
        printf("\n");
        printf("Generacion: %s\n", pokemon->generacion);  // Imprimir la generación del Pokémon
        printf("Legendario: %s\n", pokemon->legendario);  // Imprimir el estado legendario del Pokémon
    }
    else {
        // Si no se encontró el Pokémon con el nombre ingresado, mostrar un mensaje
        printf("-------------------------------\n");
        printf("POKÉMON NO ENCONTRADO\n");
    }
    printf("-------------------------------\n\n");  // Imprimir línea separadora al final de la búsqueda
}



// Función para buscar Pokémon por tipo en el HashMap proporcionado
void buscarPorTipo(HashMap *tipoPokemon) {
    char tipo[101];  // Variable para almacenar el tipo de Pokémon ingresado por el usuario
    printf("Ingrese el tipo a buscar: ");
    scanf("%s", tipo);  // Leer el tipo de Pokémon desde la entrada estándar
    printf("\n");

    Pair *pair = searchMap(tipoPokemon, tipo);  // Buscar el tipo de Pokémon en el HashMap
    if (pair != NULL) {
        List *pokemones = (List *)pair->value;  // Obtener la lista de Pokémon del tipo especificado
        Pokemon *pokemon = (Pokemon *)list_first(pokemones);  // Obtener el primer Pokémon de la lista

        printf("POKÉMON TIPO %s ENCONTRADOS\n\n", tipo);
        while (pokemon != NULL) {
            printf("-------------------------------\n");
            printf("Nombre: %s\n", pokemon->nombre);  // Imprimir el nombre del Pokémon
            printf("Numero: %s\n", pokemon->numero);  // Imprimir el número del Pokémon
            char *tipos = list_first(pokemon->tipos);  // Obtener el primer tipo del Pokémon
            printf("Tipo/s:");
            while (tipos != NULL) {
                printf(" %s", tipos);  // Imprimir cada tipo del Pokémon
                tipos = list_next(pokemon->tipos);  // Avanzar al siguiente tipo del Pokémon
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);  // Imprimir la generación del Pokémon
            printf("Legendario: %s\n", pokemon->legendario);  // Imprimir el estado legendario del Pokémon
            pokemon = (Pokemon *)list_next(pokemones);  // Avanzar al siguiente Pokémon en la lista
        }
    }
    else {
        // Si no se encontró ningún Pokémon con el tipo ingresado, mostrar un mensaje
        printf("-------------------------------\n");
        printf("NO HAY POKÉMON DEL TIPO %s\n", tipo);
    }
    // No se incluye un printf adicional al final, ya que la función original no lo tiene
}



void adivinarPokemon(HashMap *numeroPokemon){
    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios
    int numAleatorio = rand() % 898; // Genera un número aleatorio entre 0 y 898
    char numChar[5]; // Declara un arreglo de caracteres con tamaño suficiente para almacenar la cadena

    sprintf(numChar, "%d", numAleatorio); // Convierte el número aleatorio a una cadena
    getchar();



    Pair *pair = searchMap(numeroPokemon, numChar);
    List *listaPokemones = (List *)pair->value;
    Pokemon *adivinar = (Pokemon *)list_first(listaPokemones);
    char intento[101];
    int contIntentos = 0;

    //Solo para probar si funciona correctamente adivinar
    //printf("%s\n", adivinar->nombre);

    
    size_t intentos;
    intentos = 0;

    printf("Ingrese su primer Intento: ");
    scanf("%100[^\n]", intento);
    getchar();
    if (strcmp(adivinar->nombre, intento) != 0)
    {
        printf("No es el Pokémon, intenta otra vez\n\n");
        printf("Tu Pokémon es legendario? %s\n" , adivinar->legendario);
        printf("Ingrese su Segundo Intento: ");
        scanf("%100[^\n]", intento);
        getchar();
        if (strcmp(adivinar->nombre, intento) != 0)
        {
            printf("No es el Pokémon, intenta otra vez\n\n");
            printf("Los tipos de tu pokemon son: ");
            char *tipos = list_first(adivinar->tipos);
            while (tipos != NULL){
                printf(" %s", tipos);
                tipos = list_next(adivinar->tipos);
            }
            printf("\n");
            printf("Ingrese su Tercer Intento: ");
            scanf("%100[^\n]", intento);
            getchar();
            if (strcmp(adivinar->nombre, intento) != 0)
            {
                printf("No es el Pokémon, intenta otra vez\n\n");
                printf("La generacion del Pokémon es:  %s\n" , adivinar->generacion);
                printf("Ingrese su Cuarto Intento: ");
                scanf("%100[^\n]", intento);
                getchar();
                if (strcmp(adivinar->nombre, intento) != 0)
                {
                    printf("No es el Pokémon, intenta otra vez\n\n");
                    printf("Última pista, su numero en la Pokedex es %s\n" , adivinar->numero);
                    printf("Ingrese su último intento: ");
                    scanf("%100[^\n]", intento);
                    getchar();
                    if (strcmp(adivinar->nombre, intento) != 0)
                    {
                        printf("Pokémon incorrecto, has perdido :(\n");
                        printf("El Pokémon era: %s\n", adivinar->nombre);
                        printf("¡¡¡Conseguiste 0 puntos!!! ¡¡¡Vuelve a intentarlo!\n");
                        return;

                    }
                    else
                    {
                        printf("\n¡Adivinaste el Pokémon!\n");
                        printf("¡¡¡Conseguiste 20 puntos!!! ¡¡¡Felicidades!!!\n");
                        printf("Gracias por jugar! <3\n");
                        return;
                    }

                }
                else{
                    printf("\n¡Adivinaste el Pokémon!\n");
                    printf("¡¡¡Conseguiste 40 puntos!!! ¡¡¡Felicidades!!!\n");
                    printf("Gracias por jugar! <3\n");
                    return;
                }
            }
            else{
                printf("\n¡Adivinaste el Pokémon!\n");
                printf("¡¡¡Conseguiste 60 puntos!!! ¡¡¡Felicidades!!!\n");
                printf("Gracias por jugar! <3\n");
                return;
            }

        }
        else{
            printf("\n¡Adivinaste el Pokémon!\n");
            printf("Gracias por jugar! <3\n");
            printf("¡¡¡Conseguiste 80 puntos!!! ¡¡¡Felicidades!!!\n");
            return;
        }
    }
    else{
        printf("\n¡Adivinaste el Pokémon!\n");
        printf("Conseguiste 100 puntos!!! Felicidades!!!\n");
        printf("Gracias por jugar! <3\n");
        return;
    }

}


int main(void) {
    char opcion;
    HashMap *nombrePokemon = createMap(2000);
    HashMap *tipoPokemon = createMap(2000);
    HashMap *numeroPokemon = createMap(2000);
    HashMap *generacionPokemon = createMap(2000);
    int cont = 0;

    do {
        mostrarMenu();
        printf("Ingrese su opción: ");
        scanf("%c", &opcion);
        switch(opcion) {
            case '1':
                if (cont == 0){
                    printf("Cargando pokémon...\n\n");
                    cargar_pokemon(nombrePokemon, tipoPokemon, numeroPokemon, generacionPokemon);
                }
                else{
                    printf("Los pokémon ya han sido cargados, muchas gracias!! =)\n");
                }
                cont++;
                break;
            case '2':
                printf("Buscando por nombre...\n\n");
                buscarPorNombre(nombrePokemon);
                break;
            case '3':
                printf("Buscando por número...\n\n");
                buscarPorNumero(numeroPokemon);
                break;
            case '4':
                printf("Buscando por tipo...\n\n");
                buscarPorTipo(tipoPokemon);
                break;
            case '5':
                printf("Buscando por generación...\n\n");
                buscarPorGeneracion(generacionPokemon);
                break;
            case '6':
                printf("Entrando a WHO IS THAT POKEMON?\n\n");
                adivinarPokemon(numeroPokemon);
                break;
            case '7':
                printf("Saliendo de la pokédex\n");
                break;
            default:
                printf("Opción inválida. Por favor intenta de nuevo con una opcion valida\n\n");
                break;
        }
        presioneTeclaParaContinuar(); 
        limpiarPantalla(); 
    } while (opcion != '7');
    return 0;
}
