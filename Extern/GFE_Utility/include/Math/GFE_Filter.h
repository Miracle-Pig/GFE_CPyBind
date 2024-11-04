#pragma once
#include <cmath>
#include <vector>
//! @brief 滤波器

namespace GFE {

//! @brief 二阶过滤器
class BiFilter {
public:
    BiFilter(double cutoff_freq, double sampling_freq);
    double Process(double input);

private:
    double a0, a1, a2, b0, b1, b2;
    double x1, x2, y1, y2;
};

//! @brief 使用二阶过滤器级联逼近实现的Butterworth过滤器。用于显式动力分析
//! @param cutoff_freq 截止频率
//! @param n 过滤器阶数
//! @param 显式计算的时间步长
void ButterworthFilter(const std::vector<double>& x, std::vector<double>& y, double cutoff_freq, int n, double time_increment);

void ButterworthFilter(const std::vector<float>& x, std::vector<float>& y, double cutoff_freq, int n, double time_increment);

}
