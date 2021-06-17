#include <iostream>
#include <fstream>

using namespace std;

void Ts_func(float *Ts,float *close,int pnt);
void Ks_func(float *Ks,float *close,int pnt);
void SsA_func(float *SsA,float *close,int pnt,int line);
void SsB_func(float *SsB,float *close,int pnt,int line);
void Cs_func(float *Cs,float *close,int pnt,int line);

int main()
{
    cout.precision(10);
    //Reading the data out of the file
    ifstream data_file("data2020.csv");
    string entry;
    int line=0;
    while(getline(data_file,entry)){
        line=line+1;
    }
    data_file.clear();
    data_file.seekg(0);

    float close[line];
    float full;
    char semi;
    int i=0;
    while(!data_file.eof()){
        data_file >> full >> full >> semi >> full >> semi >> full >> semi >> full >> semi >> close[i] >> semi >> full;
        i=i+1;
    }

    data_file.close();

    //Indicator variables
    float Ts=0,Ks=0,SsA=0,SsB=0,Cs=0;
    bool ord_open=false;
    int ord_type=0;
    float open_price=0;
    float pl=0;

    ofstream output_file("output.txt");
    for(int pnt=0;pnt<line;++pnt){
        Ts_func(&Ts,close,pnt);
        Ks_func(&Ks,close,pnt);
        SsA_func(&SsA,close,pnt,line);
        SsB_func(&SsB,close,pnt,line);
        Cs_func(&Cs,close,pnt,line);
        output_file<<close[pnt]<<" "<<Ts<<" "<<Ks<<" "<<SsA<<" "<<SsB<<" "<<Cs<<" ";

        if(ord_open==true){
            if(ord_type==1){
                if(close[pnt]>Ts&&Ts>Ks&&close[pnt]>SsA&&SsA>SsB&&close[pnt]>close[pnt-25]){
                    output_file<<"Buy open"<<endl;
                }else{
                    pl=pl+close[pnt]-open_price;
                    output_file<<"Buy order closed p/l: "<<close[pnt]-open_price<<"Total p/l: "<<pl<<endl;
                    ord_open=false;
                    ord_type=0;
                }
            }
            if(ord_type==-1){
                if(close[pnt]<Ts&&Ts<Ks&&close[pnt]<SsA&&SsA<SsB&&close[pnt]<close[pnt-25]){
                    output_file<<"Sell open"<<endl;
                }else{
                    pl=pl+open_price-close[pnt];
                    output_file<<"Sell order closed p/l: "<<close[pnt]-open_price<<"Total p/l: "<<pl<<endl;
                    ord_open=false;
                    ord_type=0;
                }
            }
        }else{
            if(Ts!=0&&Ks!=0&&SsA!=0&&SsB!=0&&close[pnt]>Ts&&Ts>Ks&&close[pnt]>SsA&&SsA>SsB&&close[pnt]>close[pnt-25]){
                output_file<<"Buy order opened"<<endl;
                open_price=close[pnt];
                ord_type=1;
                ord_open=true;
            }else if(Ts!=0&&Ks!=0&&SsA!=0&&SsB!=0&&close[pnt]<Ts&&Ts<Ks&&close[pnt]<SsA&&SsA<SsB&&close[pnt]<close[pnt-25]){
                output_file<<"Sell order opened"<<endl;
                open_price=close[pnt];
                ord_type=-1;
                ord_open=true;
            }else{
                output_file<<endl;
            }
        }
    }
    output_file.close();
    cout<<pl;
    return 0;
}

void Ts_func(float *Ts,float *close,int pnt){
    float cmax=0,cmin=3;
    *Ts=0;
    if(pnt>7){
        for(int j=pnt-8;j<pnt+1;++j){
            if(close[j]>cmax){
                cmax=close[j];
            }
            if(close[j]<cmin){
                cmin=close[j];
            }
        }
        *Ts=(cmax+cmin)/2;
    }
}

void Ks_func(float *Ks,float *close,int pnt){
    float cmax=0,cmin=3;
    *Ks=0;
    if(pnt>24){
        for(int j=pnt-25;j<pnt+1;++j){
            if(close[j]>cmax){
                cmax=close[j];
            }
            if(close[j]<cmin){
                cmin=close[j];
            }
        }
        *Ks=(cmax+cmin)/2;
    }
}

void SsA_func(float *SsA,float *close,int pnt,int line){
    float cmax=0,cmin=3,Ts=0,Ks=0;
    int func_pnt=pnt-25;
    *SsA=0;
    if(func_pnt>24){
        if(func_pnt<line-25){
            for(int j=func_pnt-8;j<func_pnt+1;++j){
                if(close[j]>cmax){
                    cmax=close[j];
                }
                if(close[j]<cmin){
                    cmin=close[j];
                }
            }
            Ts=(cmax+cmin)/2;

            cmax=0,cmin=3;

            for(int j=func_pnt-25;j<func_pnt+1;++j){
                if(close[j]>cmax){
                    cmax=close[j];
                }
                if(close[j]<cmin){
                    cmin=close[j];
                }
            }
            Ks=(cmax+cmin)/2;
        }
        *SsA=(Ts+Ks)/2;
    }
}

void SsB_func(float *SsB,float *close,int pnt,int line){
    float cmax=0,cmin=3;
    int func_pnt=pnt-25;
    *SsB=0;
    if(func_pnt>50){
        if(func_pnt<line-25){
            for(int j=func_pnt-51;j<func_pnt+1;++j){
                if(close[j]>cmax){
                    cmax=close[j];
                }
                if(close[j]<cmin){
                    cmin=close[j];
                }
            }
        }
        *SsB=(cmax+cmin)/2;
    }
}

void Cs_func(float *Cs,float *close,int pnt,int line){
    int func_pnt=pnt+25;
    *Cs=0;
    if(func_pnt<line){
        *Cs=close[func_pnt];
    }
}
