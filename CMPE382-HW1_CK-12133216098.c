
// libraries that i have implement
#include <stdlib.h>  // rand, srand, exit
#include <stdio.h>   // printf, scanf, fopen,...
#include <unistd.h>  // !!! fork, pipe, read, wriet, close
#include <time.h>    // time(null)
#include <sys/wait.h> //wait
#include <sys/types.h> // for the process tpyes


/*
Function that checks for "num" is prime or not

if num smaller than 2 its not prime
else if its can divide not prime
else its prime

*/
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


/*
Function that checks for num is abundant or not

abundant number: if sum of dividers of a num bigger than num its self

if num directly smalleer than 12 its not abundant
*/
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


/*
Main function that whole process are launch
*/
int main(){

    // parameters
    int N;
    int upperLimit;
    int studentIDTail;
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

    int receivedN;
    int primeTotal = 0;
    int abundantTotal = 0;


    FILE *primeFile;
    FILE *abundantFile;


    // take interger N from user
    printf("Enter N: ");
    scanf("%d", &N);

    studentIDTail=609;

    upperLimit = studentIDTail+100;

    // srand(time) for on every execution random num shoul different
    srand(time(NULL));

    
    primeFile =fopen("File1.txt", "w");
    abundantFile  =fopen("File2.txt", "w");

    
    for(i=0;i<N;i++){
        num = rand() % (upperLimit +1);
        fprintf(primeFile, "%d\n", num);
    }

    for (i = 0;i < N; i++) {
        num =rand() % (upperLimit +1);
        fprintf(abundantFile, "%d\n", num);
    }

    fclose(primeFile);
    fclose(abundantFile);

    /*
    There is 6 pipe that every one of them have 
    differen responsible on data transfer

    parenToPrime --> parent give N value to prime child
    parentToAbundant --> parent give N value to abundant child
    primeToParent --> prime child give prime result to parent
    abundantToParent --> abundant child give abundant result to parent
    parentToPrimeResult --> parent give other prime child's result
    parentToAbundantResult --> parent give other abundant child's result
    */
    if (pipe(parentToPrime) == -1 || pipe(parentToAbundant) == -1 ||
        pipe(primeToParent) == -1 || pipe(abundantToParent) == -1 ||
        pipe(parentToPrimeResult) == -1 || pipe(parentToAbundantResult)==-1) {
        printf("Pipe creation failed\n");
        return 1;
    }
    /* 
    this means we create two sided communication with
    separated two one sided pipes
    */




    // creating new  process with fork()
    // at this point we have 2 process: parent, prime child
    primeProcessID = fork();

    if (primeProcessID <0) {
        printf("First fork failed.\n");
        return 1;
    }

    // this block only works on child
    if(primeProcessID == 0) {
        //close unnecessary pipes
        close(parentToPrime[1]);
        close(parentToAbundant[0]);
        close(parentToAbundant[1]);
        close(primeToParent[0]);
        close(abundantToParent[0]);
        close(abundantToParent[1]);
        close(parentToPrimeResult[1]);
        
        //read N from parent
        read(parentToPrime[0], &receivedN, sizeof(int));
        
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

        //result sending to parent
        write(primeToParent[1], &primeTotal, sizeof(int));

        int otherCount;

        //parent waits for other results
        read(parentToPrimeResult[0], &otherCount, sizeof(int));
        
        //Writing winner
        if(primeTotal > otherCount) {
            printf("I am child process P2: The winner is child process P2\n");
        } 
        else {
            printf("I am child process P2: The winner is child process P3\n");
        }

        close(parentToPrime[0]);
        close(primeToParent[1]);

        //exits when process over
        exit(0);


    }
    // creating new  process with fork()
    // at this point we have 2 process: parent, abundant child

    abundantProcessID = fork();

    if (abundantProcessID < 0) {
        printf("Second fork failed.\n");
        return 1;
    }
    // this block only works on child
    if (abundantProcessID==0) {
        //close unnecessary pipes
        close(parentToAbundant[1]);
        close(parentToPrime[0]);
        close(parentToPrime[1]);
        close(abundantToParent[0]);
        close(primeToParent[0]);
        close(primeToParent[1]);
        close(parentToAbundantResult[1]);

        //read N from parent
        read(parentToAbundant[0],&receivedN, sizeof(int));

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

        //result sending to parent
        write(abundantToParent[1], &abundantTotal, sizeof(int));

        int otherResultComesFromParents;

        //parent waits for other results
        read(parentToAbundantResult[0], &otherResultComesFromParents, sizeof(int));

        //Writing winner 
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


    //in this write()s parnet sends N to childs
    write(parentToPrime[1], &N,sizeof(int));
    write(parentToAbundant[1],&N,sizeof(int));

    //in this read()s parent takes results from childs
    read(primeToParent[0], &primeCount,sizeof(int));
    read(abundantToParent[0], &abundantCount,sizeof(int));

    //Parent sends each child's result to other child so they can compare and determine the winner    
    write(parentToPrimeResult[1],&abundantCount,sizeof(int));
    write(parentToAbundantResult[1],&primeCount,sizeof(int));

    //close unclosed pipes
    close(parentToPrime[1 ]);
    close(parentToAbundant[1]);
    close(primeToParent[0]);
    close(abundantToParent[0]);
    close(parentToPrimeResult[0]);
    close(parentToAbundantResult[ 0]);
    close(parentToPrimeResult[1]);
    close(parentToAbundantResult[1]);

    //Parent wait for other two child to finish their process
    wait(NULL);
    wait(NULL);

    //Parent writes final report to output
    printf("The number of positive integers in each file: %d\n",N);
    printf("The number of prime numbers in File1: %d\n",primeCount);
    printf("The number of abundant numbers in File2: %d\n",abundantCount);

    return 0;
}
