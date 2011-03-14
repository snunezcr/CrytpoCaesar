/*
 * Instituto Tecnologico de Costa Rica
 * Sede Santa Clara
 *
 * Escuela de Computacion
 * Arquitectura de Computadores 2011-I
 * Prof. Santiago Nunez Corrales
 *
 * Programa para el cifrado de Cesar
 */

/* Inclusion de bibliotecas */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Definiciones */
#define DIM 	6
#define LETRAS	26
#define CARS	36

/* Prototipos de funciones */
void ayuda(const char *);
int indice(char);
char caracter(int);
char caracter_en(int, int);
char caracter_des(int, int);
int encriptar(const char *, const char *, int);
int desencriptar(const char *, const char *, int);
int maximo(int *, int);
int romper(const char*, const char*);

/* Matriz global de caracteres */
char caracteres[DIM][DIM] = {
	{'a', 'b', 'c', 'd', 'e', 'f'},
	{'g', 'h', 'i', 'j', 'k', 'l'},
	{'m', 'n', 'o', 'p', 'q', 'r'},
	{'s', 't', 'u', 'v', 'w', 'x'},
	{'y', 'z', '0', '1', '2', '3'},
	{'4', '5', '6', '7', '8', '9'}
};

/* Vector de frecuencias enteras esperadas para un texto en ingles
 * escaladas entre 10000. Siempre se usara escalamiento con el maximo
 * valor de frecuencia encontrado. Estas frecuencias se calcularon usando
 * el texto completo del libro The Adventures of Sherlock Holmes de
 * Sir Arthur Conan Doyle, disponible en:
 * http://www.gutenberg.org/cache/epub/1661/pg1661.txt
 */
int frecuencias[CARS] = {
	8096, 1493, 2449, 4286, 12298, 2082,
	1831, 6650, 6982, 113, 824, 3963,
	2723, 6647, 7806, 1600, 100, 5713,
	6156, 9041, 3047, 1029, 2582, 132,
	2193, 34, 25, 17, 6,
	8, 6, 5, 5, 9, 10
};

/* Programa principal */
int main(int argc, char *argv[]) {
	/* Variables locales */
	int desplazamiento;

	/* Si no hay suficientes parametros o hay mas de los necesarios
	 * indique error
	 */
	if (argc < 4 || argc > 5) {
		ayuda(argv[0]);
		return -1;
	}

	/* Revise la bandera de operacion y ejecute la funcion respectiva con
	 * respecto a los datos de entrada
	 */

	if (strcmp(argv[3], "-e") == 0) {
		/* Si se desea encriptar, revise que exista un desplazamiento */
		if (argc == 4) {
			ayuda(argv[0]);
			return -1;
		}

		/* Convierta a entero desde string */
		desplazamiento = atoi(argv[4]);

		/* Indique que el desplazamiento debe estar entre 0 y 35 */
		if (desplazamiento < 0 || desplazamiento > 35) {
			ayuda(argv[0]);
			return -1;
		}

		/* Llame a la funcion encriptar con los parametros base */
		return encriptar(argv[1], argv[2], desplazamiento);
	}
	else if (strcmp(argv[3], "-d") == 0) {
                /* Si se desea desencriptar, debe existir un desplazamiento */
                if (argc == 4) {
                        ayuda(argv[0]);
                        return -1;
                }

                /* Convierta a entero desde string */
                desplazamiento = atoi(argv[4]);

                /* Indique que el desplazamiento debe estar entre 0 y 35 */
                if (desplazamiento < 0 || desplazamiento > 35) {
                        ayuda(argv[0]);
                        return -1;
                }

                /* Llame a la funcion desencriptar con los parametros base */
		return desencriptar(argv[1], argv[2], desplazamiento);
	} else if (strcmp(argv[3], "-b") == 0) {
                /* Llame a la funcion romper con los parametros base */
		return romper(argv[1], argv[2]);
	} else {
		/* Alguna opcion no fue adecuada */
		ayuda(argv[0]);
		return -1;
	}

	return 0;
}

