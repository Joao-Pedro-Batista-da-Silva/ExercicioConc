#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    float *vec1,*vec2;
    int n_vec;
    FILE *output_file;
    float num1,num2;
    size_t ret;
    double prod_int = 0;
    
    if(argc < 3){
        printf("ERRO \nPor favor digite %s <dimensao><nome do arquivo de saida>\n", argv[0]);
        return 1;
    }
    n_vec = atoi(argv[1]);
    vec1 = (float *) malloc(sizeof(float) *n_vec);
    vec2 = (float *) malloc(sizeof(float) *n_vec);
    if(!vec1 || !vec2){
        printf("ERRO \nNao foi possivel criar vetor");
        return 2;
    }
    //printf("ola mundo\n");
    
    srand(time(NULL));
    for(int i =0; i< n_vec; i++){
        num1 = (rand()%10)/5.0;
        vec1[i] = num1;
        num2 = (rand()%10)/2.0;
        vec2[i] = num2;
        prod_int += vec1[i]*vec2[i];
    }
    output_file = fopen(argv[2],"wb");
    if(!output_file){
        printf("ERRO \nNao foi possivel abrir arquivo para escrever");
        return 3;
    }
    printf("dimensao:%d\n",n_vec);
    ret = fwrite(&n_vec, sizeof(int), 1, output_file);
    printf("vector1-----\n");
    for(int i = 0; i<n_vec;i++){
        printf("%f ",vec1[i]);
    }
    ret = fwrite(vec1, sizeof(float), n_vec, output_file);
    if(ret < n_vec) {
        printf("ERRO \nNao foi possivel escrever no  arquivo\n");
        return 4;
    }
    printf("\nvector2-----\n");
    for(int i = 0; i<n_vec;i++){
        printf("%f ", vec2[i]);
    }
    ret = fwrite(vec2, sizeof(float), n_vec, output_file);
    if(ret < n_vec) {
        printf("ERRO \nNao foi possivel escrever no  arquivo\n");
        return 4;
   }
   printf("\n%lf\n",prod_int);
    ret = fwrite(&prod_int, sizeof(double),1,output_file);
    
    fclose(output_file);
    free(vec1);
    free(vec2);
    return 0;
}