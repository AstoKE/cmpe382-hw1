#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(){
    int N;
    int max;
    int lastThree;
    int i;
    int num;
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

    printf("File1.txt and File2.txt created successfully.\n");

    return 0;
}

