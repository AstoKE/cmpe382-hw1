#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

int isPrime(int num) {
    int i;

    if(num<2)
        return 0;

    for(i=2; i<=num/2;i++) {
        if (num%i==0) {
            return 0;
        }
    }
    return 1;
}

int isAbundant(int num) {
    int i;
    int sum = 0;

    if (num<12) {
        return 0;
    }
        
    for (i =1; i<= num/2; i++) {
        if (num % i == 0){
            sum += i;
        }

    }

    if(sum > num) {
        return 1;
    }

    return 0;
}

int main(){
    int N;
    int max;
    int lastThree;
    int i;
    int num;

    int primeCount = 0;
    int abundantCount = 0;

    FILE *file1, *file2;

    printf("Enter N: ");
    scanf("%d", &N);

    lastThree=609;

    max = lastThree+100;

    srand(time(NULL));

    file1 =fopen("File1.txt", "w");
    file2 =fopen("File2.txt", "w");

    if(file1== NULL || file2==NULL) {

        printf("Error while opening files.\n");
        return 1;
    }

    for(i=0;i<N;i++){
        num = rand() % (max +1);
        fprintf(file1, "%d\n", num);
    }

    for (i = 0;i < N; i++) {
        num =rand() % (max +1);
        fprintf(file2, "%d\n", num);
    }

    fclose(file1);
    fclose(file2);

    file1 = fopen("File1.txt", "r");
    file2 = fopen("File2.txt", "r");

    if (file1 == NULL || file2==NULL) {
        printf("Error opening files or reading.");

        return 1;
    }

    while (fscanf(file1, "%d", &num)== 1) {
        if (isPrime(num)) {
            primeCount++;
        }
    }

    while(fscanf(file2, "%d", &num) == 1) {
        if (isAbundant(num)) {
            abundantCount++;
        }
    }


    fclose(file1);
    fclose(file2);

    printf("The number of positive integers in each file: %d\n", N);
    printf("The number of prime numbers in File1: %d\n", primeCount);
    printf("The number of abundant numbers in File2: %d\n", abundantCount);

    return 0;
}

