#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

unsigned long long used_long(char *, u_int8_t *, u_int8_t *, u_int8_t *, u_int8_t *);
unsigned long long compress(char *, u_int8_t *, u_int8_t *, u_int8_t *, u_int8_t *);
unsigned long long decompress(char *, u_int8_t *, u_int8_t *, u_int8_t *);
//#define DEBUG_ARRAY 1

#define arraysize 4294967296ULL
#define array2size 4294967296ULL // 3*16777216ULL

int main(int argc, char *argv[])
{
    unsigned long long used=0;
	u_int8_t *array;
	u_int8_t *array2;
	u_int8_t *array3;
	u_int8_t *array4;
	int d=0;
	int loop=1;
	char infile[32765];
	strcpy(infile,argv[1]);

	if ( argc > 2 )
		loop=strtol(argv[2],NULL,10);

	for(d=0; d< loop; d++)
	{
		used=0; 
		array2=calloc(array2size,1);
		array3=calloc(array2size,1);
		array4=calloc(array2size,1);
		
		#ifdef DECOMPRESSOR
			decompress(infile,array2,array3,array4);
			infile[strlen(infile)-5]='\0';
		#else
			compress(infile,array,array2,array3,array4);
			strcat(infile,".comp");
		#endif

		free(array2);
		free(array3);
		free(array4);

		array2=NULL;
		array3=NULL;
		array4=NULL;
	}	

    return(0);
}

unsigned long long compress(char *infile, u_int8_t *array, u_int8_t *array2, u_int8_t *array3, u_int8_t *array4)
{
	FILE *input;
	FILE *table;
	FILE *output;
	u_int32_t inbyte=0;
	unsigned long long d=0;
    u_int32_t count=0;
	unsigned char outfile[32768];
	strcpy(outfile,infile);
	strcat(outfile,".comp");
	unsigned char tablefile[32768];
	strcpy(tablefile,infile);
	strcat(tablefile,".table");
    u_int64_t position=0;
	u_int8_t numbers[3];
	int32_t temp;
    u_int64_t fs=0;

	#ifdef INFO
		printf("Using input file : %s\nUsing table : %s\nUsing output : %s\n",infile,tablefile,outfile);
	#endif

	if ( ( input=fopen(infile,"rb") ) == NULL ) exit(10);
	if ( ( table=fopen(tablefile,"w+b") ) == NULL ) exit(20);
	if ( ( output=fopen(outfile,"wb") ) == NULL ) exit(30);

    array=calloc(arraysize,1);

    fseek(input,0L,SEEK_END);
    fs=ftell(input);
    fseek(input,0L,SEEK_SET);

	while ( position < fs )
	{
		inbyte=0;
		temp=0;
		fread(&inbyte,4,1,input);

		if ( *(array+inbyte) == 0)
		{
				fwrite(&inbyte,4,1,table);
				#ifdef INFO
					printf("%lld %lld\n",count, inbyte);			
				#endif
				*(array+inbyte)=1;
				temp=count;
				*(array2+inbyte)=temp%256;
				temp >>= 8;
				*(array3+inbyte)=temp%256;
				temp >>= 8;
				*(array4+inbyte)=temp%256;
				count++;
		}

		if ( count > 16777216 )
		{
			fclose(input);
			fclose(table);
			fclose(output);
			free(array);
			free(array2);
			free(array3);
			free(array4);
			exit(23);
		}

		numbers[0] = *(array2+inbyte);
		numbers[1] = *(array3+inbyte);
		numbers[2] = *(array4+inbyte);
		temp=numbers[2];
		temp <<= 8;
		temp=temp+numbers[1];
		temp <<= 8;
		temp=temp+numbers[0];

		fwrite(&numbers,1,3,output);

		position=position+4;
	}

    fclose(input);
	fclose(table);
	fclose(output);

	return(0);
}

unsigned long long decompress(char *infile, u_int8_t *array2, u_int8_t *array3, u_int8_t *array4)
{
	FILE *input;
	FILE *table;
	FILE *output;
	u_int32_t inbyte=0;
	unsigned long long d=0;
    unsigned long long count=0;
	unsigned char outfile[32768];
	strcpy(outfile,infile);
	outfile[strlen(outfile)-5]='\0';
	strcat(outfile,".decomp");
	unsigned char tablefile[32768];
	strcpy(tablefile,infile);
	tablefile[strlen(tablefile)-5]='\0';
	strcat(tablefile,".table");
	unsigned long filesize=0;
	unsigned long position=0;
	unsigned long password=0;
	u_int8_t numbers[3];
	int32_t temp;

	if ( ( input=fopen(infile,"rb") ) == NULL ) exit(10);
	if ( ( table=fopen(tablefile,"rb") ) == NULL ) exit(20);
	if ( ( output=fopen(outfile,"wb") ) == NULL ) exit(30);
	fseek(input,0L,SEEK_END);
	filesize=ftell(input);
	fseek(input,0L,SEEK_SET);

	fseek(table,0L,SEEK_END);
	password=ftell(table);
	fseek(table,0L,SEEK_SET);
	password=password/4;

	for(d=0; d<password; d++ )
	{
		inbyte=0;
		fread(&inbyte,4,1,table);
		*(array2+d)=(inbyte%16777216ULL);
	}

	while ( position < filesize )
	{
		inbyte=0;
		count=0;
		temp=0;

		fread(&numbers,1,3,input);
	
		temp = numbers[2]%256;
		temp <<= 8;
		temp = temp + numbers[1]%256;
		temp <<= 8;
		temp = temp + numbers[0]%256;
		
		count=temp*4;

		fseek(table,0L,SEEK_SET);
		fseek(table,count,SEEK_SET);
		fread(&inbyte,4,1,table);

		#ifdef INFO
			printf("%ld %llu %u %u %u\n",temp,inbyte, numbers[0], numbers[1], numbers[2]);
		#endif
		
		fwrite(&inbyte,4,1,output);
		position=position+3;
	}

    fclose(input);
	fclose(output);
	fclose(table);

	return(0);

}
