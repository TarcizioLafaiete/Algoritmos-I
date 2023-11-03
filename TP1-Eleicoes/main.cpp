#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <map>

enum OrdemGrafo {NORMAL = 0,INVERTIDO = 1};

//Realiza a conversao de um determinado vertice para uma posicao no vetor
int verticeParaPosicao(int vertice,int tamanho){

    if(vertice < 0){
        return (vertice/-1) - 1;
    }
    else{
        return vertice + (tamanho/2 - 1);
    }

}
//Realiza a conversao de uma determinada posicao do vetor para uma vertice
int posicaoParaVertice(int posicao,int tamanho){
    
    if(posicao < tamanho/2){
        return (posicao + 1)*-1;
    }
    else{
        return posicao - (tamanho/2 - 1);
    }
}

//Monta um vetor com as opinoes de cada seguidor do deputado Alan Turing
void montaOpiniaoSeguidor(int auxiliar[4], std::vector<int>* propCandidato){

    int p1;
    int inicioLoop = 0;
    int fimLoop = 4;
    //Condicionais para caso de haver um zero ou dois nas propostas para serem mantidas
    if(auxiliar[0] == 0 && auxiliar[1] != 0){
        auxiliar[0] = auxiliar[1];
    }
    else if(auxiliar[0] != 0 && auxiliar[1] == 0){
        auxiliar[1] = auxiliar[0];
    }
    else if(auxiliar[0] == 0 && auxiliar[1] == 0){
        //Loop inicia em 2 para montar a opinao do seguidor comecara em dois ja que as posicoes 0 e 1 do vetor sao zero
        // e devem ser desconsideradas
        inicioLoop = 2;
    }

    //Condicionais para se avaliar caso haja um ou dois zeros nas propostas de retirada
    if(auxiliar[2] == 0 && auxiliar[3] != 0){
        auxiliar[2] = auxiliar[3];
    }
    else if(auxiliar[2] != 0 && auxiliar[3] == 0){
        auxiliar[3] = auxiliar[2];
    }
    else if(auxiliar[2] == 0 && auxiliar[3] == 0){
        //Loop finaliza em 2 pois as posicoed 2 e 3 do vetor sao zero e devem ser desconsideradas
        fimLoop = 2;
    }

    for(int i = inicioLoop;i < fimLoop;i++){
        p1 = auxiliar[i];
        if(i < 2){
            p1 *= 1;
        }
        else{
            p1 *= -1;
        }
        propCandidato->push_back(p1);
    }
    
}

//Funcao responsavel por montar os grafos de implicacao e invertido
void montaGrafo(std::vector<int> propCandidato,std::map<int,std::list<int>> *grafoProposta,int modo){

    std::vector<int>::iterator it;
    std::map<int,std::list<int>>::iterator itMap;
    std::list<int> l1;
    int p1,p2,p1neg,p2neg;
    int p3,p4,p5,p6;
    // A iteracao pelo vetor de propostas e o mesmo para os dois grafos
    for(it = propCandidato.begin();it != propCandidato.end();++it){
        p1 = *it;
        ++it;
        p2 = *it;
        p1neg = p1;
        p1neg *= -1;
        p2neg = p2;
        p2neg *= -1;
        //Devido aos modos de gerar um grafo normal e um invertido se torna necessario decidir quem ira apontar para quem
        if(modo == OrdemGrafo::NORMAL){
            p3 = p1;
            p4 = p1neg;
            p5 = p2;
            p6 = p2neg;
        }
        else if(modo == OrdemGrafo::INVERTIDO){
            p3 = p2neg;
            p4 = p2;
            p5 = p1neg;
            p6 = p1;
        }
        //Caso um vertice nao tenha nenhum vertice adjacente ele nao se encontra no map
        itMap = grafoProposta->find(p4);
        if(itMap != grafoProposta->end()){
            grafoProposta->find(p4)->second.push_back(p5);
        }
        else{
            l1.push_back(p5);
            grafoProposta->insert(grafoProposta->begin(),std::pair<int,std::list<int>>(p4,l1));
            l1.clear();
        }
        itMap = grafoProposta->find(p6);
        if(itMap != grafoProposta->end()){
            grafoProposta->find(p6)->second.push_back(p3);
        }
        else{
            l1.push_back(p3);
            grafoProposta->insert(grafoProposta->begin(),std::pair<int,std::list<int>>(p6,l1));
            l1.clear();
        }

    }
}

