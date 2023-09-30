#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hashmap.h"

typedef struct {
    char nombre[100];
    char tipo[50];
    char direccion[200];
    char horario[100];
    char descripcion[500];
} PuntoDeInteres;

typedef struct {
    char pasaporte[20];
    char nombre[100];
    char pais[50];
    List *lugaresFavoritos;
} Turista;

// Definiendo mapas
HashMap *mapaPuntosDeInteres;
HashMap *mapaTuristas;

void registrarPuntoDeInteres() {
    
    PuntoDeInteres* nuevoPunto = (PuntoDeInteres*) malloc(sizeof(PuntoDeInteres));
    if(nuevoPunto == NULL) {
        printf("Error de memoria. No se pudo reservar memoria para el nuevo punto de interés.\n");
        return;
    }

    printf("Ingrese el nombre del lugar (máximo 100 caracteres): ");
    if(fgets(nuevoPunto->nombre, 100, stdin) == NULL || strlen(nuevoPunto->nombre) >= 99) {
        printf("Error de lectura o nombre demasiado largo. Operación abortada.\n");
        free(nuevoPunto);
        return;
    }
    nuevoPunto->nombre[strcspn(nuevoPunto->nombre, "\n")] = 0;
    
    printf("Ingrese el tipo del lugar (máximo 50 caracteres): ");
    if(fgets(nuevoPunto->tipo, 50, stdin) == NULL || strlen(nuevoPunto->tipo) >= 49) {
        printf("Error de lectura o tipo demasiado largo. Operación abortada.\n");
        free(nuevoPunto);
        return;
    }
    nuevoPunto->tipo[strcspn(nuevoPunto->tipo, "\n")] = 0;

    printf("Ingrese la dirección del lugar (máximo 200 caracteres): ");
    if(fgets(nuevoPunto->direccion, 200, stdin) == NULL || strlen(nuevoPunto->direccion) >= 199) {
        printf("Error de lectura o dirección demasiado larga. Operación abortada.\n");
        free(nuevoPunto);
        return;
    }
    nuevoPunto->direccion[strcspn(nuevoPunto->direccion, "\n")] = 0;

    printf("Ingrese el horario del lugar (máximo 100 caracteres): ");
    if(fgets(nuevoPunto->horario, 100, stdin) == NULL || strlen(nuevoPunto->horario) >= 99) {
        printf("Error de lectura o horario demasiado largo. Operación abortada.\n");
        free(nuevoPunto);
        return;
    }
    nuevoPunto->horario[strcspn(nuevoPunto->horario, "\n")] = 0;

    printf("Ingrese la descripción del lugar (máximo 500 caracteres): ");
    if(fgets(nuevoPunto->descripcion, 500, stdin) == NULL || strlen(nuevoPunto->descripcion) >= 499) {
        printf("Error de lectura o descripción demasiado larga. Operación abortada.\n");
        free(nuevoPunto);
        return;
    }
    nuevoPunto->descripcion[strcspn(nuevoPunto->descripcion, "\n")] = 0;

    insertMap(mapaPuntosDeInteres, nuevoPunto->nombre, nuevoPunto);

    printf("Punto de interés registrado con éxito.\n");
}

void mostrarDatosPuntoDeInteres(char* nombre) {
    PuntoDeInteres* punto = (PuntoDeInteres*) searchMap(mapaPuntosDeInteres, nombre);
    if(punto) {
        printf("\nNombre: %s", punto->nombre);
        printf("\nTipo: %s", punto->tipo);
        printf("\nDireccion: %s", punto->direccion);
        printf("\nHorario: %s", punto->horario);
        printf("\nDescripcion: %s\n", punto->descripcion);
    } else {
        printf("No se encontró un punto de interés con el nombre %s\n", nombre);
    }
}

void eliminarPuntoDeInteres(char *nombre) {
    HashMapEntry *entry = searchMap(mapaPuntosDeInteres, nombre);

    if (entry == NULL) {
        printf("El punto de interés con nombre %s no se encuentra.\n", nombre);
        return;
    }

    PuntoDeInteres *punto = (PuntoDeInteres *)entry->value;

    // Eliminando el punto de interés del mapa
    eraseMap(mapaPuntosDeInteres, nombre);

    // Liberando la memoria asociada al punto de interés
    free(punto);

    printf("El punto de interés con nombre %s ha sido eliminado con éxito.\n", nombre);
}

