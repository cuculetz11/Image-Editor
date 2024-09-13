#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
	char magic_number[2];
	int width;
	int height;
	int max_value;
	unsigned char ***pixels;
	int tip; // 0 pt PGM, 1 pt PPM
	int rgb;
	int x1, y1, x2, y2;
} image;

void alocare_din_3d(int n, int m, int k, unsigned char ****mat)
{
	unsigned char ***a = (unsigned char ***)malloc(n *
						sizeof(unsigned char **));
	if (!a) {
		printf("Failed to alloc memory\n");
		*mat = NULL;
		return;
	}
	*mat = a;
	for (int i = 0; i < n; i++) {
		a[i] = (unsigned char **)malloc(m * sizeof(unsigned char *));
		if (!a[i]) {
			printf("Failed to alloc memory\n");
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			*mat = NULL;
			return;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			a[i][j] = (unsigned char *)malloc(k * sizeof(unsigned char));
			if (!a[i][j]) {
				for (int x = 0; x < i; x++) {
					for (int y = 0; y < j; y++)
						free(a[x][y]);
					free(a[x]);
				}
				free(a);
				*mat = NULL;
				return;
			}
		}
	}
}

void read_matrix_a(int n, int m, int k, unsigned char ****mat,
				   char *nume_fisier, FILE *f)
{
	unsigned char ***a = (unsigned char ***)malloc(n *
						sizeof(unsigned char **));
	if (!a) {
		printf("Failed to load %s\n", nume_fisier);
		*mat = NULL;
		return;
	}
	*mat = a;
	for (int i = 0; i < n; i++) {
		a[i] = (unsigned char **)malloc(m * sizeof(unsigned char *));
		if (!a[i]) {
			printf("Failed to load %s\n", nume_fisier);
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			*mat = NULL;
			return;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			a[i][j] = (unsigned char *)malloc(k * sizeof(unsigned char));
			if (!a[i][j]) {
				for (int x = 0; x < i; x++) {
					for (int y = 0; y < j; y++)
						free(a[x][y]);
					free(a[x]);
				}
				free(a);
				*mat = NULL;
				return;
			}
		}
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < k; l++)
				fscanf(f, "%hhu", &a[i][j][l]);
}

void read_matrix_b(int n, int m, int k, unsigned char ****mat,
				   char *nume_fisier, FILE *f)
{
	unsigned char ***a = (unsigned char ***)malloc(n *
						sizeof(unsigned char **));
	if (!a) {
		printf("Failed to load %s\n", nume_fisier);
		*mat = NULL;
		return;
	}
	*mat = a;
	for (int i = 0; i < n; i++) {
		a[i] = (unsigned char **)malloc(m * sizeof(unsigned char *));
		if (!a[i]) {
			printf("Failed to load %s\n", nume_fisier);
			for (int j = 0; j < i; j++)
				free(a[j]);
			free(a);
			*mat = NULL;
			return;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			a[i][j] = (unsigned char *)malloc(k * sizeof(unsigned char));
			if (!a[i][j]) {
				for (int x = 0; x < i; x++) {
					for (int y = 0; y < j; y++)
						free(a[x][y]);
					free(a[x]);
				}
				free(a);
				*mat = NULL;
				return;
			}
		}
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < k; l++) {
				unsigned char x;
				fread(&x, sizeof(unsigned char), 1, f);
				a[i][j][l] = x;
			}
}

