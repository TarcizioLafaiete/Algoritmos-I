#include <iostream>
#include <string>
#include <vector>


//Struct que armazena os valores calculados durante o algoritmo da SSM
struct parametrosSSM{
    float soma;
    float sufixo;
    float prefixo;
    float ssm;
    int inicio;
    int fim;
};

//Funcao que realiza a analise da entrada de dados
void capturaAvalicao(int numAmigos,int numShows,std::vector<float>* avaliacao){
    
    float nota;
    //Cria um vetor com o tamanho correspondente ao número de shows + 1
    for(int l = 0;l < numShows+1;l++){
        //Coloca -6000 na posicao 0 para torna-la inutil e deixar os shows enumerados de 1 a n como no enunciado
        if(l == 0){
            avaliacao->push_back(-6000.0);
        }
        //Todos os shows inciados com nota zero
        else{
            avaliacao->push_back(0.0);
        }        
    }

    //Loop realiza o somatorio das notas dadas pelos amigos a cada show
    for(int i = 0;i < numAmigos;i++){
        for(int j = 1;j < numShows+1;j++){
            std::cin >> nota;
            avaliacao->at(j) += nota;
        }
    }

}


//Algoritmo de divisao e conquista para a SSM
parametrosSSM subvetorSomaMaxima(std::vector<float> avaliacao,int inicio,int fim){
    parametrosSSM esq,dir,solucao;
    //Caso do menor subproblema possivel
    if(inicio == fim){
        //Processo de conquista da solucao para o subproblema
        solucao.soma = avaliacao[inicio];
        solucao.ssm = std::max((float)0,avaliacao[inicio]);
        solucao.prefixo = solucao.ssm;
        solucao.sufixo = solucao.ssm;
        solucao.inicio = inicio;
        solucao.fim = fim;
        return solucao;
    }
    //Divisao do problema em dois subproblemas
    esq = subvetorSomaMaxima(avaliacao,inicio,(inicio + fim)/2);
    dir = subvetorSomaMaxima(avaliacao,((inicio+fim)/2)+1,fim);
    //Processo de combinacao da solucao dos subproblemas
    solucao.soma = esq.soma + dir.soma;
    solucao.ssm = std::max(std::max(esq.ssm,dir.ssm),esq.sufixo+dir.prefixo);
    solucao.sufixo = std::max(esq.sufixo+dir.soma,dir.sufixo);
    solucao.prefixo = std::max(esq.prefixo,esq.soma+dir.prefixo);
    //Verifica qual valor foi escolhido como a SSM anteriormente para definir o inicio e fim
    if(solucao.ssm == esq.ssm){
        solucao.inicio = esq.inicio;
        solucao.fim = esq.fim;
    }
    else if(solucao.ssm == dir.ssm){
        solucao.inicio = dir.inicio;
        solucao.fim = dir.fim;
    }
    else if(solucao.ssm == esq.sufixo+dir.prefixo){
        solucao.inicio = esq.inicio;
        solucao.fim = dir.fim;
    }
    return solucao;
}

int main(){

    //Declaracao de variaveis
    int numShows,numAmigos;
    std::vector<float> avaliacao;
    parametrosSSM resultado;
    bool loop = true;
    while(loop){
        std::cin>>numAmigos;
        std::cin>>numShows;
        //Condicao de parada do loop
        if(numAmigos == 0 && numShows == 0){
            loop = false;
            break;
        }
        //Leitura da entrada
        capturaAvalicao(numAmigos,numShows,&avaliacao);
        //Solucao da SSM
        resultado = subvetorSomaMaxima(avaliacao,0,numShows - 1);
        //Print da resposta
        std::cout<<resultado.inicio<<" "<<resultado.fim<<std::endl;
        //Limpa o vector para a proxima leitura
        avaliacao.clear();
    }

    return 0;
}