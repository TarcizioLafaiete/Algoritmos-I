#include <vector>
#include <iostream>

std::vector<int> inverteSequencia(std::vector<int> vector){
    std::vector<int> vectorInv;
    for(int i = vector.size();i > 0; i --){
        vectorInv.insert(vectorInv.end(),vector.at(i-1));
    }
    return vectorInv;
}

void longestIncreasingSubsequence(std::vector<int> vector,std::vector<int>* lis){

    lis->at(0) = 1;
    for(int i = 1;i < vector.size();i++){
        lis->at(i) = 1;
        for(int j  = 0; j < i;j++){
            if(vector.at(i) > vector.at(j) && lis->at(i) < lis->at(j) + 1){
                lis->at(i) = lis->at(j) + 1;
            }
        }
    }
}

void longestDecreasingSubsequence(std::vector<int> vector,std::vector<int>* lds){

    lds->at(0) = 1;
    for(int i = 1; i < vector.size();i++){
        lds->at(i) = 1;
        for(int j = 0; j < i; j++){
            if(vector.at(i) < vector.at(j) && lds->at(i) < lds->at(j) + 1){
                lds->at(i) = lds->at(j)+1;
            }
        }
    }
}

void montaResposta(std::vector<int> lds,std::vector<int> lis, std::vector<int>* resposta){

    for(int i = 0; i < lis.size();i++){
        resposta->at(i) = lis.at(i) + lds.at(i);
    }
}

int calculaOtimo(std::vector<int> soma){
    int res = 0;
    for(int i = 0; i < soma.size(); i++){
        if(res < soma.at(i)){
            res = soma.at(i);
        }
    }
    return res - 1;
}

int main(){

    std::vector<int> seq;
    std::vector <int> seqInv;
    std::vector<int> lds;
    std::vector<int> lis;
    std::vector<int> soma;
    int rolos = 0;
    int testes = 0;
    int valor = 0;

    std::cin >> testes;
    for(int i = 0; i < testes;i++){
        std::cin >> rolos;
        for(int j = 0;j < rolos;j++){
            std::cin >> valor;
            seq.insert(seq.end(),valor);
        }
        lis = std::vector<int>(rolos);
        lds = std::vector<int>(rolos);
        soma = std::vector<int>(rolos);
        seqInv = inverteSequencia(seq);
        longestDecreasingSubsequence(seqInv,&lds);
        longestIncreasingSubsequence(seqInv,&lis);
        montaResposta(lds,lis,&soma);
        std::cout<<calculaOtimo(soma)<<std::endl;
        seq.clear();
        seqInv.clear();
        lds.clear();
        lis.clear();
        soma.clear();
    }
    
    return 0;
}