void LOAD(image *img, int *load, char *nume_fisier)
{
	*load = 0;
	FILE *f = fopen(nume_fisier, "r");
	if (!f) {
		printf("Failed to load %s\n", nume_fisier);
	} else {
		//citesc elemenetele din fisier cu exceptia comentariului
		char test[50];
		fscanf(f, "%s", test);
		if (test[0] != '#')
			strcpy((*img).magic_number, test);

		fscanf(f, "%s", test);
		if (test[0] != '#')
			(*img).width = atoi(test);
		fscanf(f, "%s", test);
		if (test[0] != '#')
			(*img).height = atoi(test);
		fscanf(f, "%s", test);
		if (test[0] != '#')
			(*img).max_value = atoi(test);
		if (strcmp("P3", (*img).magic_number) == 0) {
			read_matrix_a((*img).height, (*img).width, 3,
						  &(*img).pixels, nume_fisier, f);
			*load = 1;
			(*img).tip = 1;
			img->rgb = 3;
		}
		if (strcmp("P2", (*img).magic_number) == 0) {
			read_matrix_a((*img).height, (*img).width, 1,
						  &(*img).pixels, nume_fisier, f);
			*load = 1;
			(*img).tip = 0;
			img->rgb = 1;
		}
		if (strcmp("P6", (*img).magic_number) == 0) {
			fgetc(f);
			read_matrix_b((*img).height, (*img).width, 3,
						  &(*img).pixels, nume_fisier, f);
			*load = 1;
			(*img).tip = 1;
			img->rgb = 3;
		}
		if (strcmp("P5", (*img).magic_number) == 0) {
			fgetc(f);
			read_matrix_b((*img).height, (*img).width, 1,
						  &(*img).pixels, nume_fisier, f);
			*load = 1;
			(*img).tip = 0;
			img->rgb = 1;
		}
		if (*load == 1) {
			printf("Loaded %s\n", nume_fisier);
			img->x1 = 0;
			img->y1 = 0;
			img->x2 = img->width;
			img->y2 = img->height;
		}
		fclose(f);
	}
}

void free_aloc_3d(int n, int m, unsigned char ***mat)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			free(mat[i][j]);
		free(mat[i]);
	}
	free(mat);
}

void SELECT(image *img, char *test)
{
	int x1, x2, y1, y2;
	if (*test == '-') {
		printf("Invalid set of coordinates\n");
	} else if (isdigit(*test)) {
		char *p = strtok(test, " ");
		x1 = atoi(p);
		p = strtok(NULL, " ");
		if (!p || isdigit(*p) == 0) {
			printf("Invalid command\n");
			return;
		}
		y1 = atoi(p);
		p = strtok(NULL, " ");
		if (!p || isdigit(*p) == 0) {
			printf("Invalid command\n");
			return;
		}
		x2 = atoi(p);
		p = strtok(NULL, "\n");
		if (!p || isdigit(*p) == 0) {
			printf("Invalid command\n");
			return;
		}
		y2 = atoi(p);

		if (x1 > x2) {
			int aux;
			aux = x1;
			x1 = x2;
			x2 = aux;
		}

		if (y1 > y2) {
			int aux;
			aux = y1;
			y1 = y2;
			y2 = aux;
		}
		if (x1 < 0 || y1 < 0 || x2 <= 0 || y2 <= 0) {
			printf("Invalid set of coordinates\n");
			return;
		}

		if (y1 < 0 || x2 <= 0 || y2 <= 0) {
			printf("Invalid set of coordinates\n");
			return;
		}

		if (y2 > img->height || x2 > img->width) {
			printf("Invalid set of coordinates\n");
			return;
		}
		if (x1 == x2 || y1 == y2) {
			printf("Invalid set of coordinates\n");
			return;
		}
		img->x1 = x1;
		img->y1 = y1;
		img->x2 = x2;
		img->y2 = y2;
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	} else if (strcmp(test, "ALL") == 0 || strcmp(test, "ALL ") == 0 ||
			   strcmp(test, "ALL\n") == 0) {
		img->x1 = 0;
		img->y1 = 0;
		img->x2 = img->width;
		img->y2 = img->height;
		printf("Selected ALL\n");
	} else {
		printf("Invalid command\n");
	}
}

void HISTOGRAM(image img, char *pixeli)
{
	int x, y;
	char *p = strtok(pixeli, " ");
	if (img.tip == 1) {
		printf("Black and white image needed\n");
		return;
	}
	if (isdigit(*p) == 0) {
		printf("Invalid command\n");
		return;
	}
	x = atoi(p);
	p = strtok(NULL, "\n ");
	if (!p || isdigit(*p) == 0) {
		printf("Invalid command\n");
		return;
	}
	y = atoi(p);

	p = strtok(NULL, "\n ");
	if (p) {
		printf("Invalid command\n");
		return;
	}
	if (x < 0 || y < 0) {
		printf("Invalid command\n");
		return;
	}
	int frecv[256];
	memset(frecv, 0, sizeof(frecv));
	for (int i = 0; i < img.height; i++)
		for (int j = 0; j < img.width; j++)
			frecv[img.pixels[i][j][0]]++;
	int pas = 256 / y, contor = 0;
	long long frecv_grup[y + 1], s = 0;
	for (int i = 0; i < 256; i++) {
		s = s + frecv[i];
		if ((i + 1) % pas == 0) {
			frecv_grup[contor] = s;
			s = 0;
			contor++;
		}
	}
	int maxx = 0;
	for (int i = 0; i < y; i++) {
		if (frecv_grup[i] > maxx)
			maxx = frecv_grup[i];
	}
	for (int i = 0; i < y; i++) {
		int formula = (int)(frecv_grup[i] * x / maxx);
		printf("%d\t|\t", formula);
		for (int k = 0; k < formula; k++)
			printf("*");
		printf("\n");
	}
}