void registrarTurista(char *pasaporte, char *nombre, char *pais) {
    Turista* nuevoTurista = (Turista*) malloc(sizeof(Turista));
    
    if(nuevoTurista == NULL) {
        printf("Error de memoria. No se pudo reservar memoria para el nuevo turista.\n");
        return;
    }
    
    // Inicializando los valores del nuevo turista
    strcpy(nuevoTurista->pasaporte, pasaporte);
    strcpy(nuevoTurista->nombre, nombre);
    strcpy(nuevoTurista->pais, pais);
    nuevoTurista->lugaresFavoritos = createList();
    
    // Insertando el nuevo turista en el mapa de turistas
    insertMap(mapaTuristas, nuevoTurista->pasaporte, nuevoTurista);

    printf("Turista registrado con éxito.\n");
}
int compareStrings(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strcmp(str1, str2);
}

void agregarLugarFavoritoATurista(char *pasaporte, char *nombreLugar) {
    // Buscamos el turista en el mapa de turistas usando su pasaporte como clave
    Turista* turista = (Turista*) searchMap(mapaTuristas, pasaporte);
    
    if(turista == NULL) {
        printf("No se encontró un turista con el pasaporte %s\n", pasaporte);
        return;
    }
    
    // Verificamos si el lugar ya está en la lista de lugares favoritos del turista
    if (searchList(turista->lugaresFavoritos, nombreLugar, compareStrings) != NULL) {
        printf("%s ya está en la lista de lugares favoritos de %s\n", nombreLugar, turista->nombre);
        return;
    }

    char* lugarFavorito = strdup(nombreLugar);
    if(lugarFavorito == NULL) {
        printf("Error de memoria. No se pudo agregar el lugar favorito.\n");
        return;
    }
    
    // Agregamos el lugar a la lista de lugares favoritos del turista
    appendList(turista->lugaresFavoritos, lugarFavorito);

    printf("Lugar favorito agregado con éxito.\n");
}

void mostrarTuristas(List * turistas) {
    if (turistas == NULL) {
        printf("La lista de turistas es nula.\n");
        return;
    }

    // Obtén el primer turista de la lista
    Turista * turista = (Turista *)firstList(turistas);

    while (turista != NULL) {
        // Realiza las operaciones de impresión o lo que necesites con el turista
        printf("Nombre: %s\n", turista->nombre);
        printf("Pasaporte: %s\n", turista->pasaporte);
        printf("Pais: %s\n", turista->pais);

        // Avanza al siguiente turista
        turista = next(turistas);
    }
}

void mostrarPuntosDeInteresPorTipo(char *tipo) {
    // Obteniendo una lista con todos los puntos de interés del mapa.
    List* puntosDeInteres = getValues(mapaPuntosDeInteres);
    if(puntosDeInteres == NULL) {
        printf("Error al obtener los puntos de interés. Lista no inicializada o vacía.\n");
        return;
    }
    
    // Iterando sobre cada punto de interés en la lista.
    PuntoDeInteres* punto;
    int count = 0;
    while((punto = next(puntosDeInteres)) != NULL) {
        // Si el tipo del punto de interés coincide con el tipo ingresado, mostramos sus datos.
        if(strcmp(punto->tipo, tipo) == 0) {
            printf("\nNombre: %s", punto->nombre);
            printf("\nTipo: %s", punto->tipo);
            printf("\nDireccion: %s", punto->direccion);
            printf("\nHorario: %s", punto->horario);
            printf("\nDescripcion: %s\n", punto->descripcion);
            count++;
        }
    }
    
    // Si no se encontró ningún punto de interés del tipo especificado.
    if(count == 0) {
        printf("No se encontraron puntos de interés del tipo %s\n", tipo);
    }
}

