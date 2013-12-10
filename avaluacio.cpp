#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

struct EventDetection{
    string ID, event;
};

void FillVector(vector<EventDetection>& res, EventDetection& ED, ifstream& F, string& fID, string& fEv){
    F.open("Resultats.txt");
    string linia="";
    while(!F.eof()){
        getline(F, linia);
        int idx=linia.find(' ');
        if(idx!=-1){
            fID=linia.substr(0, idx);
            fEv=linia.substr(idx+1, linia.size());
            ED.ID=fID;
            ED.event=fEv;
            res.push_back(ED);
        }
    }
    F.close();
}

void FillMap(map<string, string>& sols, ifstream& F, string& fID, string& fEv){
    string linia="";
    F.open("Solucions.txt");
    while(!F.eof()){
        getline(F, linia);
        int idx=linia.find(' ');
        if(idx!=-1){
            fID=linia.substr(0, idx);
            fEv=linia.substr(idx+1, linia.size());
            sols[fID]=fEv;
        }
    }
    F.close();
}

map<string, int> IndexationMap(){
    map<string, int> idx;
    idx["concert"]=1; idx["exhibition"]=2; idx["fashion"]=3; idx["non_event"]=4; idx["other"]=5;
    idx["protest"]=6; idx["sports"]=7; idx["theater_dance"]=8; idx["conference"]=9;
    return idx;
}

void ConfusionMatrixConstruction(const vector<EventDetection>& res, map<string, string> sols, map<string, int> aux, float ConfusionMatrix[10][10]){
    for(int i=0; i<res.size(); i++){
        //cout<<res[i].ID<<' '<<res[i].event<<endl;
        map<string, string>::iterator it=sols.find(res[i].ID);
        //cout<<(*it).first<<' '<<(*it).second<<endl;
        if(it!=sols.end()){
            map<string, int>::iterator it_res=aux.find(res[i].event);
            map<string, int>::iterator it_sols=aux.find((*it).second);
            if(it_res!=aux.end() && it_sols!=aux.end()){
                ConfusionMatrix[(*it_sols).second][(*it_res).second]++;
            }
        }
    }
}

/*
void ShowConfusionMatrix(float ConfusionMatrix[10][10]){
    for(int i=1; i<10; i++){
        for(int j=1; j<10; j++){
            cout<<ConfusionMatrix[i][j]<<' ';
        }
        cout<<endl;
    }
}
*/

void RatesExtraction(const float ConfusionMatrix[10][10], float pc[10], float pf[10], float nc[10], float total){
    //FILES
    for (int i=1; i<10; i++){
        for(int j=1; j<10; j++){
            if(i==j){
                pc[i]+=ConfusionMatrix[i][j];
            } else{
                nc[i]+=ConfusionMatrix[i][j];
            }
            total+=ConfusionMatrix[i][j];
        }

    }
    //COLUMNES
    for (int j=1; j<10; j++){
        for(int i=1; i<10; i++){
            if(i!=j){
                pf[j]+=ConfusionMatrix[i][j];
            }
            total+=ConfusionMatrix[i][j];
        }
    }
}

float Median(float Vector[10]){
    float median=0.0;
    for(int j=1; j<10; j++){
        if(Vector[j]!=0){
            median+=Vector[j];
        }
    }
    return median/9.0;
}

int main(){
    string fID, fEv;
    //Lectura fitxer Resultats
    vector<EventDetection> res;
    EventDetection ED;
    ifstream F;
    FillVector(res, ED, F, fID, fEv);


    //Lectura fitxer solucions
    map<string, string> sols;
    FillMap(sols, F, fID, fEv);

    //Creació map auxiliar per a l'assignació de nom_event=index
    map<string, int> aux=IndexationMap();


    //Búsqueda IDs al map de solucions
    float ConfusionMatrix[10][10]={{0}};//tot zeros
    ConfusionMatrixConstruction(res, sols, aux, ConfusionMatrix);
    //ShowConfusionMatrix(ConfusionMatrix)

    //Extreure Precision, Recall i F1score de la matriu de confusió
    float pc[10]={0}, pf[10]={0}, nc[10]={0};
    float total=0;
    RatesExtraction(ConfusionMatrix, pc, pf, nc, total);

    //Càlcul i escriptura de paràmetres
    float Precision[10]={0}, Recall[10]={0}, f1Score[10]={0}, Specificity[10]={0};
    float PositiveProb=0;
    for(int k=1; k<10; k++){
        Precision[k]=pc[k]/(pc[k]+pf[k]);
        Recall[k]=pc[k]/(pc[k]+nc[k]);
        float p=Precision[k];
        float r=Recall[k];
        if(p!=0 && r!=0){
            f1Score[k]=2.0*(p*r/(p+r));
        } else{
            f1Score[k]=0.0;
        }
        Specificity[k]=nc[k]/(nc[k]+pf[k]);
        PositiveProb+=pf[k]+pc[k];
    }
    PositiveProb/=total;

    ofstream G;
    G.open("mesures.txt");
    G<<"Precision: "<<Median(Precision)<<endl;
    G<<"Recall: "<<Median(Recall)<<endl;
    G<<"f1Score: "<<Median(f1Score)<<endl;

    //WriteOutput files
    /*ofstream G, H, I, J, K;
    G.open("Precision.txt"); H.open("Recall.txt"); I.open("f1Score.txt"); J.open("Specificity.txt");
    K.open("PositiveP.txt");
    for(int n=1; n<10; n++){
        G<<Precision[n]<<endl;
        H<<Recall[n]<<endl;
        I<<f1Score[n]<<endl;
        J<<Specificity[n]<<endl;
        K<<PositiveProb<<endl;
    }

    //Delimitador matlab
    G<<','<<endl; H<<','<<endl; I<<','<<endl; J<<','<<endl;
    */

}

