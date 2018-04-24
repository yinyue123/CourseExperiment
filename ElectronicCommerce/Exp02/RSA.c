#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long ull;

ull gcd(ull a,ull b){
    int t;
    while(b){
        t=a;
        a=b;
        b=t%b;
    }
    return a;
}

void generateKey(ull p,ull q,ull* e,ull* d){

    //generatePubKey
    int r;
    (*e)=2;
    r=(p-1)*(q-1);

    (*e)=65537;
    while((*e)<1||(*e)>r||gcd((*e),r)!=1){
        (*e)++;
        if((*e)==-1){
            printf("error,can't find e,overflow\n");
            return;
        }
    }
    printf("e:%llu",*e);

    //generateSecKey
    *d=1;
    while((*e)*(*d)%r!=1){
        *d=rand()%r;
        //(*d)++;
        //printf("d:%llu",*d);
        if((*d)==-1){
            printf("error,can't find b,overflow\n");
            return;
        }
    }
    printf("d:%llu",*d);
}
/*
ull MulMod(ull a,ull b,ull mod){
    int num1[64],num2[64],res[128];
    int opr1,opr2,i;
    ull o1,o2;
    for(opr1=0,opr2=0;opr1<64;opr1++){
        num1[opr1]=0;
        num2[opr1]=0;
        res[opr2++]=0;
        res[opr2+0
    o1=1;o2=1;
    for(opr1=0;opr1<64;opr1++){
        num1[opr1]=(a&o1)?1:0;
        o1<<=1;
    }
    for(opr2=0;opr2<64;opr2++){
        num2[opr2]=(a&o2)?1:0;
        o2<<=1;
    }
    for(opr1=0;opr1<64;opr1++){
        for(opr2=0;opr2<64;opr2++){
            if(num1[opr1]&num2[opr2])
                res[opr1+opr2-1]++;
        }
        for(i=0;i<128;i++){
            if(res[i]>1){
                res[i]=0;res[i+1]++;
            }
        }
    }

    o2=1;
    for(i=0;i<64;i++){
        if(res[i])
            o1+=o2;
        o2<<=1;
    }
    return o1;
}
*/
/*
ull powAndMod(ull x,ull m,ull mod){
    ull y;
    y=1;
    while(m!=0){
        if(m%2==1){
            y=(x*y)%mod;
            m--;
        }else{
            x=(x*x)%mod;
            m/=2;
        }
    }
    return y;
}
*/
ull quickMod(ull a,ull b,ull mod){
    //a^10%c=a^(1*2^3+0*2^2+1*2^1+0*2^0)%c=((a^(2^3)%c)*(a^(2^1)%c))%c
    ull aus=1;
    while(b){
        if(b&0x1)
            aus=aus*a%mod;
        a=a*a%mod;
        b>>=1;
    }
    return aus;
}

int fermatPrime(ull num){
    //a^(n-1)=1(mod n)
    int i;
    for(i=0;i<5;i++)
        if(quickMod(1+rand()%(num-1),num-1,num)!=1)
            break;
    if(i==5)return 1;
    return 0;
}

void generateBigPrime(ull* p,ull* q){
    int i;
    ull temp;
    srand((unsigned)time(NULL));
    do{
        *p=0;
        for(i=0;i<1;i++){
            temp=rand()%20000;
            temp<<=16*i;
            *p|=temp;
        }
//    printf("p:%llu\tq:%llu\n",*p,*q);
    }while(!fermatPrime(*p));

    do{
        *q=0;
        for(i=0;i<1;i++){
            temp=rand()%20000;
            temp<<=16*i;
            *q|=temp;
        }
    }while(p==q||!fermatPrime(*q));
    printf("p:%llu\tq:%llu\n",*p,*q);
}

void enc(char plaintext[],ull ciphertext[],ull n,ull e){
    int i;
    for(i=0;plaintext[i]!=0;i++){
        ciphertext[i]=quickMod((ull)plaintext[i],e,n);
        printf("%c",plaintext[i]);
    }
    ciphertext[i]=0;
}
void dec(ull ciphertext[],char plaintext[],ull n,ull d){
    int i;
    for(i=0;ciphertext[i]!=0;i++)
        plaintext[i]=(char)quickMod(ciphertext[i],d,n);
    ciphertext[i]=0;
}
void printfx(ull ciphertext[]){
    int i;
    for(i=0;ciphertext[i]!=0;i++){
        printf("%llu\t",ciphertext[i]);
    }
    printf("\n");
}

int main(){
    /* prime p q
     * n=p*q r=(p-1)(q-1)
     * e<r gcd(e,r)=1 (e*d)mod r=1
     * pub key (n,e) sec key (n,d)
     * plaintext A ciphertext B
     * A=B^e(mod n) B=A^d(mod n)
     * e d can be exchanged
     * */
    ull p=0,q=0;
    ull e,d;
    /*
    p=1725580387;
    q=1816490293;
    e=65537;
    d=5685353;
    */
    char text[1000];
    ull textx[1000];
    generateBigPrime(&p,&q);
    generateKey(p,q,&e,&d);
    //printf("p:%llu\tq:%llu\te:%llu\td:%llu\n",p,q,e,d);

    printf("pub key:(%llu,%llu)\nsec key:(%llu,%llu)\n",p*q,e,p*q,d);

    printf("please input the text you want to encrypt:");
    gets(text);
    enc(text,textx,p*q,e);
    printf("After the encrypt,the ciphertext is following\n");
    printfx(textx);
    dec(textx,text,p*q,d);
    printf("After the decrypt,the plaintext is following\n");
    printf("%s\n",text);
    return 0;
}
