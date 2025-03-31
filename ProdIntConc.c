// Código de produto interno Concorrente
// Aluno - João Pedro Batista da Silva
// DRE - 121053174

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
typedef struct{
    int init;
    int n_threads;
    int size_vec;
    float *vec1,*vec2;
    int id_thread;
}dados;
typedef struct{
    int id_thread;
    double soma;
} t_ret;
void* prod_interno(void *args){
    dados *arg = (dados*) args;
    t_ret *ret = malloc(sizeof(t_ret));
    if(ret == NULL){
        printf("ERRO\nNao foi possivel criar struct de apoio\n");
        pthread_exit(NULL);
    }
    //printf("Estou na thread:%d-------\n",arg->id_thread);
    for(int i = arg->init; i< arg->size_vec; i+=arg->n_threads){
        //printf("vec1[%d]:%f\nvec2[%d]:%f\n",i, arg->vec1[i],i,arg->vec2[i]);
        ret->soma += arg->vec1[i] * arg->vec2[i];
        //printf("soma: %f\n",ret->soma);
    }
    free(arg);    
    pthread_exit((void*) ret);
}

int main(int argc, char *argv[]){
    float *vec1,*vec2;
    int n_threads, dim;
    FILE *arq;
    pthread_t *tid;
    size_t ret;
    double soma_final=0;
    t_ret *retorno;
    double valor_seq, var_rel;
    if(argc<3){
        printf("ERRO\nPor favor insira a entrada no formato %s<n_threads><nome_do_arquivo>\n",argv[0]);
        return 1;
    }
    n_threads = atoi(argv[1]);
    
    arq = fopen(argv[2],"rb");
    if(arq == NULL){
        printf("ERRO\nErro ao ler arquivo\n");
        return 2;
    }
    ret = fread(&dim, sizeof(int),1,arq);
    if(!ret){
        printf("ERRO ao ler a dimensao do vetor\n");
        return 3;
    }
    vec1 = (float *) malloc(sizeof(float)*dim);
    if(vec1 == NULL){
        printf("1-ERRO malloc\n");
        return 4;
    }
    
    ret = fread(vec1, sizeof(float),dim,arq);
    if(ret < dim) {
        printf("ERRO nao foi possivel fazer leitura dos elementos do vetor\n");
        return 5;
    }
    
    vec2 = (float *) malloc(sizeof(float)*dim);
    if(vec2 == NULL){
        printf("2-ERRO malloc\n");
        return 6;
    }
    ret = fread(vec2, sizeof(float),dim,arq);
    
    if(ret < dim) {
        printf("ERRO nao foi possivel fazer leitura dos elementos do vetor\n");
        return 7;
    }
    ret = fread(&valor_seq, sizeof(double), 1, arq);
    fclose(arq);
    if(!ret) {
        printf("ERRO nao foi possivel fazer leitura do valor do produto interno do arquivo sequencial\n");
        return 8;
    }
    if(dim<n_threads){n_threads=dim;}
    tid = (pthread_t*) malloc(sizeof(pthread_t)* n_threads);
    if(tid == NULL){
        printf("ERRO nao foi possivel alocar memoria pro tid\n");
        return 9;
    }
    
    for(int i = 0; i<n_threads;i++){
        dados *args = (dados *) malloc(sizeof(dados));
        args->init = i;
        args->n_threads = n_threads;
        args->size_vec = dim;
        args->vec1 = vec1;
        args->vec2 = vec2;
        args->id_thread = i;
        if(pthread_create(&tid[i], NULL, prod_interno, (void *) args)){
            printf("ERRO\nNao foi possivel criar thread %d.\n",i);
            return 9;
        };
    }
    
    for(int i =0 ;i<n_threads; i++){
        if(pthread_join(tid[i], (void**) &retorno)){
            printf("ERRO\nNao foi possivel fazer join com a thread%d",i);
        }
        soma_final += retorno->soma;
    }
    
    printf("soma final:%f\n",soma_final);
    arq = fopen(argv[2], "ab");
    if(!arq){
        printf("ERRO\nNao foi possivel abrir o arquivo para atualizar");
        return 10;
    }
    ret = fwrite(&soma_final, sizeof(double), 1, arq);
    fclose(arq);
    var_rel = abs((valor_seq-soma_final)/valor_seq);
    printf("variacao relativa:%f\n",var_rel);
    return 0;
}