void EQUALIZE(image *img)
{
	if (img->tip != 0) {
		printf("Black and white image needed\n");
		return;
	}
	int frecv[256] = {0};
	// Calculez frecventa fiecarui pixel
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++)
			frecv[img->pixels[i][j][0]]++;
	}
	/* Calculez noua valoare a fiecarui pixel folosind formula
	data(egalnd histograma)*/
	double H[256] = {0};
	double s = 0;
	int area = img->height * img->width;

	for (int i = 0; i < 256; i++) {
		s = s + frecv[i];
		H[i] = round((255 * s) / area);
		if (H[i] > 255)
			H[i] = 255;
		if (H[i] < 0)
			H[i] = 0;
	}
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++)
			img->pixels[i][j][0] = (unsigned char)H[img->pixels[i][j][0]];
	}
	printf("Equalize done\n");
}

void CROP(image *img)
{
	unsigned char ***copie;
	int m = img->x2 - img->x1;
	int n = img->y2 - img->y1;
	alocare_din_3d(n, m, img->rgb, &copie);
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++)
			for (int l = 0; l < img->rgb; l++)
				copie[i - img->y1][j - img->x1][l] = img->pixels[i][j][l];
	//creez o copie egala cu portiunea selectata
	free_aloc_3d(img->height, img->width, img->pixels);
	img->pixels = copie;
	img->height = n;
	img->width = m;
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = m;
	img->y2 = n;
	//noua imagine va avea caracteristicile portiunii selectate
	printf("Image cropped\n");
}

void SAVE(image img, char *text)
{
	char *p = strtok(text, "\n ");
	char *nume_fisier = p;
	p = strtok(NULL, "\n ");
	if (!p) {
		//pentru binar folosesc "wb" si fwrite pentru matricea de pixeli
		FILE *f = fopen(nume_fisier, "wb");
		if (!f) {
			printf("Failed to save %s\n", nume_fisier);
			return;
		}
		if (strcmp(img.magic_number, "P2") == 0)
			fprintf(f, "P5");
		else if (strcmp(img.magic_number, "P3") == 0)
			fprintf(f, "P6");
		else
			fprintf(f, "%s", img.magic_number);
		fprintf(f, "\n");
		fprintf(f, "%d %d", img.width, img.height);
		fprintf(f, "\n");
		fprintf(f, "%d", img.max_value);
		fprintf(f, "\n");
		for (int i = 0; i < img.height; i++)
			for (int j = 0; j < img.width; j++)
				for (int l = 0; l < img.rgb; l++)
					fwrite(&img.pixels[i][j][l], sizeof(unsigned char), 1, f);
		fclose(f);
		printf("Saved %s\n", nume_fisier);
		return;
	} else if (strcmp(p, "ascii") == 0) {
		//pentru ascii folosesc "w" si fprintf pentru matricea de pixeli
		FILE *f = fopen(nume_fisier, "w");
		if (!f) {
			printf("Failed to save %s\n", nume_fisier);
			return;
		}
		if (strcmp(img.magic_number, "P5") == 0)
			fprintf(f, "P2");
		else if (strcmp(img.magic_number, "P6") == 0)
			fprintf(f, "P3");
		else
			fprintf(f, "%s", img.magic_number);
		fprintf(f, "\n");
		fprintf(f, "%d %d", img.width, img.height);
		fprintf(f, "\n");
		fprintf(f, "%d", img.max_value);
		fprintf(f, "\n");
		for (int i = 0; i < img.height; i++) {
			for (int j = 0; j < img.width; j++)
				for (int l = 0; l < img.rgb; l++)
					fprintf(f, "%hhu ", img.pixels[i][j][l]);
			fprintf(f, "\n");
		}
		fclose(f);
		printf("Saved %s\n", nume_fisier);
		return;
	}
}

