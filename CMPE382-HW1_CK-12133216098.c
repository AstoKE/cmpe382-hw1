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

    int parentToPrime[2];
    int parentToAbundant[2];

    int primeToParent[2];
    int abundantToParent[2];

    int parentToPrimeResult[2];
    int parentToAbundantResult[2];

    pid_t primeProcessID, abundantProcessID;

    int receivedSize;
    int primeTotal = 0;
    int abundantTotal = 0;


    FILE *primeFile;
    FILE *abundantFile;

    printf("Enter N: ");
    scanf("%d", &N);

    lastThree=609;

    max = lastThree+100;

    srand(time(NULL));

    primeFile =fopen("File1.txt", "w");
    abundantFile  =fopen("File2.txt", "w");

    if(primeFile== NULL || abundantFile==NULL) {

        printf("Error while opening files.\n");
        return 1;
    }

    for(i=0;i<N;i++){
        num = rand() % (max +1);
        fprintf(primeFile, "%d\n", num);
    }

    for (i = 0;i < N; i++) {
        num =rand() % (max +1);
        fprintf(abundantFile, "%d\n", num);
    }

    fclose(primeFile);
    fclose(abundantFile);

    if (pipe(parentToPrime) == -1 || pipe(parentToAbundant) == -1 ||
        pipe(primeToParent) == -1 || pipe(abundantToParent) == -1 ||
        pipe(parentToPrimeResult) == -1 || pipe(parentToAbundantResult)==-1) {
        printf("Pipe creation failed\n");
        return 1;
    }


    primeProcessID = fork();

    if (primeProcessID <0) {
        printf("First fork failed.\n");
        return 1;
    }

    if(primeProcessID == 0) {
        close(parentToPrime[1]);
        close(parentToAbundant[0]);
        close(parentToAbundant[1]);
        close(primeToParent[0]);
        close(abundantToParent[0]);
        close(abundantToParent[1]);

        read(parentToPrime[0], &receivedSize, sizeof(int));

        primeFile = fopen("File1.txt", "r");
        if (primeFile == NULL) {
            printf("Prime child could not open File1.txt\n");
            exit(1);
        }

        while(fscanf(primeFile, "%d", &num) == 1) {
            if (isPrime(num)) {
                primeTotal++;
            }
        }

        fclose(primeFile);

        write(primeToParent[1], &primeTotal, sizeof(int));

        int otherResultComesFromParents;

        read(parentToPrimeResult[0], &otherResultComesFromParents, sizeof(int));

        if(primeTotal > otherResultComesFromParents) {
            printf("I am child process P2: The winner is child process P2\n");
        } else {
            printf("I am child process p2: The winner is child process P3\n");
        }

        close(parentToPrime[0]);
        close(primeToParent[1]);

        exit(0);


    }

    abundantProcessID =fork();

    if (abundantProcessID < 0) {
        printf("Second fork failed.\n");
        return 1;
    }

    if (abundantProcessID==0) {
        close(parentToAbundant[1]);
        close(parentToPrime[0]);
        close(parentToPrime[1]);
        close(abundantToParent[0]);
        close(primeToParent[0]);
        close(primeToParent[1]);

        read(parentToAbundant[0],&receivedSize, sizeof(int));

        abundantFile = fopen("File2.txt", "r");
        if (abundantFile == NULL) {
            printf("Abundant child could not open File2.txt\n");
            exit(1);
        }

        while (fscanf(abundantFile,"%d", &num) == 1) {
            if (isAbundant(num)) {

                abundantTotal++;
            }
        }

        fclose(abundantFile);

        write(abundantToParent[1], &abundantTotal, sizeof(int));

        int otherResultComesFromParents;

        read(parentToAbundantResult[0], &otherResultComesFromParents, sizeof(int));

        if(abundantTotal > otherResultComesFromParents) {
            printf("I am child process P3: The winner is child process P3\n");
        } else {
            printf("I am child process P3: The winner is child process P2\n");
        }

        close(parentToAbundant[0]);
        close(abundantToParent[1]);

        exit(0);
    }

    close(parentToPrime[0]);
    close(parentToAbundant[0]);
    close(primeToParent[1]);
    close(abundantToParent[1]);

    write(parentToPrime[1], &N, sizeof(int));
    write(parentToAbundant[1], &N, sizeof(int));

    read(primeToParent[0], &primeCount, sizeof(int));
    read(abundantToParent[0], &abundantCount, sizeof(int));

    write(parentToPrimeResult[1], &abundantCount, sizeof(int));
    write(parentToAbundantResult[1], &primeCount, sizeof(int));

    close(parentToPrime[1]);
    close(parentToAbundant[1]);
    close(primeToParent[0]);
    close(abundantToParent[0]);
    close(parentToPrimeResult[0]);
    close(parentToAbundantResult[0]);
    close(parentToPrimeResult[1]);
    close(parentToAbundantResult[1]);

    wait(NULL);
    wait(NULL);

    printf("The number of positive integers in each file: %d\n", N);
    printf("The number of prime numbers in File1: %d\n", primeCount);
    printf("The number of abundant numbers in File2: %d\n", abundantCount);

}

