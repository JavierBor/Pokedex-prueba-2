# Sistema de busqueda de Pokémon

## Descripción

Este sistema permite a los usuarios buscar informacion acerca de variados Pokémon. Los usuarios pueden buscar los Pokémon por nombre, numero de la pokedex, tipos, etc. Ademas el programa trae un mini juego en el que usuario podra entretenerse adivinando diferentes Pokémon. 

## Cómo compilar y ejecutar

Dado que el sistema está diseñado para ser accesible y fácil de probar, recomendamos usar [Repl.it](http://repl.it/) para una rápida configuración y ejecución. Sigue estos pasos para comenzar:

1. Visita [Repl.it](https://repl.it/).
2. Crea una nueva cuenta o inicia sesión si ya tienes una.
3. Una vez en tu dashboard, selecciona "New Repl" y elige "Import from GitHub".
4. Pega la URL del repositorio: `https://github.com/JavierBor/Pokedex-prueba-2/settings`.
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará un entorno de ejecución.
6. Presiona el botón "Run" para compilar y ejecutar la aplicación.

## Funcionalidades

### Funcionando correctamente:

- Cargar Pokémon.
- Buscar por nombre.
- Buscar por número.
- Buscar por tipo.
- Buscar por generacion.
- WHO IS THAT POKEMON?

### Problemas conocidos:

- Las funciones que requieren que el usuario ingrese una cadena solo admite 100 carácteres.
- Los nombres de Pokémon deben estar escritos con la primera letra mayuscula (Igual que como fue ingresado), ya que de lo contrario el programa no funciona correctamente y no encontrara el/los Pokémon buscado/s.

### A mejorar:

- Implementar una interfaz de usuario más amigable.
- La funcion WHO IS THAT POKEMON? entregue pistas mas relevantes.

## Ejemplo de uso

**Paso 1: Cargar Pokemon**

Se comienza cargando los Pokémon del archivo csv en el programa.

```
Ingrese su opción: 1
cargando Pokémon...

-------------------------------
Nombre: Pignite
Numero: 499
Tipo/s: Fire Fighting
Generacion: 5
Legendario: FALSE
-------------------------------
...
```

El sistema muestra todos los Pokémon cargados correctamente.

**Paso 2: Buscar por nombre**

El usuario busca el Pokémon que desee por nombre.

```
Ingrese su opción: 2
Buscando por nombre...

Ingrese el nombre del Pokémon a buscar: Pikachu     

POKÉMON ENCONTRADO

-------------------------------
Nombre: Pikachu
Numero: 25
Tipo/s: Electric
Generacion: 1
Legendario: FALSE
-------------------------------

Presione una tecla para continuar...
```

El sistema actualiza la prioridad de Ana Martínez a "Alto", asegurando que será una de las próximas pacientes en ser atendida.

**Paso 3: Buscar por número**

El usuario busca el Pokémon que desee por numero de la pokedex.

```
Ingrese su opción: 3
Buscando por número...

Ingrese el numero del Pokémon a buscar: 23

Pokémon ENCONTRADOS

-------------------------------
Nombre: Ekans
Numero: 23
Tipo/s: Poison
Generacion: 1
Legendario: FALSE
-------------------------------
Presione una tecla para continuar...
```

Se muestra el/los Pokémon que corresponde/n al numero ingresado por el usuario.

**Paso 4: Buscar por tipo**

El usuario busca los Pokémon que correspondad a algun tipo.

```
Ingrese su opción: 4
Buscando por tipo...

Ingrese el tipo a buscar: Fire

POKÉMON TIPO Fire ENCONTRADOS

-------------------------------
Nombre: Charmander
Numero: 4
Tipo/s: Fire
Generacion: 1
Legendario: FALSE
-------------------------------
...
```

El programa muestra todos los Pokémon que correspondan al tipo ingresado por el usuario.

**Paso 5: Buscar Pokémon por generación**

El usuario ingresa una generacion que desee buscar.

````
Ingrese su opción: 5
Buscando por generación...

Ingrese la generación a buscar: 5 

POKÉMON ENCONTRADOS

-------------------------------
Nombre: Victini
Numero: 494
Tipo/s: Psychic Fire
Generacion: 5
Legendario: TRUE
-------------------------------
...
````

El progama muestra todos los Pokémon que coincidan con la genración ngresada por el usuario.

**Paso 6: WHO IS THAT POKEMON**

El usuario ingresa el Pokémon que cree que es en base a las pistas proporcionadas por el usuario.

````
Ingresar printeo.
````

El programa indica si el usuario falló o acerto, mostrando el puntaje correspondiente.

## Contribuciones (si son dos personas o más)

### Diego Alvarez:

- Creo por completo la función cargar Pokémon y también la función de buscar por nombre.
- Auto-evaluacion: 3 (Aporte Excelente).

### Javier Borquez:

- Implemento todos los TDAs que utilizaríamos dentro del código y creo la función Adivinar Pokémon
- Auto-evaluacion: 3 (Aporte Excelente).

### Benjamin Peredo:

- Implemento la función buscar por generación, ayudo en la función de buscar por tipo y ayudo en la implementación de los TDAs
- Auto-evaluacion: 3 (Aporte Excelente).

### Martin Urizar:

- Ayudo a corregir errores generales y de tipeo. Ademas, ayudo con la funcion cargar_pokemon.
- Auto-evaluacion: 3 (Aporte Excelente).