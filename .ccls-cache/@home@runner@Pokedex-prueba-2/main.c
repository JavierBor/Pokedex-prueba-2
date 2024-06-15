#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "TDAs/list.h"
#include "TDAs/map.h"
#include "TDAs/extra.h"

typedef struct {
    char nombre[100];
    char numero[5];
    List *tipos;
    char generacion[100];
    char legendario[100];
} Pokemon;

int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2;
}

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}


//-----------------------------------------------------------------------------------
int es_numero(const char *cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit(cadena[i])) {
            return 0;
        }
    }
    return 1;
}
void cargar_pokemon(Map *nombrePokemon, Map *tipoPokemon, Map *numeroPokemon, Map *generacionPokemon){
    FILE *archivo = fopen("Pokemon.csv", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    

    char **campos;
    campos = leer_linea_csv(archivo, ',');
    while ((campos = leer_linea_csv(archivo, ',')) != NULL){
        Pokemon *pokemon = (Pokemon *)malloc(sizeof(Pokemon));
        if (pokemon == NULL) {
            perror("Error al asignar memoria");
            return;
        }
        //printf("Hola mundo\n");
        strcpy(pokemon->numero, campos[0]);
        strcpy(pokemon->nombre, campos[1]);
        
        pokemon->tipos = list_create();
        list_pushBack(pokemon->tipos, strdup(campos[2]));
        
        if (!es_numero(campos[3])){
            list_pushBack(pokemon->tipos, strdup(campos[3]));
            strcpy(pokemon->generacion, campos[11]);
            strcpy(pokemon->legendario, campos[12]);
        }
        else
        {
            strcpy(pokemon->generacion, campos[10]);
            strcpy(pokemon->legendario, campos[11]);
        }
          void *data = list_first(pokemon->tipos);
          while (data != NULL) {
              printf("%s ", (char *)data);
              data = list_next(pokemon->tipos);
          }
          printf("\n");
        //strcpy(pokemon->generacion, campos[11]);
        //printf("Generacion: %s\n", pokemon->generacion);

        //printf("Legendario: %s\n", pokemon->legendario);
        map_insert(nombrePokemon, pokemon->nombre, pokemon);
    }
    fclose(archivo);

    MapPair *pair = map_first(nombrePokemon);
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
        
        pair = map_next(nombrePokemon);
    }
    printf("-------------------------------\n");

}


//-----------------------------------------------------------------------------------


void mostrarMenu() {
    printf("========================================\n");
    printf("     MENU PRINCIPAL\n");
    printf("========================================\n");
    printf("1. Cargar pokémon\n");
    printf("2. Buscar por nombre\n");
    printf("3. Buscar por número\n");
    printf("4. Buscar por tipo\n");
    printf("5. Buscar por generación\n");
    printf("6. Salir\n\n");
}
//-----------------------------------------------------------------------------------
void buscarPorNombre(Map *nombrePokemon)
{
    char nombre[100];
    printf("Ingrese el nombre del pokemon a buscar: ");
    scanf("%s", nombre);
    limpiarPantalla();
    MapPair *pair = map_search(nombrePokemon, nombre);
    if (pair != NULL)
    {
        Pokemon *pokemon = (Pokemon *)pair->value;
        
        printf("POKEMON ENCONTRADO\n\n");
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
        printf("Legendario: %s\n\n", pokemon->legendario);
        
        
    }
    else{
        printf("POKEMON NO ENCONTRADO\n");
    }
}
    
//-----------------------------------------------------------------------------------
int main(void) {
    int opcion;
    Map *nombrePokemon = map_create(is_equal_str);
    Map *numeroPokemon = map_create(is_equal_str);
    Map *tipoPokemon = map_create(is_equal_str);
    Map *generacionPokemon = map_create(is_equal_str);

    do {
        mostrarMenu();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch (opcion) {
        case 1:
            printf("Cargando pokémon...\n\n");
            cargar_pokemon(nombrePokemon, tipoPokemon, numeroPokemon, generacionPokemon);
            presioneTeclaParaContinuar();
            limpiarPantalla();
            break;
        case 2:
            limpiarPantalla();
            printf("Buscando por nombre...\n\n");
            buscarPorNombre(nombrePokemon);
            presioneTeclaParaContinuar();
            limpiarPantalla();
            break;
        case 3:
            printf("Buscando por número...\n\n");
            break;
        case 4:
            printf("Buscando por tipo...\n\n");
            break;
        case 5:
            printf("Buscando por generación...\n\n");
            break;
        case 6:
            printf("Saliendo...\n");
            break;
        default:
            printf("Opción inválida...\n\n");
            break;
        }
    } while (opcion != 6);
    return 0;
}