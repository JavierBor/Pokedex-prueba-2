#include <stdio.h>

void imprimirMenu(){
  printf("1) Cargar pokémon.\n");
  printf("2) Buscar pokémon por nombre.\n");
  printf("3) Buscar pokémon por número.\n");
  printf("4) Buscar pokémon por tipo.\n");
  printf("5) Buscar pokémon por generacion.\n");
  printf("6) Salir.\n");
  return;
}


int main(void) {
  int opcion;
  
  do{
    imprimirMenu();
    printf("Ingrese una opcion: ");
    scanf("%i", &opcion);
    switch(opcion){
      case 1:
        printf("Cargando pokémon\n");
        break;
      case 2:
        printf("Buscando pokémon por nombre\n");
        break;
      case 3:
        printf("Buscando pokémon por número\n");
        break;
      case 4:
        printf("Buscando pokémon por tipo\n");
        break;
      case 5:
        printf("Buscando pokémon por generacion\n");
        break;
      case 6:
        printf("Saliendo\n");
        break;
      default:
        printf("Opción no válida\n");
        break;
    }
  }while (opcion != 6);
}