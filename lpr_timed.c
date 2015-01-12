//lpt logger - wersja z zapisem czasu
//10 	Ack 	In 	Status-6
//12 	Paper-Out 	In 	Status-5 	No
//13 	Select 	In 	Status-4 	No 	
//15 	Error 	In 	Status-3 	No

#include <stdio.h>
#include <string.h>
#include <unistd.h> //dla getopt
#include <time.h>
#include <stdlib.h> //bez tego pliku nagłówkowego atof zwraca 0 (!)
#include <sys/io.h>
#include <sys/perm.h>

#define BASEPORT 0x378

struct data
{
    int reg;
    float ts;
};

struct timespec start, end;
float t_s;
struct data rec_data;


void t_probe(r)
{
    clock_gettime(CLOCK_MONOTONIC, &end);
    t_s = (float)(end.tv_sec - start.tv_sec) + (float)(end.tv_nsec - start.tv_nsec)  / 1.0e9;
    rec_data.reg = r; //zapisywana jest tylko nowa wartość
    rec_data.ts = t_s;
}


int main(int argc, char *argv[])
{
    char *filename = "test.bin";
    float endtime = 10.0;
    int maxloops = 1e7;
    int opt = 0;
    int loops = 0;

    ioperm(BASEPORT+1,1,1); //rejestr status

    while ((opt = getopt(argc, argv, "o:t:l:")) != -1) { //rozpoznawanie opcji linii komend
        switch(opt) 
        {
            case 'o':
                filename = optarg;
                break;
            case 't':
                endtime = atof(optarg);
                break;
            case 'l':
                maxloops = atoi(optarg);
                break;
            case '?':
                printf("Usage: lpr_timed -o outputfile -t time\n");
                break;
        }
    }

	FILE *ptr_datafile; //zapis do pliku

	ptr_datafile=fopen(filename,"wb");
	if (!ptr_datafile)
	{
		printf("Unable to open file! \n");
		return 1;
	}

    /*##############################
    początek czesci rejestrującej
    ##############################*/
    int r = 0;
    int r_old = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
	while (t_s <= endtime && loops < maxloops)
	{
        r = inb(BASEPORT+1); //odczyt wartości portu
        r = r & 0b01111000; //tylko interesujace bity 6:3
        if (r != r_old)
        {
            t_probe(r);
            fwrite(&rec_data, sizeof(struct data), 1, ptr_datafile);
            r_old = r;
        }
        loops++;	
    }

    r = inb(BASEPORT+1);
    r = r & 0b01111000; 
    t_probe(r);
    fwrite(&rec_data, sizeof(struct data), 1, ptr_datafile); //dodatkowa próbka zapisana na końcu

	fclose(ptr_datafile);

    printf("Loops: %d\n", loops);
    return 0;
}
