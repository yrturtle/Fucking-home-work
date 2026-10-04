#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// 精度阈值，避免浮点数误差
const double eps = 1e-7;

int main()
{
    // 定义2行3列的增广矩阵，对应2个未知数的电桥方程组
    double aug[2][3];
    double E, R1, R2, R3, R0, Rx;

    // 1. 输入电桥所有参数
    cout << "请依次输入电源电压E, R1, R2, R3, R0, Rx：" << endl;
    cin >> E >> R1 >> R2 >> R3 >> R0 >> Rx;

    // 2. 自动填充增广矩阵，符合节点电压法规则
    aug[0][0] = 1.0/R1 + 1.0/R2 + 1.0/R0;
    aug[0][1] = -1.0/R0;
    aug[0][2] = E / R1;

    aug[1][0] = -1.0/R0;
    aug[1][1] = 1.0/R3 + 1.0/Rx + 1.0/R0;
    aug[1][2] = E / Rx;

    cout << "\n填充完成的增广矩阵为：" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<3; j++){
            cout << fixed << setprecision(4) << aug[i][j] << "\t";
        }
        cout << endl;
    }

    // 3. 高斯-约旦消元核心逻辑，专门适配2阶矩阵
    for(int i=0; i<2; i++)
    {
        // 第一步：找当前列主元，避免除以极小值带来的误差
        int max_row = i;
        for(int j=i+1; j<2; j++)
        {
            if(fabs(aug[j][i]) > fabs(aug[max_row][i]))
                max_row = j;
        }
        // 交换主元行到当前行
        for(int j=i; j<3; j++)
        {
            swap(aug[i][j], aug[max_row][j]);
        }

        // 判断主元是否为0，无解直接退出
        if(fabs(aug[i][i]) < eps)
        {
            cout << "方程组无解，电桥参数异常！" << endl;
            return 0;
        }

        // 第二步：把当前主元行归一化，让主元位置变成1
        double div = aug[i][i];
        for(int j=i; j<3; j++)
        {
            aug[i][j] /= div;
        }

        // 第三步：消去所有其他行的当前列元素，直接消成0
        for(int j=0; j<2; j++)
        {
            if(j != i && fabs(aug[j][i]) > eps)
            {
                double factor = aug[j][i];
                for(int k=i; k<3; k++)
                {
                    aug[j][k] -= factor * aug[i][k];
                }
            }
        }
    }

    // 4. 直接输出结果
    cout << "\n求解完成！" << endl;
    cout << "节点A的电压Va = " << fixed << setprecision(4) << aug[0][2] << " V" << endl;
    cout << "节点D的电压Vd = " << fixed << setprecision(4) << E << " V" << endl;

    cout << "节点C的电压Vc = " << fixed << setprecision(4) << aug[1][2] << " V" << endl;
    cout << "节点B的电压Vb = " << fixed << setprecision(4) << 0 << " V" << endl;

    cout << "桥臂R0的电流I0 = " << fixed << setprecision(4) << (aug[0][2] - aug[1][2])/R0 << " A" << endl;

    return 0;
}