/* Funcion que despliega la ayuda del programa */
void ayuda(const char *ejecutable) {
	printf("Cifrado de Cesar:\n");
	printf("-----------------\n\n");
	printf("%s fin fout flag val\n\n", ejecutable);
	printf("fin: nombre del archivo de entrada\n");
	printf("fout: nombre del archivo de salida\n");
	printf("flag: bandera para indicar operacion:\n");
	printf("\t-e: encriptar usando val (0, 35) como desplazamiento\n");
	printf("\t-d: desencriptar usando val (0, 35) como desplazamiento\n");
	printf("\t-b: romper (b: break) el codigo\n");
}

/* Funcion que retorna un valor lineal en la matriz de acuerdo al caracter
 * leido y a un offset. Retorna -1 si no es un caracter traducible.
 *
 * Nota: no considera acentos latinos.
 */
int indice(char c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	else if (c >= 'a' && c <= 'z')
                return c - 'a';
	else if (c >= '0' && c <= '9')
                return (c - '0') + LETRAS;
	else
		return -1;
}

/* Funcion que dado un indice retorna el caracter respectivo linealmente */
char caracter(int indice) {
	if (indice < LETRAS)
		return 'a' + (char)indice;
	else
		return (char)(indice - LETRAS) + '0';
}

/* Funcion que dado un valor y un offset, retorna un caracter (encripcion) */
char caracter_en(int valor, int desplazamiento) {
	int val_columna = valor / DIM;
	int val_fila = valor % DIM;
	int off_columna = desplazamiento / DIM;
	int off_fila = desplazamiento % DIM;
	int fila = (val_fila + off_fila) % DIM;
	int columna = (val_columna + off_columna) % DIM;

	return caracteres[fila][columna];
}

/* Funcion que dado un valor y un offset, retorna un caracter (desencripcion) */
char caracter_des(int valor, int desplazamiento) {
	int val_columna = valor / DIM;
	int val_fila = valor % DIM;
	int off_columna = desplazamiento % DIM;
	int off_fila = desplazamiento / DIM;
	int fila = (DIM + val_fila - off_fila) % DIM;
	int columna = (DIM + val_columna - off_columna) % DIM;

	return caracteres[fila][columna];
}

/* Funcion que encripta un trozo de texto en un archivo y lo almacena en otro */
int encriptar(const char *fin, const char *fout, int desplazamiento) {
	FILE *entrada;
	FILE *salida;
	char c;
	int posicion;

	entrada = fopen(fin, "r");

	/* Si el archivo de entrada no es legible, error */
	if (entrada == NULL) {
		printf("Error. El archivo %s no se puede leer.\n", fin);
		return -1;
	}

	salida = fopen(fout, "w");

	/* Si el archivo de salida no es legible, error */
	if (salida == NULL) {
		printf("Error. El archivo %s no se puede escribir.\n", fout);
	}

	if (salida == NULL)
		return -1;

	do {
		c = fgetc(entrada);
		posicion = indice(c);
		if (posicion != -1) {
			fputc(caracter_en(posicion, desplazamiento), salida);
		}
	} while (c != EOF);

	/* Cierre de los archivos */
	fclose(salida);
	fclose(entrada);

	return 0;
}


/* Funcion que desencripta un trozo de texto de archivo y lo almacena en otro */
int desencriptar(const char *fin, const char *fout, int desplazamiento) {
	FILE *entrada;
	FILE *salida;
	char c;
	int posicion;

	entrada = fopen(fin, "r");

	/* Si el archivo de entrada no es legible, error */
	if (entrada == NULL) {
		printf("Error. El archivo %s no se puede leer.\n", fin);
		return -1;
	}

	salida = fopen(fout, "w");

	/* Si el archivo de salida no es legible, error */
	if (salida == NULL) {
		printf("Error. El archivo %s no se puede escribir.\n", fout);
	}

	if (salida == NULL)
		return -1;

	do {
		c = fgetc(entrada);
		posicion = indice(c);		

		if (posicion != -1) {
			/* Se calcula la posicion original */
			fputc(caracter_des(posicion, desplazamiento), salida);
		}
	} while (c != EOF);

	/* Cierre de los archivos */
	fclose(salida);
	fclose(entrada);

	return 0;
}

