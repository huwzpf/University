from pandas import read_csv
import matplotlib.pyplot as plt

x = read_csv("D:/Projects/MN2/MN2/MN2/data.csv")
plt.plot(x["n"], x["Jacobi"], label="Jacobi")
plt.plot(x["n"], x["GS"], label="Gauss-Seidel")
plt.xlabel("Rozmiar macierzy")
plt.ylabel("Czas wykonania [ms]")
plt.title("Wykres czasu wykonania algorytmów iteracyjnych \n od rozmiaru macierzy wejściowej")
plt.legend()
plt.show()
s = 0
for index, row in x.iterrows():
    s += row["Jacobi"]/row["GS"]
print(f"Średni stosunek czasu działania algorytmu Jacobiego do Gaussa-Seidla : {s/len(x)}")