void EDGE(image *img)
{
	unsigned char ***copie;
	int n = img->y2 - img->y1;
	int m = img->x2 - img->x1;
	alocare_din_3d(n, m, img->rgb, &copie);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				copie[i][j][l] = img->pixels[i + img->y1][j + img->x1][l];
	//creez o copie egala cu portiunea selectata si aplic filtrul
	int a = 0, b = n, c = 0, d = m;
	if (img->y1 == 0)
		a = 1;
	if (img->y2 == img->height)
		b = n - 1;
	if (img->x1 == 0)
		c = 1;
	if (img->x2 == img->width)
		d = m - 1;
	//vad daca pixelii au suficenti vecini pentru a stii unde sa aplic filtrul
	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++)
			for (int l = 0; l < img->rgb; l++) {
				double sum_r = 0;
				sum_r = sum_r + img->pixels[img->y1 + i - 1][img->x1 + j - 1]
						[l] * (-1) + img->pixels[img->y1 + i - 1][img->x1 + j]
						[l] * (-1) + img->pixels[img->y1 + i - 1][img->x1 + j
						 + 1][l] * (-1);
				sum_r = sum_r + img->pixels[img->y1 + i][img->x1 + j - 1][l]
						* (-1) + img->pixels[img->y1 + i][img->x1 + j][l] * 8
						+ img->pixels[img->y1 + i][img->x1 + j + 1][l] * (-1);
				sum_r = sum_r + img->pixels[img->y1 + i + 1][img->x1 + j - 1]
						[l] * (-1) + img->pixels[img->y1 + i + 1][img->x1 + j]
						[l] * (-1) + img->pixels[img->y1 + i + 1][img->x1 + j
						+ 1][l] * (-1);
				if (sum_r > 255)
					sum_r = 255;
				if (sum_r < 0)
					sum_r = 0;
				copie[i][j][l] = (unsigned char)round(sum_r);
			}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				img->pixels[i + img->y1][j + img->x1][l] = copie[i][j][l];
	free_aloc_3d(n, m, copie);
	printf("APPLY EDGE done\n");
}

void SHARPEN(image *img)
{
	unsigned char ***copie;
	int n = img->y2 - img->y1;
	int m = img->x2 - img->x1;
	alocare_din_3d(n, m, img->rgb, &copie);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				copie[i][j][l] = img->pixels[i + img->y1][j + img->x1][l];
	int a = 0, b = n, c = 0, d = m;
	if (img->y1 == 0)
		a = 1;
	if (img->y2 == img->height)
		b = n - 1;
	if (img->x1 == 0)
		c = 1;
	if (img->x2 == img->width)
		d = m - 1;
	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++)
			for (int l = 0; l < img->rgb; l++) {
				double sum_r = 0;
				sum_r = sum_r + img->pixels[img->y1 + i - 1][img->x1 + j][l]
						* (-1);
				sum_r = sum_r + img->pixels[img->y1 + i][img->x1 + j - 1][l]
						* (-1) + img->pixels[img->y1 + i][img->x1 + j][l] * 5
						+ img->pixels[img->y1 + i][img->x1 + j + 1][l] * (-1);
				sum_r = sum_r + img->pixels[img->y1 + i + 1][img->x1 + j][l]
						* (-1);
				if (sum_r > 255)
					sum_r = 255;
				if (sum_r < 0)
					sum_r = 0;
				copie[i][j][l] = (unsigned char)round(sum_r);
			}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				img->pixels[i + img->y1][j + img->x1][l] = copie[i][j][l];
	free_aloc_3d(n, m, copie);

	printf("APPLY SHARPEN done\n");
}

