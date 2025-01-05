#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main()
{
    double p;
    int a, b, c, d, n;
    cin >> p >> a >> b >> c >> d >> n;

    double max_price = 0.0;
    double max_decline = 0.0;

    for (int k = 1; k <= n; ++k)
    {
        double angle1 = a * k + b;
        double angle2 = c * k + d;
        double price = p * (sin(angle1) + cos(angle2) + 2);

        if (k == 1)
        {
            max_price = price;
        }
        else
        {
            if (price > max_price)
            {
                max_price = price;
            }
            else
            {
                double decline = max_price - price;
                if (decline > max_decline)
                {
                    max_decline = decline;
                }
            }
        }
    }

    cout << fixed << setprecision(6) << max_decline << endl;

    return 0;
}
