  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <ctype.h>
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
    printf("1. Cargar pokémon\n");
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
    printf("POKEMONES CARGADOS CORRECTAMENTE!!! ツ \n");
    printf("TOTAL DE POKEMONES CARGADOS CORRECTAMENTE: %ld\n", totalPokemones);


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

        printf("POKEMONES ENCONTRADOS\n\n");
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
        printf("NO HAY POKEMONS DE LA GENERACIÓN %s\n", generacion);
    }


    printf("-------------------------------\n");
    if (totalGeneracion != 0)
    {
        printf("\nTOTAL DE POKEMONES DE LA GENERACIÓN  %ld\n", totalGeneracion);

    }
}

void buscarPorNombre(HashMap *nombrePokemon) {
    char nombre[100];
    printf("Ingrese el nombre del pokemon a buscar: ");
    scanf("%s", nombre);
    printf("\n");
    Pair *pair = searchMap(nombrePokemon, nombre);
    if (pair != NULL) {
        Pokemon *pokemon = (Pokemon *)pair->value;

        printf("POKEMON ENCONTRADO\n\n");
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
        printf("POKEMON NO ENCONTRADO\n");
    }
    printf("------------------------------\n\n");
}

void mostrarTipo(List* listaPokeTipos){
    char *tipo = (char *)list_first(listaPokeTipos);
    printf("%s", tipo);
    tipo = (char *)list_next(listaPokeTipos);
    if (tipo != NULL)
        printf(", %s", tipo);
    printf(".\n");
    return;
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
        printf("POKEMON ENCONTRADOS\n\n");
        printf("-------------------------------\n\n");
        printf("POKEMON TIPO %s\n\n", tipo);
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
        printf("NO HAY POKEMON DEL TIPO %s\n", tipo);
    }
    printf("-------------------------------\n");
}




void adivinarPokemon(HashMap *numeroPokemon){
    int numAleatorio = rand() % 1071;
    char *numChar;
    sprintf(numChar, "%d", numAleatorio);
    
    Pair *pair = searchMap(numeroPokemon, numChar);
    List *listaPokemones = (List *)pair->value;
    Pokemon *adivinar = (Pokemon *)list_first(listaPokemones);
    char intento[50];
    int contIntentos = 0;

    while (true){
        if (contIntentos == 4){
        printf("Lamentablemente no haz acertado el pokemon, el cual era %s\n", adivinar->nombre);
        }
        if (contIntentos >= 0){
            printf("El/Los tipo/s del pokemon es/son: ");
            mostrarTipo(adivinar->tipos);
        }

        if (contIntentos >= 1){
            printf("El pokemon es de la generacion: %s\n", adivinar->generacion);
        }

        if (contIntentos >= 2){
            if (strcmp(adivinar->legendario, "TRUE") == 0){
                printf("El pokemon es legendario\n");
            }
            else{
                printf("El pokemon no es legendario\n");
            }
        }

        if (contIntentos >= 3){
            printf("!!!ULTIMO INTENTO!!!\n");
            printf("El numero del pokemon es: %s\n", adivinar->numero);
        }
        
        printf("Ingrese el nombre del pokemon: ");
        scanf(" %s", intento);
        
        if (strcmp(intento, adivinar->nombre) == 0){
            printf("!!!Haz acertado!!!\n");
            return;
        }

        else{
            printf("Parece que ese pokémon no es el correcto, intenta de nuevo\n");
        }
        contIntentos++;
    }
}
/*
void adivinarPokemon(HashMap *numeroPokemon){
    int numAleatorio = rand() % 1071;
    char *numChar;
    sprintf(numChar, "%d", numAleatorio);

    Pair *pair = searchMap(numeroPokemon, numChar);
    List *listaPokemones = (List *)pair->value;
    Pokemon *adivinar = (Pokemon *)list_first(listaPokemones);
    char intento[50];
    int contIntentos = 0;

    while (true){
        if (contIntentos == 4){
        printf("Lamentablemente no haz acertado el pokemon, el cual era %s\n", adivinar->nombre);
        }
        if (contIntentos >= 0){
            printf("El/Los tipo/s del pokemon es/son: ");
            mostrarTipo(adivinar->tipos);
        }

        if (contIntentos >= 1){
            printf("El pokemon es de la generacion: %s\n", adivinar->generacion);
        }

        if (contIntentos >= 2){
            if (strcmp(adivinar->legendario, "TRUE") == 0){
                printf("El pokemon es legendario\n");
            }
            else{
                printf("El pokemon no es legendario\n");
            }
        }

        if (contIntentos >= 3){
            printf("!!!ULTIMO INTENTO!!!\n");
            printf("El numero del pokemon es: %s\n", adivinar->numero);
        }

        printf("Ingrese el nombre del pokemon: ");
        scanf(" %s", intento);

        if (strcmp(intento, adivinar->nombre) == 0){
            printf("!!!Haz acertado!!!\n");
            return;
        }

        else{
            printf("Parece que ese pokémon no es el correcto, intenta de nuevo\n");
        }
        contIntentos++;
    }
}
*/



int main(void) {
    int opcion;
    HashMap *nombrePokemon = createMap(2000);
    HashMap *tipoPokemon = createMap(2000);
    HashMap *numeroPokemon = createMap(2000);
    HashMap *generacionPokemon = createMap(2000);
    int cont = 0;
    
    do {
        mostrarMenu();
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                if (cont == 0){
                    printf("Cargando pokémon...\n\n");
                    cargar_pokemon(nombrePokemon, tipoPokemon, numeroPokemon, generacionPokemon);
                }
                else{
                    printf("Los pokémon ya han sido cargados, muchas gracias!! =)\n");
                }
                cont++;
                break;
            case 2:
                printf("Buscando por nombre...\n\n");
                buscarPorNombre(nombrePokemon);
                break;
            case 3:
                printf("Buscando por número...\n\n");
                // Falta implementar la función buscarPorNumero
                break;
            case 4:
                printf("Buscando por tipo...\n\n");
                buscarPorTipo(tipoPokemon);
                // Falta implementar la función buscarPorTipo
                break;
            case 5:
                printf("Buscando por generación...\n\n");
                buscarPorGeneracion(generacionPokemon);
                // Falta implementar la función buscarPorGeneracion
                break;
            case 6:
                printf("Entrando a WHO IS THAT POKEMON?\n\n");
                break;
            case 7:
                printf("Saliendo de la pokédex\n");
                break;
            default:
                printf("Opción inválida. Por favor intenta de nuevo con una opcion valida\n\n");
                break;
        }
        presioneTeclaParaContinuar(); 
        limpiarPantalla(); 
    } while (opcion != 7);
    return 0;
}
