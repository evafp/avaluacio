#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

struct EventDetection{
    string ID, event;
};

int main(){
    string fID, fEv;
    //Lectura fitxer Resultats
    vector<EventDetection> res;
    EventDetection ED;
    ifstream F;
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

    //Lectura fitxer solucions
    map<string, string> sols;
    F.open("Solucions.txt");
    linia.clear();
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

    //Creació map auxiliar per a l'assignació de nom_event=index
    map<string, int> aux;
    aux["concert"]=1; aux["exhibition"]=2; aux["fashion"]=3; aux["non_event"]=4; aux["other"]=5;
    aux["protest"]=6; aux["sports"]=7; aux["theater_dance"]=8; aux["conference"]=9;

    //Búsqueda IDs al map de solucions
    int ConfusionMatrix[10][10]={{0}};//tot zeros
    for(int i=0; i<res.size(); i++){
        //cout<<res[i].ID<<' '<<res[i].event<<endl;
        map<string, string>::iterator it=sols.find(res[i].ID);
        //cout<<(*it).first<<' '<<(*it).second<<endl;
        if(it!=sols.end()){
            map<string, int>::iterator it_res=aux.find(res[i].event);
            map<string, int>::iterator it_sols=aux.find((*it).second);
            if(it_res!=aux.end() && it_sols!=aux.end()){
                ConfusionMatrix[(*it_res).second][(*it_sols).second]++;
            }
        }
    }

    //Extreure Precision, Recall i F1score de la matriu de confusió
    int precision, recall, f1score;
    int pc=0, pf=0, nc=0;
    for (int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            cout<<ConfusionMatrix[i][j]<<' ';
        }
        cout<<endl;
    }
}
