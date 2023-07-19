import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

data_file = np.loadtxt("data.dat", delimiter="\n", unpack=False, dtype=str)
coefficients_file = np.loadtxt("out.dat", delimiter="\n", unpack=False, dtype=str)

number_of_data_points = int(data_file[0])

X = np.empty(shape=number_of_data_points)
Y = np.empty(shape=number_of_data_points)

for i in range(number_of_data_points):
    X[i] = float(data_file[i + 1].split(' ')[0])
    Y[i] = float(data_file[i + 1].split(' ')[1])

X, Y = zip(*sorted(zip(X, Y)))

degree = int(coefficients_file[0])

coefficients = np.empty(shape=degree)

for i in range(degree):
    coefficients[i] = float(coefficients_file[i+1])

P_x = np.poly1d(coefficients[::-1])

res = np.sqrt(np.sum(np.square(Y - P_x(X))))

print('Residual for the polynomial fit - ', res)

x = np.linspace(np.min(X), np.max(X), num=100)
mpl.use('tkagg')
plt.title("Polynomial fit for data points")
plt.xlabel("x")
plt.ylabel("y")
plt.plot(x, P_x(x), color='blue', markersize=10, label='Polynomial')
plt.scatter(X, Y, [20 for _ in X], color='red', label='Data points')
plt.grid()
plt.legend()
plt.show()


