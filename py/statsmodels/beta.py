import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

def beta(y, x):
    mask = ~np.isnan(y) & ~np.isnan(x)
    y = y[mask]
    x = x[mask]
    slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
    return intercept, slope, p_value

np.random.seed(12345678)
x = np.random.random(10)
y = np.random.random(10)
# slope, intercept, r_value, p_value, std_err = stats.linregress(x, y)
# print("r-squared:", r_value**2)
alpha, beta, p_value = beta(y, x)
print(alpha, beta, p_value)

plt.plot(x, y, 'o', label='original data')
plt.plot(x, alpha + beta*x, 'r', label='fitted line')
plt.legend()
plt.show()