void importarDesdeCSV(char* nombreArchivoPuntos, char* nombreArchivoTuristas) {
    FILE *archivoPuntos = fopen(nombreArchivoPuntos, "r");
    FILE *archivoTuristas = fopen(nombreArchivoTuristas, "r");

    if (archivoPuntos == NULL || archivoTuristas == NULL) {
        printf("Error al abrir uno o ambos archivos CSV.\n");
        return;
    }

    // Leer puntos de interés desde el archivo CSV
    char linea[1000];
    while (fgets(linea, sizeof(linea), archivoPuntos)) {
        PuntoDeInteres* nuevoPunto = (PuntoDeInteres*) malloc(sizeof(PuntoDeInteres));

        if(nuevoPunto == NULL) {
            printf("Error de memoria. No se pudo reservar memoria para el nuevo punto de interés.\n");
            continue; // Continua con la siguiente iteración
        }

        sscanf(linea, "%99[^,],%49[^,],%199[^,],%99[^,],%499[^\n]",
               nuevoPunto->nombre, nuevoPunto->tipo, nuevoPunto->direccion,
               nuevoPunto->horario, nuevoPunto->descripcion);
        
        // Insertamos el nuevo punto en el HashMap
        insertMap(mapaPuntosDeInteres, nuevoPunto->nombre, nuevoPunto);
    }
    fclose(archivoPuntos);

    // Leer turistas desde el archivo CSV
    while (fgets(linea, sizeof(linea), archivoTuristas)) {
        Turista* nuevoTurista = (Turista*) malloc(sizeof(Turista));
        
        if(nuevoTurista == NULL) {
            printf("Error de memoria. No se pudo reservar memoria para el nuevo turista.\n");
            continue; // Continua con la siguiente iteración
        }

        char lugaresFavoritos[500]; // Asumiendo que todos los lugares favoritos juntos no superan los 500 caracteres.
        sscanf(linea, "%19[^,],%99[^,],%49[^,],%499[^\n]",
               nuevoTurista->pasaporte, nuevoTurista->nombre, nuevoTurista->pais, lugaresFavoritos);
        
        // Aquí deberías procesar los lugares favoritos y añadirlos a la lista de lugares favoritos del turista
        nuevoTurista->lugaresFavoritos = createList();

        char *token = strtok(lugaresFavoritos, ";");
        while (token != NULL) {
            char *nombreLugar = strdup(token);
            if(nombreLugar != NULL)
                pushBack(nuevoTurista->lugaresFavoritos, nombreLugar);
            token = strtok(NULL, ";");
        }
        
        // Insertamos el nuevo turista en el HashMap
        insertMap(mapaTuristas, nuevoTurista->pasaporte, nuevoTurista);
    }
    fclose(archivoTuristas);

    printf("Datos importados desde archivos CSV.\n");
}

void exportarACSV(char *nombreArchivoPuntos, char *nombreArchivoTuristas) {
    // Abrir los archivos en modo de escritura.
    FILE *archivoPuntos = fopen(nombreArchivoPuntos, "w");
    FILE *archivoTuristas = fopen(nombreArchivoTuristas, "w");

    if (archivoPuntos == NULL || archivoTuristas == NULL) {
        printf("Error al abrir uno o ambos archivos para escribir.\n");
        return;
    }

    // Iterar sobre los puntos de interés y escribir en el archivo correspondiente.
    PuntoDeInteres *punto;
    List *puntosList = getValues(mapaPuntosDeInteres);
    if(puntosList == NULL) {
        printf("No hay puntos de interés para exportar.\n");
    } else {
        while((punto = (PuntoDeInteres *) next(puntosList)) != NULL) {
            fprintf(archivoPuntos, "%s,%s,%s,%s,%s\n", punto->nombre, punto->tipo, punto->direccion, punto->horario, punto->descripcion);
        }
    }
    // Libera la memoria de la lista de puntos de interés.
    freeList(puntosList);

    // Iterar sobre los turistas y escribir en el archivo correspondiente.
    Turista *turista;
    List *turistasList = getValues(mapaTuristas);
    if(turistasList == NULL) {
        printf("No hay turistas para exportar.\n");
    } else {
        while((turista = (Turista *) next(turistasList)) != NULL) {
            fprintf(archivoTuristas, "%s,%s,%s", turista->pasaporte, turista->nombre, turista->pais);

            // Iterar sobre los lugares favoritos de cada turista y escribir en el archivo correspondiente.
            char *lugarFavorito;
            List *favoritosList = turista->lugaresFavoritos;
            while((lugarFavorito = (char *) next(favoritosList)) != NULL) {
                fprintf(archivoTuristas, ",%s", lugarFavorito);
            }
            fprintf(archivoTuristas, "\n");
        }
    }
    // Libera la memoria de la lista de turistas.
    freeList(turistasList);

    // Cerrar los archivos.
    fclose(archivoPuntos);
    fclose(archivoTuristas);
    printf("Datos exportados exitosamente a los archivos CSV.\n");
}

