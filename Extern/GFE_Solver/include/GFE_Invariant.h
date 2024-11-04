#include <GFE_Solver_Global.h>
#include <vector>

namespace GFE {
using std::vector;
using data_t = float;
vector<data_t> CalcPrincipal(int type, std::size_t size, const vector<GFE::data_t>& X11, const vector<GFE::data_t>& X22, const vector<GFE::data_t>& X33,
                                                  const vector<GFE::data_t>& X12, const vector<GFE::data_t>& X23, const vector<GFE::data_t>& X31);

data_t CalcMaxPrincipal(data_t x11, data_t x22, data_t x33,
                                        data_t x12, data_t x23, data_t x31);
data_t CalcMidPrincipal(data_t x11, data_t x22, data_t x33,
                                        data_t x12, data_t x23, data_t x31);
data_t CalcMinPrincipal(data_t x11, data_t x22, data_t x33,
                                        data_t x12, data_t x23, data_t x31);
data_t CalcMaxPrincipalAbs(data_t x11, data_t x22, data_t x33,
                                        data_t x12, data_t x23, data_t x31);

data_t CalcInPlaneMaxPrincipal(data_t x11, data_t x22, data_t x12);
data_t CalcInPlaneMinPrincipal(data_t x11, data_t x22, data_t x12);
data_t CalcInPlaneMaxPrincipalAbs(data_t x11, data_t x22, data_t x12);

GFE_Solver_DLIB data_t CalcMises(data_t x11, data_t x22, data_t x33,
                                 data_t x12, data_t x23, data_t x31);

            /******* 计算不变量数组。数组长度以x11为准*******/

GFE_Solver_DLIB vector<data_t> CalcMaxPrincipal(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x33,
                                        const vector<data_t>& x12, const vector<data_t>& x23, const vector<data_t>& x31);
GFE_Solver_DLIB vector<data_t> CalcMidPrincipal(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x33,
                                        const vector<data_t>& x12, const vector<data_t>& x23, const vector<data_t>& x31);
GFE_Solver_DLIB vector<data_t> CalcMinPrincipal(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x33,
                                        const vector<data_t>& x12, const vector<data_t>& x23, const vector<data_t>& x31);
GFE_Solver_DLIB vector<data_t> CalcMaxPrincipalAbs(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x33,
                                        const vector<data_t>& x12, const vector<data_t>& x23, const vector<data_t>& x31);

GFE_Solver_DLIB vector<data_t> CalcInPlaneMaxPrincipal(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x12);
GFE_Solver_DLIB vector<data_t> CalcInPlaneMinPrincipal(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x12);
GFE_Solver_DLIB vector<data_t> CalcInPlaneMaxPrincipalAbs(const vector<data_t>& x11, const vector<data_t>& x22, const vector<data_t>& x12);

GFE_Solver_DLIB vector<data_t> CalcMises(const vector<GFE::data_t>& X11, const vector<GFE::data_t>& X22, const vector<GFE::data_t>& X33,
                                         const vector<GFE::data_t>& X12, const vector<GFE::data_t>& X23, const vector<GFE::data_t>& X31);

GFE_Solver_DLIB vector<data_t> CalcMagnitude(const vector<data_t>& x1, const vector<data_t>& x2, const vector<data_t>& x3);
}