/* Funcion que obtiene el maximo de un arreglo de tamano fijo */
int maximo(int *arreglo, int n) {
	int i;
	int mayor = -1;

	if (n == 0 || arreglo == NULL)
		return mayor;

	for (i = 0; i < n; i++)
		if (arreglo[i] > mayor)
			mayor = arreglo[i];

	return mayor;
}

/* Funcion que rompe el cifrado de un archivo */
int romper(const char *fin, const char *fout) {
	FILE *entrada;
	FILE *salida;
	char c;
	int posicion;
	/* Tabla de frecuencias del archivo inician en 0*/
	int freqs_archivo[CARS];
	int total_caracteres;
	/* Variables para el calculo de la desviacion estandar */
	int i;
	int j;
	int mayor_ref;
	int mayor_local;
	int factor_max;
	int factor_min;
	int escala;
	int variancia_minima;
	int variancia_actual;
	int desplazamiento;
	int tmp;

	entrada = fopen(fin, "r");

	/* Si el archivo de entrada no es legible, error */
	if (entrada == NULL) {
		printf("Error. El archivo %s no se puede leer.\n", fin);
		return -1;
	}

	salida = fopen(fout, "w");

	/* Si el archivo de salida no es legible, error */
	if (salida == NULL) {
		printf("Error. El archivo %s no se puede escribir.\n", fout);
	}

	if (salida == NULL)
		return -1;

	printf("Creando tabla de frecuencias...\n");

	/* No se ha visto ningun caracter */
	total_caracteres = 0;

	/* Se inicializa la tabla local de frecuencias en 0 */
	for (i = 0; i < CARS; i++)
		freqs_archivo[i] = 0;

	do {
		c = fgetc(entrada);
		posicion = indice(c);		

		if (posicion != -1) {
			/* Sumar 1 a la posicion de las frecuencias */
			freqs_archivo[posicion]++;
		}
	} while (c != EOF);

	/* Imprimir la tabla de frecuencias */
	printf("Frecuencias identificadas en el archivo a romper:\n\n");
	for (i = 0; i < CARS; i++)
		printf("%c: %d\n", caracter(i), freqs_archivo[i]);

	/* Se calcula el factor de ponderacion para calculo de variancia y
	 * mantener al minimo el error numerico
	 */
	mayor_ref = maximo(frecuencias, CARS);
	mayor_local = maximo(freqs_archivo, CARS);

	/* Se calcula el maximo y el minimo para escalar la tabla local */
	if (mayor_ref > mayor_local) {
		factor_max = mayor_ref;
		factor_min = mayor_local;
	} else {
		factor_max = mayor_local;
		factor_min = mayor_ref;
	}

	/* Se efectua un escalamiento entero */
	escala = factor_max / factor_min;

	printf("\nEscalando valores para calcular desviacion: %d %d = %d\n\n",
						mayor_ref, mayor_local, escala);

	/* Se escalan todos los valores de la tabla local */
	if (mayor_ref > mayor_local) {
		for (i = 0; i < CARS; i++)
			freqs_archivo[i] *= escala;
	} else {
		for (i = 0; i < CARS; i++)
			freqs_archivo[i] /= escala;
	}

	printf("Frecuencias del archivo escaladas:\n\n");
	for (i = 0; i < CARS; i++)
		printf("%c: %d\n", caracter(i), freqs_archivo[i]);

	variancia_minima = 1 << 30;
	desplazamiento = -1;

	/* Se calcula la desviacion estandar total y se deja la minima */
	for (j = 0; j < CARS; j++) {
		variancia_actual = 0;

		for (i = 0; i < CARS; i++) {
			tmp = freqs_archivo[(i + j) % CARS];
			tmp -= frecuencias[i];
			tmp *= tmp;
			printf("step: %d\n", tmp);
			//tmp *= factor_max;
			variancia_actual += tmp;
		}
		printf("Variancia: %d Desp: %d \n", variancia_actual, j);

		if (variancia_actual < variancia_minima) {
			variancia_minima = variancia_actual;
			desplazamiento = j;
		}
	}

	printf("Variancia minima: %d Desplazamiento: %d\n", variancia_minima, desplazamiento);

	/* Cierre de los archivos */
	fclose(salida);
	fclose(entrada);

	return 0;
}