int main() {
    // Inicializar mapas
    mapaPuntosDeInteres = createMap(100); // Asumiendo un tamaño inicial de 100
    mapaTuristas = createMap(100); // Asumiendo un tamaño inicial de 100
    
    // Variables para el input del usuario
    char opcion;
    char buffer[500];

    do {
        // Menu principal
        printf("1. Registrar punto de interés\n");
        printf("2. Mostrar datos punto de interés\n");
        printf("3. Eliminar punto de interés\n");
        printf("4. Registrar turista\n");
        printf("5. Agregar lugar favorito a turista\n");
        printf("6. Mostrar turistas por país\n");
        printf("7. Mostrar puntos de interés por tipo\n");
        printf("8. Importar desde CSV\n");
        printf("9. Exportar a CSV\n");
        printf("0. Salir\n");
        printf("Ingrese una opción: ");
        scanf(" %c", &opcion); // Se añade un espacio para ignorar cualquier espacio en blanco previo
        getchar(); // Limpiar el buffer del '\n'
        
        // Ejecutar la opción seleccionada
        switch(opcion) {
            case '1':
                registrarPuntoDeInteres();
                break;
            case '2':
                printf("Ingrese el nombre del punto de interés: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                mostrarDatosPuntoDeInteres(buffer);
                break;
            case '3':
                printf("Ingrese el nombre del punto de interés a eliminar: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                eliminarPuntoDeInteres(buffer);
                break;
            case '4':
                {
                    char pasaporte[20];
                    char nombre[100];
                    char pais[50];
                    
                    printf("Ingrese el pasaporte del turista: ");
                    fgets(pasaporte, sizeof(pasaporte), stdin);
                    pasaporte[strcspn(pasaporte, "\n")] = 0;

                    printf("Ingrese el nombre del turista: ");
                    fgets(nombre, sizeof(nombre), stdin);
                    nombre[strcspn(nombre, "\n")] = 0;

                    printf("Ingrese el país del turista: ");
                    fgets(pais, sizeof(pais), stdin);
                    pais[strcspn(pais, "\n")] = 0;

                    registrarTurista(pasaporte, nombre, pais);
                }
                break;
            case '5':
                {
                    char pasaporte[20];
                    char nombreLugar[100];
                    
                    printf("Ingrese el pasaporte del turista: ");
                    fgets(pasaporte, sizeof(pasaporte), stdin);
                    pasaporte[strcspn(pasaporte, "\n")] = 0;

                    printf("Ingrese el nombre del lugar favorito: ");
                    fgets(nombreLugar, sizeof(nombreLugar), stdin);
                    nombreLugar[strcspn(nombreLugar, "\n")] = 0;

                    agregarLugarFavoritoATurista(pasaporte, nombreLugar);
                }
                break;
            case '6':
                printf("Ingrese el país: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                mostrarTuristas(buffer);
                break;
            case '7':
                printf("Ingrese el tipo de punto de interés: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                mostrarPuntosDeInteresPorTipo(buffer);
                break;
            case '8':
                {
                    char nombreArchivoPuntos[100];
                    char nombreArchivoTuristas[100];

                    printf("Ingrese el nombre del archivo de puntos de interés: ");
                    fgets(nombreArchivoPuntos, sizeof(nombreArchivoPuntos), stdin);
                    nombreArchivoPuntos[strcspn(nombreArchivoPuntos, "\n")] = 0;

                    printf("Ingrese el nombre del archivo de turistas: ");
                    fgets(nombreArchivoTuristas, sizeof(nombreArchivoTuristas), stdin);
                    nombreArchivoTuristas[strcspn(nombreArchivoTuristas, "\n")] = 0;

                    importarDesdeCSV(nombreArchivoPuntos, nombreArchivoTuristas);
                }
                break;
            case '9':
                exportarACSV("puntos.csv", "turistas.csv");
                break;
            case '0':
                break;
            default:
                printf("Opción no válida\n");
        }
    } while(opcion != '0');
    
    // Liberar memoria
    freeMap(mapaPuntosDeInteres);
    freeMap(mapaTuristas);

    return 0;
}
