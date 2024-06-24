#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "TDAs/list.h"
#include "TDAs/hashmap.h"
#include "TDAs/extra.h"

typedef struct {
    char nombre[100];
    char numero[5];
    List *tipos;
    char generacion[100];
    char legendario[100];
} Pokemon;

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
          return 0;
        }
    }
    return 1;
  }


void cargar_pokemon(HashMap *nombrePokemon, HashMap *tipoPokemon, HashMap *numeroPokemon, HashMap *generacionPokemon) {
    FILE *archivo = fopen("Pokemon.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    List *listaPokeTipos;
    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));
        if (pokemon == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        strcpy(pokemon->numero, campos[0]);
        strcpy(pokemon->nombre, campos[1]);
        pokemon->tipos = list_create();
        list_pushBack(pokemon->tipos, strdup(campos[2]));
        Pair *buscarTipo = searchMap(tipoPokemon, campos[2]);
        if (buscarTipo == NULL || buscarTipo->value == NULL) {
            listaPokeTipos = list_create();
            list_pushBack(listaPokeTipos, pokemon);
            insertMap(tipoPokemon, strdup(campos[2]), listaPokeTipos);
        } 
        else {
            listaPokeTipos = (List *)buscarTipo->value;
            list_pushBack(listaPokeTipos, pokemon);
        }

        if (!es_numero(campos[3])){
            list_pushBack(pokemon->tipos, strdup(campos[3]));

            buscarTipo = searchMap(tipoPokemon, campos[3]);
            if (buscarTipo == NULL || buscarTipo->value == NULL) {
                listaPokeTipos = list_create();
                list_pushBack(listaPokeTipos, pokemon);
                insertMap(tipoPokemon, strdup(campos[3]), listaPokeTipos);
            } else {
                List *listaPokeTiposEncontrados = buscarTipo->value;
                list_pushBack(listaPokeTiposEncontrados, pokemon);
            }
            strcpy(pokemon->generacion, campos[11]);
            strcpy(pokemon->legendario, campos[12]);

        }
        else
        {
            strcpy(pokemon->generacion, campos[10]);
            strcpy(pokemon->legendario, campos[11]);

        }



        insertMap(nombrePokemon, pokemon->nombre, pokemon);
           // Insertar en el HashMap de generación
            Pair *buscarGeneracion = searchMap(generacionPokemon, pokemon->generacion);
            if (buscarGeneracion == NULL || buscarGeneracion->value == NULL) {
                List *listaPokeGeneracion = list_create();
                list_pushBack(listaPokeGeneracion, pokemon);
                insertMap(generacionPokemon, strdup(pokemon->generacion), listaPokeGeneracion);
            } else {
                List *listaPokeGeneracion = (List *)buscarGeneracion->value;
                list_pushBack(listaPokeGeneracion, pokemon);
            }



            Pair *buscarNumero = searchMap(numeroPokemon, pokemon->numero);
            if (buscarNumero == NULL || buscarNumero->value == NULL) {
                List *listaPokeNumero = list_create();
                list_pushBack(listaPokeNumero, pokemon);
                insertMap(numeroPokemon, strdup(pokemon->numero), listaPokeNumero);
            } else {
                List *listaPokenumero = (List *)buscarNumero->value;
                list_pushBack(listaPokenumero, pokemon);
            }


        }

        fclose(archivo);

    Pair *pair = firstMap(nombrePokemon);
    size_t totalPokemones = 0;
    while (pair != NULL){
        Pokemon *pokemon = (Pokemon *)pair->value;
        printf("-------------------------------\n");
        printf("Nombre: %s\n", pokemon->nombre);
        printf("Numero: %s\n", pokemon->numero);
        char *tipo = (char *)list_first(pokemon->tipos);
        printf("Tipo/s:");
        while (tipo != NULL){
            printf(" %s", tipo);
            tipo = (char *)list_next(pokemon->tipos);
        }
        printf("\n");
        printf("Generacion: %s\n", pokemon->generacion);
        printf("Legendario: %s\n", pokemon->legendario);

        pair = nextMap(nombrePokemon);
        totalPokemones+=1;

    }
    printf("-------------------------------\n");
    printf("POKÉMON CARGADOS CORRECTAMENTE!!! ツ \n");
    printf("TOTAL DE POKÉMON CARGADOS CORRECTAMENTE: %ld\n", totalPokemones);


}


