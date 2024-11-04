#ifndef GFE_HYPERELATESTDATA_H
#define GFE_HYPERELATESTDATA_H
#include <Eigen/Eigen>
#include <iostream>
#include <vector>

namespace LeastSquaresFit {

std::vector<float> ReducedPoly(int N, std::vector<double> uniaxial, std::vector<double> biaxial, std::vector<double> planar, std::vector<double> volumetric);

struct MRTestData
{
    MRTestData(double x, double y, int tp)
        : x_(x), y_(y), tp_(tp) {}

    template <typename T>
    bool operator()(const T* const Cij, T* residual) const
    {
        switch (tp_)
        {
        case 0:
        {
            // Uniaxial
            residual[0] = pow(1.0 - 2.0 * (1.0 - pow(y_ + 1, -3)) * (Cij[0] * (y_ + 1) + Cij[1]) / x_, 2);
            break;
        }
        case 1:
        {
            // Biaxial
            break;
        }
        case 2:
        {
            // Planar
            break;
        }
        }
        return true;
    }

private:
    const double x_;
    const double y_;
    const int tp_;
};

// 兼容N=1时的Poly
std::vector<float> MR(std::vector<double> uniaxial, double v);

struct PolyTestData
{
    PolyTestData(double x, double y, int tp)
        : x_(x), y_(y), tp_(tp) {}

    template <typename T>
    bool operator()(const T* const Cij, T* residual) const
    {
        double I1 = pow(y_ + 1, 2) + 2 / (y_ + 1);
        double I2 = pow(y_ + 1, -2) + 2 * (y_ + 1);
        switch (tp_)
        {
        case 0:
        {
            // Uniaxial
            residual[0] = pow(1.0 - 2.0 * (1.0 - pow(y_ + 1, -3)) * (Cij[0] * (y_ + 1) + Cij[1] + 2.0 * Cij[2] * (y_ + 1) * (I1 - 3) + Cij[3] * (I1 - 3 + (y_ + 1) * (I2 - 3)) + 2.0 * Cij[4] * (I2 - 3)) / x_, 2);
            break;
        }
        case 1:
        {
            // Biaxial
            break;
        }
        case 2:
        {
            // Planar
            break;
        }
        }
        return true;
    }

private:
    const double x_;
    const double y_;
    const int tp_;
};

// N一定为2
std::vector<float> Poly(std::vector<double> uniaxial, double v);

struct FoamTestData
{
    FoamTestData(double x, double y, double v, int N, int tp)
        : x_(x), y_(y), v_(v), N_(N), tp_(tp) {}

    template <typename T>
    bool operator()(const T* const Para, T* residual) const
    {
        double beta = v_ / (1 - 2 * v_);
        switch (tp_)
        {
        case 0:
        {
            // Uniaxial
            double J = (y_ + 1) * pow(1 - v_ * y_, 2);
            T tmp = T(0.0);
            for (int i = 0; i < N_; ++i)
                tmp += Para[i * 2] / Para[i * 2 + 1] * pow(y_ + 1, Para[i * 2 + 1]) - pow(J, -Para[i * 2 + 1] * beta);
            residual[0] = pow(1.0 - 2.0 / (y_ + 1) * tmp / x_, 2);
            break;
        }
        case 1:
        {
            // Biaxial
            break;
        }
        case 2:
        {
            // Planar
            break;
        }
        }
        return true;
    }

private:
    const double x_;
    const double y_;
    const double v_;
    const int N_;
    const int tp_;
};

std::vector<float> Foam(std::vector<double> uniaxial, int n, double v);

}
#endif