//DFS Primeira e usada para montar a pilha com os vertices
//Seu funcionamente e semelhante a algoritmo de DFS visto em sala com a diferenca da montagem de pilha
void DFSPrimeira(int posicao,std::stack<int>* pilhaVertice,std::vector<bool>* visitados,std::map<int,std::list<int>> grafoProposta){
    std::list<int> vizinhos;
    std::list<int>::iterator it;
    std::map<int,std::list<int>>::iterator itMap;
    int vertice;
    int posicaoAtual;
    if(visitados->at(posicao) == true){
        return;
    }
    visitados->at(posicao) = true;
    itMap = grafoProposta.find(posicaoParaVertice(posicao,visitados->size()));
    if(itMap != grafoProposta.end()){
        vizinhos = grafoProposta.at(posicaoParaVertice(posicao,visitados->size()));
        for(it = vizinhos.begin();it != vizinhos.end();++it){
            vertice = *it;
            posicaoAtual = verticeParaPosicao(vertice,visitados->size());
            if(visitados->at(posicaoAtual) == 0){
                DFSPrimeira(posicaoAtual,pilhaVertice,visitados,grafoProposta);
            }
        }
    }
    pilhaVertice->push(posicaoParaVertice(posicao,visitados->size()));

}


// DFS Segunda e usada para definir a componente fortemente conexa de cada vertice
// Tirando a atribuicao ao vetor das CFC's e o mesmo algoritmo de DFS visto em sala
void DFSSegunda(int posicao,int contador,std::vector<bool>* visitaInverida,std::vector<int>* cfc,std::map<int,std::list<int>> grafoInvertido){

    std::list<int> vizinhos;
    std::list<int>::iterator it;
    std::map<int,std::list<int>>::iterator itMap;
    int vertice;
    int posicaoAtual;
    if(visitaInverida->at(posicao) == true){
        return;
    }
    visitaInverida->at(posicao) = true;
    itMap = grafoInvertido.find(posicaoParaVertice(posicao,visitaInverida->size()));
    if(itMap != grafoInvertido.end()){
    vizinhos = grafoInvertido.at(posicaoParaVertice(posicao,visitaInverida->size()));
    for(it = vizinhos.begin();it != vizinhos.end();++it){
            vertice = *it;
            posicaoAtual =  verticeParaPosicao(vertice,visitaInverida->size());
            if(visitaInverida->at(posicaoAtual) == 0){
                DFSSegunda(posicaoAtual,contador,visitaInverida,cfc,grafoInvertido);
            }
        }
    }
    cfc->at(posicao) = contador;
}

//Itera no vetor de cfc's e verifica se ha uma contradicao
std::string eSatisfazivel(std::vector<int> cfc,int propostas){

    for(int i = 0;i < propostas;i++){
        if(cfc.at(i) == cfc.at(i + propostas)){
            return "nao";            
        }
    }
    return "sim";
}

int main(){

    //Declaracao de variaveis
    int seguidores,propostas,auxiliar[4];
    bool loop =  true;
    std::vector<int> propostaCandidato;
    std::vector<bool> visitados;
    std::vector<bool> visitadosInvertido;
    std::vector<int> cfc;
    std::map<int,std::list<int>> grafoPropostas;
    std::map<int,std::list<int>> grafoInvertido;
    std::stack<int> pilhaVertices;
    int vertice;
    int posicao;
    int contador = 1;
    //loop principal
    while(loop){
        std::cin>>seguidores;
        std::cin>>propostas;
        if(seguidores == 0 && propostas == 0){
            loop = false;
        }
        else{
            for(int i = 0;i < seguidores;i++){
                for(int j = 0; j < 4;j++){
                    std::cin >> auxiliar[j];
                }
                montaOpiniaoSeguidor(auxiliar,&propostaCandidato);
            }
            montaGrafo(propostaCandidato,&grafoPropostas,OrdemGrafo::NORMAL);
            montaGrafo(propostaCandidato,&grafoInvertido,OrdemGrafo::INVERTIDO);
            //Inicializacao dos vetores de visita das DFS's
            visitados = std::vector<bool>(propostas*2,false);
            visitadosInvertido = std::vector<bool>(propostas*2,false);
            cfc = std::vector<int>(propostas*2,0);
            //Primeira visita no grafo realizada no algoritmo de Kosaraju para montar a pilha 
            for(int i = 0;i < visitados.size();i++){
                if(visitados.at(i) == 0){
                    DFSPrimeira(i,&pilhaVertices,&visitados,grafoPropostas);
                }
            }
            //Segunda visita no grafo invertido realizada no algoritmo de Kosaraju para indetificar as cfc's
            while(!pilhaVertices.empty()){
                vertice = pilhaVertices.top();
                pilhaVertices.pop();
                posicao = verticeParaPosicao(vertice,visitadosInvertido.size());
                if(!visitadosInvertido.at(posicao)){
                    DFSSegunda(posicao,contador,&visitadosInvertido,&cfc,grafoInvertido);
                    contador ++;
                }
            }
            //Imprimi o resultado
            std::cout<<eSatisfazivel(cfc,propostas)<<std::endl;
            //Limpa variavies para o proximo teste
            grafoInvertido.clear();
            grafoPropostas.clear();
            propostaCandidato.clear();
            contador = 1;
        }
    }
    return 0;
}