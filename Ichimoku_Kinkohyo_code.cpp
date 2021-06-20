/*
c++ code that calculates the Ichimoku Kinkohyo idicator values and gives buy and sell signals based on indicator conditions.

The theory for this code is based on the paper;
Short-Term Foreign Exchange Rate Trading Based on the Support/Resistance Level of Ichinoku Kinkohyo
Shaangkun Deng and Akito Sakurai
IEEE 2014

See my PDF file titled "Ichimoku Kinkohyo code theory" in the repository for the theory used for this code.

For questions and comments, you can contact me by messaging my reddit account: s1123581321
*/

#include <iostream>
#include <fstream>

using namespace std;

//Indicator calculation functions
void Ts_func(float *Ts,float *close,int pnt);   //Tenkan-sen
void Ks_func(float *Ks,float *close,int pnt);   //Kijun-sen
void SsA_func(float *SsA,float *close,int pnt,int line);    //Senkou span A
void SsB_func(float *SsB,float *close,int pnt,int line);    //Senkou span B
void Cs_func(float *Cs,float *close,int pnt,int line);  //Chikou span

int main()
{
    //Reading the data out of the file
    //Data from https://www.histdata.com/ in the "Generic ASCII" format
    ifstream data_file("data2020.csv");
    string entry;
    int line=0;

    //Finds the total number of lines in the file
    while(getline(data_file,entry)){
        line=line+1;
    }
    data_file.clear();
    data_file.seekg(0);

    float close[line];
    float full;
    char semi;
    int i=0;

    //Reads the data out of the file, the only data used in analysis is the close price
    while(!data_file.eof()){
        data_file >> full >> full >> semi >> full >> semi >> full >> semi >> full >> semi >> close[i] >> semi >> full;
        i=i+1;
    }

    data_file.close();

    //Indicator variables
    float Ts=0,Ks=0,SsA=0,SsB=0,Cs=0;

    //Order variables
    bool ord_open=false;
    int ord_type=0;


    ofstream output_file("output.txt"); //File to write indicator values and order signals to

    for(int pnt=0;pnt<line;++pnt){

        //Calculating the indicator values
        Ts_func(&Ts,close,pnt);
        Ks_func(&Ks,close,pnt);
        SsA_func(&SsA,close,pnt,line);
        SsB_func(&SsB,close,pnt,line);
        Cs_func(&Cs,close,pnt,line);
        output_file<<close[pnt]<<" "<<Ts<<" "<<Ks<<" "<<SsA<<" "<<SsB<<" "<<Cs<<" ";

        //Conditions for order signals
        if(ord_open==true){
            if(ord_type==1){
                if(close[pnt]<Ts||Ts<Ks||close[pnt]<SsA||SsA<SsB||close[pnt]<close[pnt-25]){
                    ord_open=false;
                    ord_type=0;
                    output_file<<endl;
                }else{
                    output_file<<endl;
                }
            }
            if(ord_type==-1){
                if(close[pnt]>Ts||Ts>Ks||close[pnt]>SsA||SsA>SsB||close[pnt]>close[pnt-25]){
                    ord_open=false;
                    ord_type=0;
                    output_file<<endl;
                }else{
                    output_file<<endl;
                }
            }
        }else{
            if(Ts!=0&&Ks!=0&&SsA!=0&&SsB!=0&&close[pnt]>Ts&&Ts>Ks&&close[pnt]>SsA&&SsA>SsB&&close[pnt]>close[pnt-25]){
                output_file<<"Buy order signal"<<endl;
                ord_type=1;
                ord_open=true;
            }else if(Ts!=0&&Ks!=0&&SsA!=0&&SsB!=0&&close[pnt]<Ts&&Ts<Ks&&close[pnt]<SsA&&SsA<SsB&&close[pnt]<close[pnt-25]){
                output_file<<"Sell order signal"<<endl;
                ord_type=-1;
                ord_open=true;
            }else{
                output_file<<endl;
            }
        }
    }
    output_file.close();
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

/*
The output looks like;

1.12206 1.12207 1.12194 1.12165 1.12173 1.12232 
1.12205 1.12208 1.12194 1.12165 1.12173 1.12235 
1.12204 1.12208 1.12195 1.12165 1.12173 1.12235 
1.1221 1.12208 1.12197 1.12168 1.12173 1.12235 
1.12225 1.12213 1.12204 1.12171 1.12173 1.12234 
1.12225 1.12213 1.12204 1.12172 1.12173 1.12234 
1.12224 1.12214 1.12204 1.12176 1.12173 1.12233 Buy order signal
1.12221 1.12214 1.12204 1.12176 1.12173 1.12225 
1.12226 1.12215 1.12205 1.12176 1.12173 1.12222 
1.12232 1.12218 1.12208 1.12176 1.12173 1.12219 
1.12239 1.12222 1.12211 1.12177 1.12173 1.1221 
1.1224 1.12225 1.12212 1.12178 1.12173 1.12209 
1.12232 1.12231 1.12212 1.12179 1.12173 1.1221 
1.12235 1.12231 1.12212 1.12179 1.12173 1.12202 
1.12237 1.12231 1.12212 1.12179 1.12173 1.12201 
1.12237 1.12231 1.12212 1.12179 1.12173 1.12203 
1.12235 1.12233 1.12212 1.12179 1.12173 1.12188 
1.12235 1.12236 1.1222 1.12185 1.12178 1.1219 
1.12232 1.12236 1.12221 1.12188 1.12181 1.12194 
1.12236 1.12236 1.12221 1.12188 1.12181 1.12193 
1.12236 1.12234 1.12221 1.12193 1.12187 1.12188 Buy order signal
1.12233 1.12234 1.12221 1.12193 1.12187 1.12186 
1.12231 1.12234 1.12221 1.12194 1.12187 1.12181 


The section of the original data file that corresponds to this output in this example is;

20200101 200900;1.122100;1.122110;1.122060;1.122060;0
20200101 201000;1.122060;1.122080;1.122050;1.122050;0
20200101 201100;1.122050;1.122060;1.122000;1.122040;0
20200101 201200;1.122050;1.122100;1.122050;1.122100;0
20200101 201300;1.122110;1.122260;1.122110;1.122250;0
20200101 201400;1.122240;1.122310;1.122240;1.122250;0
20200101 201500;1.122250;1.122260;1.122220;1.122240;0
20200101 201600;1.122240;1.122260;1.122210;1.122210;0
20200101 201700;1.122220;1.122260;1.122190;1.122260;0
20200101 201800;1.122260;1.122320;1.122260;1.122320;0
20200101 201900;1.122320;1.122420;1.122310;1.122390;0
20200101 202000;1.122390;1.122440;1.122360;1.122400;0
20200101 202100;1.122400;1.122410;1.122310;1.122320;0
20200101 202200;1.122330;1.122370;1.122320;1.122350;0
20200101 202300;1.122350;1.122380;1.122340;1.122370;0
20200101 202400;1.122370;1.122390;1.122320;1.122370;0
20200101 202500;1.122360;1.122380;1.122330;1.122350;0
20200101 202600;1.122340;1.122360;1.122330;1.122350;0
20200101 202700;1.122350;1.122370;1.122310;1.122320;0
20200101 202800;1.122330;1.122360;1.122300;1.122360;0
20200101 202900;1.122360;1.122360;1.122350;1.122360;0
20200101 203000;1.122340;1.122350;1.122280;1.122330;0
20200101 203100;1.122320;1.122340;1.122310;1.122310;0
*/