void BLUR(image *img)
{
	unsigned char ***copie;
	int n = img->y2 - img->y1;
	int m = img->x2 - img->x1;
	alocare_din_3d(n, m, img->rgb, &copie);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				copie[i][j][l] = img->pixels[i + img->y1][j + img->x1][l];
	int a = 0, b = n, c = 0, d = m;
	if (img->y1 == 0)
		a = 1;
	if (img->y2 == img->height)
		b = n - 1;
	if (img->x1 == 0)
		c = 1;
	if (img->x2 == img->width)
		d = m - 1;
	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++)
			for (int l = 0; l < img->rgb; l++) {
				double sum_r = 0;
				sum_r = sum_r + img->pixels[img->y1 + i - 1][j + img->x1 - 1]
						[l] + img->pixels[img->y1 + i - 1][j + img->x1][l] +
						img->pixels[img->y1 + i - 1][j + img->x1 + 1][l];
				sum_r = sum_r + img->pixels[img->y1 + i][j + img->x1 - 1][l] +
						img->pixels[img->y1 + i][j + img->x1][l] + img->pixels
						[img->y1 + i][j + img->x1 + 1][l];
				sum_r = sum_r + img->pixels[img->y1 + i + 1][j + img->x1 - 1]
						[l] + img->pixels[img->y1 + i + 1][j + img->x1][l] +
						img->pixels[img->y1 + i + 1][j + img->x1 + 1][l];
				sum_r = (double)sum_r / 9.0;
				copie[i][j][l] = (unsigned char)round(sum_r);
			}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				img->pixels[i + img->y1][j + img->x1][l] = copie[i][j][l];
	free_aloc_3d(n, m, copie);

	printf("APPLY BLUR done\n");
}

void GAUSSIAN_BLUR(image *img)
{
	unsigned char ***copie;
	int n = img->y2 - img->y1;
	int m = img->x2 - img->x1;
	alocare_din_3d(n, m, img->rgb, &copie);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				copie[i][j][l] = img->pixels[i + img->y1][j + img->x1][l];
	int a = 0, b = n, c = 0, d = m;
	if (img->x1 == 0)
		a = 1;
	if (img->y1 == 0)
		c = 1;
	if (img->x2 == img->width)
		b = n - 1;
	if (img->y2 == img->height)
		d = m - 1;
	for (int i = a; i < b; i++)
		for (int j = c; j < d; j++)
			for (int l = 0; l < img->rgb; l++) {
				double sum_r = 0;
				sum_r = sum_r + img->pixels[i + img->y1 - 1][j + img->x1 - 1]
						[l] * 1 + img->pixels[i + img->y1 - 1][j + img->x1][l]
						* 2 + img->pixels[i + img->y1 - 1][j + img->x1 + 1][l];
				sum_r = sum_r + img->pixels[i + img->y1][j + img->x1 - 1][l]
						* 2 + img->pixels[i + img->y1][j + img->x1][l] * 4 +
						 img->pixels[i + img->y1][j + img->x1 + 1][l] * 2;
				sum_r = sum_r + img->pixels[i + img->y1 + 1][j + img->x1 - 1]
						[l] * 1 + img->pixels[i + img->y1 + 1][j + img->x1][l]
						* 2 + img->pixels[i + img->y1 + 1][j + img->x1 + 1][l];
				sum_r = (double)sum_r / 16.0;
				copie[i][j][l] = (unsigned char)round(sum_r);
			}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			for (int l = 0; l < img->rgb; l++)
				img->pixels[i + img->y1][j + img->x1][l] = copie[i][j][l];
	free_aloc_3d(n, m, copie);

	printf("APPLY GAUSSIAN_BLUR done\n");
}

void APPLY(image *img_s, char *comanda)
{
	if (img_s->tip == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	} else if (strcmp(comanda, "EDGE") == 0) {
		EDGE(img_s);
		return;
	} else if (strcmp(comanda, "BLUR") == 0) {
		BLUR(img_s);
		return;
	} else if (strcmp(comanda, "SHARPEN") == 0) {
		SHARPEN(img_s);
		return;
	} else if (strcmp(comanda, "GAUSSIAN_BLUR") == 0) {
		GAUSSIAN_BLUR(img_s);
		return;
	} else if (*comanda) {
		printf("APPLY parameter invalid\n");
	}
}

