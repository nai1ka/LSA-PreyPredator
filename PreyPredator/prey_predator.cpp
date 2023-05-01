//
// Created by Nail Minnemullin on 30.04.2023.
//
#include "iostream"
#include "cmath"

using namespace std;

void printPointsToFile(double* t,double* data, int n, char *name) {
    static FILE *tempDataFile;
    tempDataFile = ::fopen(name, "w");
    for (int i = 0; i < n; ++i) {
        ::fprintf(tempDataFile, "%f %f\n", t[i], data[i]);
    }
    fclose(tempDataFile);
    fflush(tempDataFile);
}


 void drawGraph( char *kName, char*vName) {
    static FILE *pipe;
    pipe = popen("gnuplot -persistent", "w");
    ::fprintf(pipe, "plot '%s' with lines, '%s' with lines\n", kName, vName);
    pclose(pipe);
}


int main() {
    cout.precision(2);
    cout<<std::fixed;
    int v0, k0, t, n;
    double a1, b1, a2, b2;
    cin >> v0 >> k0 >> a1 >> b1 >> a2 >> b2 >> t >> n;
    double timeStep = 1.0*t/n;
    double time = 0;
    n++;
    double v[n];
    double k[n];
    double tArr[n];


    for (int i = 0; i < n; ++i) {
        v[i] = a2/b2 + (v0- a2/b2)*cos(sqrt(a1*a2)*time)- (k0-a1/b1)*(sqrt(a2)*b1/b2/sqrt(a1))*sin(sqrt(a1*a2)*time);
        k[i] = a1/b1 + (v0- a2/b2)* sqrt(a1)*b2*sin(sqrt(a1*a2)*time)/b1/sqrt(a2) + (k0-a1/b1)*cos(sqrt(a1*a2)*time);
        time +=timeStep;
    }

    time = 0;
    cout<<"t:\n";
    for (int i = 0; i < n; ++i) {
        tArr[i] = time;
        time+=timeStep;
        cout<<tArr[i]<<" ";
    }
    cout<<endl;
    cout<<"v:\n";
    for (int i = 0; i < n; ++i) {
        cout<<v[i]<<" ";
    }
    printPointsToFile(tArr,v,n, "PreyPredator/v.txt");

    cout<<endl;
    cout<<"k:\n";
    for (int i = 0; i < n; ++i) {
        cout<<k[i]<<" ";
    }
    cout<<endl;
    printPointsToFile(tArr,k,n, "PreyPredator/k.txt");

    drawGraph("PreyPredator/k.txt","PreyPredator/v.txt");
}

