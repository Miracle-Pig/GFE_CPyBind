#pragma once
#include "GFE_DaviFunc.h"
#include <memory>

class MatSolver {
public:
    enum TypeEnum {
        Elastic,
        Davidenkov
    };
    MatSolver(int t) { type = t; }
    virtual bool DeepCopy(std::shared_ptr<MatSolver>);
    virtual double Calc(double e) = 0;
    int Type() { return type; }
    virtual void Init();
    double GetM() { return M; }

    double Den;
    double G;
    double Poisson;
    double aR=0,bR=0;       // alpha阻尼, beta阻尼
    bool isZ = false;

protected:
    void InitM();

    int type;
    double M;
};

class ElasticSolver : public MatSolver {
public:
    ElasticSolver();
    ElasticSolver(double _G);
    virtual bool DeepCopy(std::shared_ptr<MatSolver>) override;
    double Calc(double e) override;
};

class Davidenkov : public MatSolver{
public:
    Davidenkov();
    Davidenkov(double _A, double _B, double _Gamma0, double _TauMax, double _Lambda, double _G);
    virtual bool DeepCopy(std::shared_ptr<MatSolver>) override;
    virtual void Init() override;
    double Calc(double e) override;
    std::pair<double, double> GetTg(double gamma) { return m_func.GetTg(gamma); }

    double A = 0;
    double B = 0;
    double Gamma0 = 0;
    double TauMax = 0;
    double Lambda = 0;
    static constexpr int StaParamNum = 9;
    double Sta[StaParamNum] = {0,0,0,0,0,0,0,1,0};

private:
    DavidenkovFunc<double> m_func;
};
