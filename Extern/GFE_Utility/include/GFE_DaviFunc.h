#pragma once
#include <vector>
#ifdef __linux
#include <math.h>
#endif

template <class T>
struct DavidenkovFunc
{
    T A,B,Gamma0,TauMax,Lambda,G;

    DavidenkovFunc() {}
    DavidenkovFunc(const DavidenkovFunc<T>& rhs) {
        this->A = rhs.A;
        this->B = rhs.B;
        this->Gamma0 = rhs.Gamma0;
        this->TauMax = rhs.TauMax;
        this->Lambda = rhs.Lambda;
        this->G = rhs.G;
    }

    void SetParams(T A_, T B_, T Gamma0_, double TauMax_, double Lambda_, double G_)
    {
        A = A_;
        B = B_;
        Gamma0 = Gamma0_;
        TauMax = (T)TauMax_;
        Lambda = (T)Lambda_;
        G = (T)G_;
    }

    T Sign(T v) {
        return (v == (T)0) ? (T)0 : (v > (T)0) ? (T)1 : (T)-1;
    }

    T H(T gamma) {
        T _a = pow(abs(gamma) / Gamma0, 2.0*B);
        return pow(_a/(1.0+_a), A);
    }

    T Taui(T gamma)
    {
        T t1 = G*gamma*(1.0-H(gamma));
        if(t1 > TauMax) t1 = TauMax - (t1 - TauMax) * Lambda * G;
        else if( t1 < -TauMax) t1 = -TauMax + (t1 - TauMax) * Lambda * G;
        return t1;
    }

    T Taub(T gamma1, T gammac, T taoc, T n)
    {
        return taoc+G*(gamma1-gammac)*(1.0-H(abs(gamma1-gammac)/2.0/n));
    }

    T Resh(T nk, T dir, T gammam, T gammac, T taoc)
    {
        auto gamma1 = dir * gammam;
        return Taui(gamma1) - Taub(gamma1, gammac, taoc, nk);
    }

    T FindN(T dir, T gammam, T gammac, T taoc)
    {
        T nk1, H1;
        if(dir > 0.0) {
            H1 = 1.0 - (Taui(gammam)-taoc) / (G*(gammam-gammac));
            H1 = pow(H1, 1.0/A);
            H1 = pow(H1/(1.0-H1), 1.0/2.0/B)*Gamma0;
            nk1 = abs(gammam-gammac)/2.0/H1;
        }
        else {
            H1 = 1.0 - (Taui(-gammam)-taoc) / (G*(-gammam-gammac));
            H1 = pow(H1, 1.0/A);
            H1 = pow(H1/(1.0-H1), 1.0/2.0/B)*Gamma0;
            nk1 = abs(-gammam-gammac)/2.0/H1;
        }
        return nk1;
    }

    void Cal(T gammat, T* inSta, T* OutSta)
    {
        T gammar, taor, gammacr, taocr, gammamr, taomr, dirr, br, nr, gamma, tao, gammac, taoc, gammam, taom, dir, n, Gt;

        gamma = inSta[0];
        tao = inSta[1];
        gammac = inSta[2];
        taoc = inSta[3];
        gammam = inSta[4];
        taom = inSta[5];
        dir = inSta[6];
        n = inSta[7];

        if(gammat >= gammam) {
            br = (T)1.0;
            gammamr = gammat;
            taomr = Taui(gammamr);
            gammacr = gammat;
            taocr = taomr;
            gammar = gammat;
            taor = taomr;
            dirr = (T)1.0;
            nr = n;
        }
        else if(gammat < -gammam) {
            br = (T)1.0;
            gammamr = -gammat;
            taomr = Taui(-gammamr);
            gammacr = -gammat;
            taocr = taomr;
            gammar = gammat;
            taor = -taomr;
            dirr = (T)-1.0;
            nr = n;
        }
        else {
            gammamr = gammam;
            br = (T)0.0;
        }

        gammar = gammat;
        if(br == 0.0) {
            if((gammat - gamma)*dir >= 0.0) {
                taor = Taub(gammat, gammac ,taoc, n);
                gammacr = gammac;
                taocr = taoc;
                gammamr = gammam;
                taomr = taom;
                dirr = dir;
                nr = n;
            }
            else {
                gammacr = gamma;
                taocr = tao;
                nr = FindN(Sign(gammat - gamma), gammam, gammacr, taocr);
                taor = Taub(gammat, gammacr, taocr, nr);
                gammamr = gammam;
                taomr = taom;
                dirr = Sign(gammat - gamma);
            }
        }

        if(br > 0.0)
        {
            Gt = G * (1.0 - (1.0 + 2.0 * A * B * pow(abs(Gamma0), 2.0 * B) / (pow(abs(Gamma0), 2.0 * B) + pow(abs(gammat), 2.0 * B))) * H(gammat));
        }
        else
        {
            Gt = G * (1.0 - (1.0 + 2.0 * A * B * pow(2.0 * nr * Gamma0, 2.0 * B) / (pow(2.0 * nr * Gamma0, 2.0 * B) + pow(abs(gammat - gammac), 2.0 * B))) * H(abs(gammat - gammac) / 2.0 / nr));
        }

        OutSta[0] = gammar;
        OutSta[1] = taor;
        OutSta[2] = gammacr;
        OutSta[3] = taocr;
        OutSta[4] = gammamr;
        OutSta[5] = taomr;
        OutSta[6] = dirr;
        OutSta[7] = nr;
        OutSta[8] = Gt;
    }

    T LinearInterp(T x, std::vector<T> xx)
    {
        if(x >= 0.0 && x < 1.0)
        {
            T k = xx[1] - xx[0];
            return k * x + xx[0];
        }
        else if(x >= 1.0 && x < 2.0)
        {
            T k = xx[2] - xx[1];
            return k * x + xx[1] - k;
        }
        else if(x >= 2.0 && x <= 3.0)
        {
            T k = xx[3] - xx[2];
            return k * x - k * 2.0 + xx[2] ;
        }
        else
            return (T)0.0;
    }

    std::pair<T, T> GetTg(T gamma)
    {

        std::vector<T> xx = {(T)0, gamma, -gamma, gamma};
        int xxSize = xx.size();
        T sta[9] = {(T)0, (T)0, (T)0, (T)0, (T)0, (T)0, (T)0, (T)1, (T)0};
        int er = 30;
        int nb = (xxSize - 1) * er;
        std::vector<std::vector<T>> fg;
        T gt = (T)0;
        for(int i = 0; i < nb; ++i)
        {
            Cal(LinearInterp((T)((i + 1) * 1.0 / er), xx), sta, sta);
            fg.push_back({sta[0], sta[1]});
            if(i == nb - 1)
                gt = sta[8];
        }
        T s = (T)0;
        for(int i = 0; i < er; ++i)
        {
            s += xx[1] / (T)er * (fg[2 * er + i][1] - fg[2 * er - 2 - i][1]);

        }
        return {gt / G, s / 4.0 / 3.14159265 / (fg[er - 1][0] * fg[er - 1][1] / 2.0)};
    }

};