void buscarPorNumero(HashMap *numeroPokemon){
    char numero[5];
    printf("Ingrese el numero del Pokémon a buscar: ");
    scanf("%s", numero);
    printf("\n");
    Pair *pair = searchMap(numeroPokemon, numero);
    if (pair != NULL) {
        List *pokemones = (List *)pair->value; // Lista de Pokémon de la generación especificada

        printf("Pokémon ENCONTRADOS\n\n");
        printf("-------------------------------\n");

        Pokemon *pokemon = (Pokemon *)list_first(pokemones);
        while (pokemon != NULL) {

            printf("Nombre: %s\n", pokemon->nombre);
            printf("Numero: %s\n", pokemon->numero);
            char *tipo = (char *)list_first(pokemon->tipos);
            printf("Tipo/s:");
            while (tipo != NULL){
                printf(" %s", tipo);
                tipo = (char *)list_next(pokemon->tipos);
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);
            printf("Legendario: %s\n\n", pokemon->legendario);
            pokemon = (Pokemon *)list_next(pokemones);
        }
    } else {
        printf("-------------------------------\n");
        printf("No se encontro el numero en la Pokedex\n");
    }


    printf("-------------------------------\n");

}


void buscarPorGeneracion(HashMap *generacionPokemon) {
    char generacion[100];
    printf("Ingrese la generación a buscar: ");
    scanf("%s", generacion);
    printf("\n");
    size_t totalGeneracion;
    totalGeneracion = 0;

    Pair *pair = searchMap(generacionPokemon, generacion);
    if (pair != NULL) {
        List *pokemones = (List *)pair->value; // Lista de Pokémon de la generación especificada

        printf("POKÉMON ENCONTRADOS\n\n");
        printf("-------------------------------\n");

        Pokemon *pokemon = (Pokemon *)list_first(pokemones);
        while (pokemon != NULL) {
            totalGeneracion+=1;
            printf("Nombre: %s\n", pokemon->nombre);
            printf("Numero: %s\n", pokemon->numero);
            char *tipo = (char *)list_first(pokemon->tipos);
            printf("Tipo/s:");
            while (tipo != NULL){
                printf(" %s", tipo);
                tipo = (char *)list_next(pokemon->tipos);
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);
            printf("Legendario: %s\n\n", pokemon->legendario);
            pokemon = (Pokemon *)list_next(pokemones);
        }
    } else {
        printf("-------------------------------\n");
        printf("NO HAY POKÉMON DE LA GENERACIÓN %s\n", generacion);
    }


    printf("-------------------------------\n");
    if (totalGeneracion != 0)
    {
        printf("\nTOTAL DE POKÉMON DE LA GENERACIÓN  %ld\n", totalGeneracion);

    }
}


void buscarPorNombre(HashMap *nombrePokemon) {
    char nombre[100];
    printf("Ingrese el nombre del Pokémon a buscar: ");
    scanf("%s", nombre);
    printf("\n");
    Pair *pair = searchMap(nombrePokemon, nombre);
    if (pair != NULL) {
        Pokemon *pokemon = (Pokemon *)pair->value;

        printf("POKÉMON ENCONTRADO\n\n");
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
    }
    else {
        printf("-------------------------------\n");
        printf("POKÉMON NO ENCONTRADO\n");
    }
    printf("------------------------------\n\n");
}