void ROTATE(image *img, int unghi)
{
	int unghi_c = unghi;
	int n = img->y2 - img->y1;
	int m = img->x2 - img->x1;
	if (unghi < 0)
		unghi = 360 + unghi;
	if (unghi % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (unghi == 0 || unghi == 360) {
		printf("Rotated %d\n", unghi_c);
		return;
	}
	if (n == img->height && m == img->width) {
		int rotation = unghi / 90;
		//aplic de un numar de ori rotatia de 90 de grade in functie de unghi
		for (int i = 0; i < rotation; i++) {
			unsigned char ***copie;
			alocare_din_3d(m, n, img->rgb, &copie);
		//creez o copie ce e egala cu portiunea selectata
			for (int j = 0; j < n; j++)
				for (int k = 0; k < m; k++)
					for (int l = 0; l < img->rgb; l++)
						copie[k][n - j - 1][l] = img->pixels[j + img->y1][k +
						img->x1][l];
			free_aloc_3d(n, m, img->pixels);
			int aux = n;
			n = m;
			m = aux;
			img->pixels = copie;
		}
		img->height = n;
		img->width = m;
		img->x1 = 0;
		img->y1 = 0;
		img->x2 = m;
		img->y2 = n;
	/*copia devine imaginea mea principala si schimb dimensiunile
	pentru cazul in care selectia e cat imaginea*/
		printf("Rotated %d\n", unghi_c);
	} else if (n == m) {
		int rotation = unghi / 90;
		for (int i = 0; i < rotation; i++) {
			unsigned char ***copie;
			alocare_din_3d(m, n, img->rgb, &copie);
			for (int j = 0; j < n; j++)
				for (int k = 0; k < m; k++)
					for (int l = 0; l < img->rgb; l++)
						copie[k][n - j - 1][l] = img->pixels[j + img->y1]
												[k + img->x1][l];
			for (int j = 0; j < n; j++)
				for (int k = 0; k < m; k++)
					for (int l = 0; l < img->rgb; l++)
						img->pixels[j + img->y1][k + img->x1][l] =
						copie[j][k][l];
			free_aloc_3d(n, m, copie);
		/*iau valorile din copie si le pun in imaginea principala
		 specifice penrtu portiunea selectata*/
		}
		printf("Rotated %d\n", unghi_c);
	} else if (n != m) {
		printf("The selection must be square\n");
	}
}

int main(void)
{
	int load = 0;
	while (1) {
		char comanda[15], text[50]; fgets(text, 50, stdin); image img;
		//citesc cu fgets pentru a putea vedea comenziile invalide
		char *p = strtok(text, "\n "); strcpy(comanda, p);
		if (strcmp(comanda, "LOAD") == 0) {
			p = strtok(NULL, "\n");
			if (p) {
				if (load == 1) {
					free_aloc_3d(img.height, img.width, img.pixels);
					load = 0;
				}
				LOAD(&img, &load, p);
			} else {
				printf("Invalid command\n");
			}
		} else if (strcmp(comanda, "SELECT") == 0 && load == 1) {
			p = strtok(NULL, "\n");
			if (p)
				SELECT(&img, p);
			else
				printf("Invalid command\n");
		} else if (strcmp(comanda, "SELECT") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "HISTOGRAM") == 0 && load == 1) {
			p = strtok(NULL, "\n");
			if (p)
				HISTOGRAM(img, p);
			else
				printf("Invalid command\n");
		} else if (strcmp(comanda, "HISTOGRAM") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "EQUALIZE") == 0 && load == 1) {
			EQUALIZE(&img);
		} else if (strcmp(comanda, "EQUALIZE") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "CROP") == 0 && load == 1) {
			CROP(&img);
		} else if (strcmp(comanda, "CROP") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "SAVE") == 0 && load == 1) {
			p = strtok(NULL, "\n");
			if (p)
				SAVE(img, p);
			else
				printf("Invalid command\n");
		} else if (strcmp(comanda, "SAVE") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "APPLY") == 0 && load == 1) {
			p = strtok(NULL, "\n");
			if (p)
				APPLY(&img, p);
			else
				printf("Invalid command\n");
		} else if (strcmp(comanda, "APPLY") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "ROTATE") == 0 && load == 1) {
			p = strtok(NULL, "\n");
			if (p)
				ROTATE(&img, atoi(p));
			else
				printf("Invalid command\n");
		} else if (strcmp(comanda, "ROTATE") == 0 && load == 0) {
			printf("No image loaded\n");
		} else if (strcmp(comanda, "EXIT") == 0) {
			if (load == 1)
				free_aloc_3d(img.height, img.width, img.pixels);
			else
				printf("No image loaded\n");
			break;
		} else if (comanda[0]) {
			printf("Invalid command\n");
		}
	}
}
