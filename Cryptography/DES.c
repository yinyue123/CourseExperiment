#include <stdio.h>
#include <stdlib.h>
//#include <iostream.h>
//#include <stdlib.h>
#include <time.h>


static unsigned char ip[64]={//Initial permutation IP
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

static unsigned char ip1[64]={// initial replacement IP reverse IP-1
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

static unsigned char pc1[56]={// key replacement 1
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static unsigned char pc2[48]={// key replacement 2
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static unsigned char e[48]={// Extended replacement E
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static unsigned char lcircle[16]={// the number of rounds to be rounded
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static unsigned char s[8][4][16]={// S table
    // S1
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    // S2
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    // S3
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
    // S4
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
    // S5
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    // S6
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
    // S7
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
    // S8
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6,  11
};

static unsigned char P[32]={// replace P
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};

#define max 100
unsigned char firstkey[32];//password
unsigned char random_key[64];// Random sequence for CBC encryption
unsigned char r_key[64];  // random sequence
unsigned char temp_key[64];   // temporary key store
unsigned char mw[8]; // Random sequence for CBC encryption
unsigned char m[64]; // plaintext / ciphertext array
unsigned char charkey[8]; // char type key
unsigned char ejzkey[64];  // binary key
unsigned char ER[48];// 32 bits extended by 32 bits later
unsigned char dmw[8]={0}; // encoded plaintext / ciphertext
unsigned char lskey[56];  // temporary storage key
unsigned char childkey[16][56];  // 16 subkeys

FILE* fptr=NULL, *dfptr;
int size;
int mode_cbc;
unsigned char* L, *R;

void Thekey(unsigned char* firstkey, unsigned char* bkey);  // generate a 64-bit key based on the password
void encode();   // encryption
void decode();   // decryption
void getbit(unsigned char* m, unsigned char* mw);//// char to binary
void replace(unsigned char* a, unsigned char* b, int n);// replace function
void circle(unsigned char* key, int n); // left shift function
void yihuo(unsigned char* a, unsigned char* b, int n);// exclusive OR function
void S_box(unsigned char* a);// S box
void getchars(unsigned char* a, unsigned char* mw); //binary to char
int  add(unsigned char* a); //summation function
void getdkey(unsigned char* key);//produces 16 subkeys
void enrepeat();// encryption loop
void derepeat();// decrypt the loop
void getbmphead(FILE* fptr, FILE* dfptr);// read the BMP header
void randomkey(unsigned char* random_key);  // Generates a 64-bit random sequence



void Thekey(unsigned char* firstkey, unsigned char* bkey)// generate a key greater than 64 bits, less than completion
{
    int i, j=0;

    for(i=0;i<8;i++)
    {
        if(firstkey[j]=='\n')
        {
            j=0;
            i--;
        }
        else
        {
            bkey[i]=firstkey[j];
            j++;
        }
    }
    return ;
};

void encode() // encryption
{
    int i, n, j=0;

    while(ftell(fptr)<size)
    {

        fread(&mw[0], 1, 8, fptr);

        getbit(m, mw);   // convert plaintext to binary
        if(j==0)
        {
            for(i=0;i<64;i++)
                r_key[i]=random_key[i];
        }

        if(mode_cbc)//1.CBC 0.ECB
            yihuo(m, r_key, 64);
        replace(m, ip, n=64);// initial replacement of plaintext

        L=&m[0];// clear text is divided into left and right parts
        R=&m[32];

        enrepeat(); // enter the sixteenth iteration

        replace(L, ip1, 64);// reverse initial permutation
        for(i=0;i<64;i++)
            r_key[i]=L[i];
        getchars(L, dmw);   // convert the binary ciphertext into char

        fwrite(&dmw[0], 1, 8, dfptr);// Write the ciphertext to the file
        j++;
    }

    return;
}  ;

void decode()  // decryption
{
    int i, n, s=0, j=0;


    while((s=ftell(fptr))<size)
    {
        fread(&mw[0], 1, 8, fptr);

        getbit(m, mw); // convert ciphertext to binary

        L=&m[0];  // ciphertext is divided into left and right parts
        R=&m[32];
        if(j==0)
        {
            for(i=0;i<64;i++)
                r_key[i]=random_key[i];
        }
        for(i=0;i<64;i++)
            temp_key[i]=L[i];
        replace(m, ip, n=64); // the initial replacement of ciphertext

        derepeat();

        replace(L, ip1, 64);  // reverse initial permutation
        if(mode_cbc)
            yihuo(L, r_key, 64);
        for(i=0;i<64;i++)
            r_key[i]=temp_key[i];
        getchars(L, dmw);  // convert binary plaintext into char

        fwrite(&dmw[0], 1, 8, dfptr); // Write the plaintext data in the array to the file
        j++;
    }

    return;
};
void enrepeat()
{
    int i, n, q;

    for(q=0;q<16;q++)//Enter 16 iterations
    {
        for(i=0;i<56;i++)
            lskey[i]=childkey[q][i];

        replace(lskey, pc2, n=48);// The key is compressed to 48 bits

        for(i=0;i<32;i++)
            ER[i]=R[i];

        replace(ER, e, n=48);// expand the right half to 48 bits



        yihuo(ER, lskey, n=48);  // The right half is XORed with the key (48 bits)

        S_box(ER);// enter the S-box operation
        replace(ER, P, n=32); // P replace the right half


        yihuo(ER, L, n=32); // left and right XOR
        for(i=0;i<32;i++) // swap around
        {
            L[i]=R[i];
            R[i]=ER[i];
        }
        if(q==15) // 16 times and then exchange again
        {
            for(i=0;i<32;i++)
            {
                ER[i]=R[i];
                R[i]=L[i];
                L[i]=ER[i];
            }
        }
    }

}  ;


void derepeat()
{
    int i, n, q, j=0;

    for(q=0;q<16;q++) // Enter the 16 rounds of iterations
    {
        for(i=0;i<56;i++)
            lskey[i]=childkey[15-q][i];

        replace(lskey, pc2, n=48); //The key is compressed to 48 bits


        for(i=0;i<32;i++)
            ER[i]=R[i];

        replace(ER, e, n=48); // expand the right half to 48 bits



        yihuo(ER, lskey, n=48);  // The right half is XORed with the key (48 bits)

        S_box(ER);//enter the S-box operation
        replace(ER, P, n=32); //P replace the right half

        yihuo(ER, L, n=32); //left and right XOR
        for(i=0;i<32;i++) //swap around
        {
            L[i]=R[i];
            R[i]=ER[i];
        }
        if(q==15) //16 times and then exchange again
        {
            for(i=0;i<32;i++)
            {
                ER[i]=R[i];
                R[i]=L[i];
                L[i]=ER[i];
            }
        }
    }

}  ;


void getdkey(unsigned char* key)
{
    int i, j;
    for(i=0;i<16;i++)
    {
        circle(key, i);
        for(j=0;j<56;j++)
        {
            childkey[i][j]=key[j];  //get 16 subkeys
        }
    }
    return;
}  ;

void getbit(unsigned char* m, unsigned char* mw)
{
    int i, j;
    unsigned char and[8]={128, 64, 32, 16, 8, 4, 2, 1};
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            if((mw[i]&and[j])==0)//with the method to remove the corresponding binary
                m[i*8+j]=0;
            else
                m[i*8+j]=1;
        }
    }
    return;
} ;

void replace(unsigned char* a, unsigned char* b, int n)

{
    int i;
    unsigned char c[max];//replace
    for(i=0;i<n;i++)
        c[i]=a[b[i]-1];
    for(i=0;i<n;i++)
        a[i]=c[i];
    return;
}  ;

void circle(unsigned char* key, int n)
{
    int i, j, k;

    switch(lcircle[n])// key left loop: the first 28 bits to the left and the last 28
    {
        case 1:
            j=key[0];
            for(i=0;i<27;i++)
                key[i]=key[i+1];
            key[27]=j;

            j=key[28];
            for(i=28;i<55;i++)
                key[i]=key[i+1];
            key[55]=j;
            break;
        case 2:
            j=key[0];
            k=key[1];
            for(i=0;i<26;i++)
                key[i]=key[i+2];
            key[26]=j;
            key[27]=k;

            j=key[28];
            k=key[29];
            for(i=28;i<54;i++)
                key[i]=key[i+2];
            key[54]=j;
            key[55]=k;
            break;
    }

    return;
} ;

void yihuo(unsigned char* a, unsigned char* b, int n)
{
    int i;
    for(i=0;i<n;i++)
        a[i]=a[i]^b[i];
    return;
}  ;

void S_box(unsigned char* a)
{
    int i, j;
    unsigned char b[8][6];
    unsigned char c[8];
    for(i=0;i<8;i++) //divide the 48 bits into 8 groups of six
    {
        for(j=0;j<6;j++)
            b[i][j]=a[i*6+j];
    }
    for(i=0;i<8;i++)
    {
        c[i]=s[i] [ b[i][0]*2+b[i][5] ] [ b[i][1]*8+b[i][2]*4+b[i][3]*2+b[i][4] ];//find the corresponding decimal number in table S
    }
    for(i=0;i<8;i++)//convert decimal to binary
    {
        for(j=3;j>=0;j--)
        {
            a[i*4+j]=c[i]%2;
            c[i]/=2;
        }
    }
    return;
};

void getchars(unsigned char* a, unsigned char* dmw)
{
    int i;
    for(i=0;i<8;i++)
        dmw[i]=add(&a[i*8]);//binary conversion to decimal char
    return;
}  ;

int add(unsigned char* a)
{
    return a[0]*128+a[1]*64+a[2]*32+a[3]*16+a[4]*8+a[5]*4+a[6]*2+a[7];
} ;

void getbmphead(FILE* fptr, FILE* dfptr)
{
    int i;
    char c;

    for(i=0;i<55;i++)
    {
        c=fgetc(fptr);
        fputc(c, dfptr);
    }
    return ;
}  ;

void randomkey(unsigned char* random_key)
{
    int i;

    srand(time(0));
    for(i=0;i<64;i++)
        random_key[i]=rand()%2;
    return;
} ;

int main()
{
    int i, n;
    char filename[256]={'p','a','s','s','w','d','.','b','m','p',0}, c;
    int function,ret;

    printf("Please choose the function:\n");
    printf("1.picture encrypt\t2.login\n");
    scanf("%d",&function);
    getchar();
    function--;
    if(function)
    {
        printf("1.Login\t2.Set password\n");
        ret=scanf("%d",&function);
        //if(ret!=2)fflush(stdin);
        getchar();
    }else
    {
        for(i=0;i<256;i++)
            filename[i]=0;
        printf("please type the picture name that you want to encrypt:\n");
        gets(filename);

    }
    fptr=fopen(filename, "rb");
    if(fptr==NULL)
    {
        printf("Cant open the file\n");
        return 0;
    }
    fseek(fptr,0,SEEK_END);
    size=ftell(fptr);
    rewind(fptr);

    randomkey(random_key);


    printf("please type the password\n");
    i=0;
    firstkey[i]=getchar();
    while(firstkey[i]!='\n')
    {
        i++;
        firstkey[i]=getchar();
    }
    if(firstkey[0]=='\n')
    {
        printf("illegal password");
        return 0;
    }
    Thekey(firstkey, charkey);

    getbit(ejzkey, charkey);//convert the key to binary

    replace(ejzkey, pc1, n=56);  //key is replaced by 56 bits

    getdkey(ejzkey); // get 16 subkeys
    if(function==0){
        printf("Choose the encrypt mode:\t0.EBC\t1.CBC\n");
        scanf("%d",&mode_cbc);
    }
    else
        mode_cbc=1;
    if(function==0 || function==2)
    {
        
        if(mode_cbc!=0 && mode_cbc!=1)
        {
            printf("illegal choice\n");
        }
        printf("encrypting...\n");
        dfptr=fopen("encipher.bmp", "wb+");
        getbmphead(fptr, dfptr);  // read the file header
        encode(); //encryption
        printf("Finish.The new file is encipher.bmp\n");
        fclose(fptr);
        fclose(dfptr);
    }
    if(function==0 || function==1)
    {
        printf("decryteing...\n");
        fptr=fopen("encipher.bmp", "rb");
        dfptr=fopen("cipher.bmp", "wb+");
        if(fptr==NULL || dfptr==NULL){
            printf("Fail to open the file\n");
        }
        getbmphead(fptr, dfptr);
        decode();  //decrypt
        printf("Finish.The new file is cipher.bmp\n");
        //fclose(fptr);

                printf("Login successfully\n");
        if(function==1){
            fptr=fopen("password","rb");
            dfptr=fopen("cipher.bmp","rb");
            char cmp,cmp2;
            do{
                printf("123\n");
                cmp=fgetc(fptr);
                cmp2=fgetc(dfptr);
                printf("\n1");
                if(cmp!=cmp2){
                    printf("Fail to login,please check the password and try again\n");
                    break;

                }
            }while(cmp!=EOF||cmp2!=EOF);
            if(cmp==cmp2)
                printf("Login successfully\n");
            fclose(fptr);
            fclose(dfptr);

        }
    }





    return 0;
}