void buscarPorTipo(HashMap *tipoPokemon){
    char tipo[100];
    printf("Ingrese el tipo a buscar: ");
    scanf("%s", tipo);
    printf("\n");
    Pair *pair = searchMap(tipoPokemon, tipo);
    if (pair != NULL){
        List *pokemones = (List *)pair->value;
        Pokemon *pokemon = (Pokemon *)list_first(pokemones);
        printf("POKÉMON ENCONTRADOS\n\n");
        printf("-------------------------------\n\n");
        printf("POKÉMON TIPO %s\n\n", tipo);
        while (pokemon != NULL){
            printf("Nombre: %s\n", pokemon->nombre);
            printf("Numero: %s\n", pokemon->numero);
            char *tipos = list_first(pokemon->tipos);
            printf("Tipo/s:");
            while (tipos != NULL){
                printf(" %s", tipos);
                tipos = list_next(pokemon->tipos);
            }
            printf("\n");
            printf("Generacion: %s\n", pokemon->generacion);
            printf("Legendario: %s\n\n", pokemon->legendario);
            pokemon = (Pokemon *) list_next(pokemones);
        }
    }
    else{
        printf("-------------------------------\n");
        printf("NO HAY POKÉMON DEL TIPO %s\n", tipo);
    }
    printf("-------------------------------\n");
}


void adivinarPokemon(HashMap *numeroPokemon){
    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios
    int numAleatorio = rand() % 1072; // Genera un número aleatorio entre 0 y 1072
    char numChar[5]; // Declara un arreglo de caracteres con tamaño suficiente para almacenar la cadena

    sprintf(numChar, "%d", numAleatorio); // Convierte el número aleatorio a una cadena
    getchar();



    Pair *pair = searchMap(numeroPokemon, numChar);
    List *listaPokemones = (List *)pair->value;
    Pokemon *adivinar = (Pokemon *)list_first(listaPokemones);
    char intento[51];
    int contIntentos = 0;

    //Solo para probar si funciona correctamente adivinar
    printf("%s\n", adivinar->nombre);

    
    size_t intentos;
    intentos = 0;

    printf("Ingrese su primer Intento: ");
    scanf("%50[^\n]", intento);
    getchar();
    if (strcmp(adivinar->nombre, intento) != 0)
    {
        printf("No es el pokemon, intenta otra vez\n\n");
        printf("Los tipos de tu pokemon son: ");
        char *tipos = list_first(adivinar->tipos);

        while (tipos != NULL){
            printf(" %s", tipos);
            tipos = list_next(adivinar->tipos);
        }
        printf("\n");
        printf("Ingrese su Segundo Intento: ");
        scanf("%50[^\n]", intento);
        getchar();
        if (strcmp(adivinar->nombre, intento) != 0)
        {
            printf("No es el Pokémon, intenta otra vez\n\n");
            printf("La generacion del pokemon es:  %s\n" , adivinar->generacion);
            printf("Ingrese su Tercer Intento: ");
            scanf("%50[^\n]", intento);
            getchar();
            if (strcmp(adivinar->nombre, intento) != 0)
            {
                printf("No es el Pokémon, intenta otra vez\n\n");
                printf("Tu Pokémon es legendario? %s\n" , adivinar->legendario);
                printf("Ingrese su Cuarto Intento: ");
                scanf("%50[^\n]", intento);
                getchar();
                if (strcmp(adivinar->nombre, intento) != 0)
                {
                    printf("No es el Pokémon, intenta otra vez\n\n");
                    printf("Ultima pista, su numero en la Pokedex es %s\n" , adivinar->numero);
                    printf("Ingrese su ultimo intento: ");
                    scanf("%50[^\n]", intento);
                    getchar();
                    if (strcmp(adivinar->nombre, intento) != 0)
                    {
                        printf("Pokémon incorrecto, haz perdido :(\n");
                        printf("El Pokémon era: %s\n", adivinar->nombre);
                        return;

                    }
                    else
                    {
                        printf("Adivinaste el Pokémon!\n");
                        printf("Gracias por jugar! <3");
                        return;
                    }

                }
                else{
                    printf("Adivinaste el Pokémon!\n");
                    printf("Gracias por jugar! <3");
                    return;
                }
            }
            else{
                printf("Adivinaste el Pokémon!\n");
                printf("Gracias por jugar! <3");
                return;
            }

        }
        else{
            printf("Adivinaste el Pokémon!\n");
            printf("Gracias por jugar! <3");
            return;
        }
    }
    else{
        printf("Adivinaste el Pokémon!\n");
        printf("Gracias por jugar! <3");
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
