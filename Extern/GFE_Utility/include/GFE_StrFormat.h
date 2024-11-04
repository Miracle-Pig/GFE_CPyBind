#ifndef GFE_STRFORMAT_H
#define GFE_STRFORMAT_H

#endif // GFE_STRFORMAT_H


struct ToStringFormat {
    static int precision;    // 小数点后位数
    static bool scientific;     // 手动设置是否科学计数法表示。仅在auto_scientific=false时生效
    static bool auto_scientific;    // 自动判断是否使用科学计数法
    static int auto_sci_int_limit;       // 自动使用科学计数法时，整数部分的最大位数
};
int ToStringFormat::precision = 4;
bool ToStringFormat::scientific = true;
bool ToStringFormat::auto_scientific = true;
int ToStringFormat::auto_sci_int_limit = 6;

