import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.integrate import solve_ivp

P = 4

#In[1]:  ПЗ
def f1(x, u):
    return u**2/(1 + x**4) + u - (u**3)*np.sin(10*x)

#In[2]: контроль погрешности
def lee(v2, v, Lee, EPS, c1, c2, h):
    S = (v2[-1] - v[-1])/((2**P) - 1)
    Lee.append(S * (2**P))
    if np.abs(S) < (EPS/(2**(P+1))):
        h = 2*h
        c1.append(c1[-1])
        c2.append(c2[-1] + 1)
    elif abs(S) > EPS:
        h /= 2
        c1.append(c1[-1] + 1)
        c2.append(c2[-1])
    else:
        c1.append(c1[-1])
        c2.append(c2[-1])
    return h

#In[3]:  расчет приближенной траектории при изменяющемся шаге

def K(xn, vn, h, k):
    return f1(xn + (h/2), vn + (h/2)*k)

def sub_RK(xn, vn, h):
    k = []
    k.append(f1(xn, vn))
    k.append(K(xn, vn, h, k[0]))
    k.append(K(xn, vn, h, k[1]))
    k.append(K(xn, vn, h, 2*k[2]))
    return k

def method_RK_with_lee(x0, u0, h0, Nmax, EPS):
    x = []
    v = []
    v2 = []
    LEE = []
    hi = []
    c1 = []
    c2 = []

    x.append(x0)
    v.append(u0)
    v2.append(u0)
    hi.append(h0)

    xn = x0 + h0
    i = 1
    h = h0
    
    LEE.append(0)
    c1.append(0)
    c2.append(0)
    while i < Nmax:
        k = sub_RK(xn, v[-1], h)
        v.append(v[-1] + (h/6) * (k[0] + 2 * k[1] + 2 * k[2] + k[3]))

        k_tmp = sub_RK(xn - h/2, v[-2], h/2)
        v2_tmp = v[-2] + (h/12) * (k_tmp[0] + 2 * k_tmp[1] + 2 * k_tmp[2] + k_tmp[3])
        k2 = sub_RK(xn, v2_tmp, h/2)
        v2.append(v2_tmp + (h/12) * (k2[0] + 2 * k2[1] + 2 * k2[2] + k2[3]))

        x.append(xn)
        h = lee(v2, v, LEE, EPS, c1, c2, h)
        hi.append(h)
        if (np.abs(v[-1] - v[-2]) < EPS):
            break
        xn += h
        i += 1

    return x, v, v2, LEE, hi, c1, c2


#In[5]:  построение графика

def grafik_v(xi, vi):
    plt.plot(xi, vi, 'b-')

#In[6]:  вывод в таблицу

def print_to_excel(xi, vi, v2i, lee, hi, C1, C2):
    out = pd.DataFrame()

    out["Xi"] = xi
    out["Vi"] = vi
    out["V2i"] = v2i
    out["Vi - V2i"] = [x-y for x,y in zip(vi, v2i)]
    out["ОЛП"] = lee
    out["hi"] = hi
    out["C1"] = C1
    out["C2"] = C2

    out.to_excel("output2_1.xlsx", sheet_name="test_example", index=False)

#In[]:
plt.close()

EPSILON = 0.00001
h = 0.001
x0 = 0
u0 = 5
max_count = 1000

xi, vi, v2i, lee, hi, C1, C2 = method_RK_with_lee(x0, u0, h, max_count, EPSILON)

print_to_excel(xi, vi, v2i, lee, hi, C1, C2)

grafik_v(xi, vi)

# In[0]: check
# def rhs(x, u):
#     return u**2/(1 + x**4) + u - (u**3)*np.sin(10*x)

# sol1 = solve_ivp(rhs, [0., 100.], (0., 10.), method = 'RK45', rtol=1e-12)
# u, y = sol1.y
# x = sol1.t
# plt.plot(x, y, 'r-')


plt.show()