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
        if (!es_numero(campos[3])){
            list_pushBack(pokemon->tipos, strdup(campos[3]));
            strcpy(pokemon->generacion, campos[11]);
            strcpy(pokemon->legendario, campos[12]);

            Pair *buscarTipo = searchMap(tipoPokemon, campos[3]);
            if ( buscarTipo == NULL || buscarTipo->value == NULL){
                listaPokeTipos = list_create();
                list_pushBack(listaPokeTipos, pokemon);
                insertMap(tipoPokemon, strdup(campos[3]), listaPokeTipos);
            }

            else{
                List *listaPokeTiposEncontrados = buscarTipo->value;
                list_pushBack(listaPokeTiposEncontrados, pokemon);
            }
        }
        else
        {
            strcpy(pokemon->generacion, campos[10]);
            strcpy(pokemon->legendario, campos[11]);
        }
        insertMap(nombrePokemon, pokemon->nombre, pokemon);
        }
        fclose(archivo);
// EL QUE LEA ESTO, AUN QUE NO ESTE IMPLEMENTADO EL DE TIPOS Y GENERACION DEBERIA CARGAR LOS MAPAS IGUAL, SI NO ME CREEN PRUEBEN CON EL REPLIT REALG  
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
          printf("Legendario: %s\n\n", pokemon->legendario);
      } else {
          printf("-------------------------------\n");
          printf("POKEMON NO ENCONTRADO\n");
      }
      printf("------------------------------\n");
  }

  int main(void) {
      int opcion;
      HashMap *nombrePokemon = createMap(2000);
      HashMap *tipoPokemon = createMap(2000);
      HashMap *numeroPokemon = createMap(2000);
      HashMap *generacionPokemon = createMap(2000);

      do {
          mostrarMenu();
          printf("Ingrese su opción: ");
          scanf("%d", &opcion);
          switch (opcion) {
              case 1:
                  printf("Cargando pokémon...\n\n");
                  cargar_pokemon(nombrePokemon, tipoPokemon, numeroPokemon, generacionPokemon);
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
                  // Falta implementar la función buscarPorTipo
                  break;
              case 5:
                  printf("Buscando por generación...\n\n");
                  // Falta implementar la función buscarPorGeneracion
                  break;
              case 6:
                  printf("Saliendo de la pokédex\n");
                  break;
              default:
                  printf("Opción inválida. Por favor intenta de nuevo con una opcion valida\n\n");
                  break;
          }
          presioneTeclaParaContinuar(); 
          limpiarPantalla(); 
      } while (opcion != 6);
      return 0;
  }
