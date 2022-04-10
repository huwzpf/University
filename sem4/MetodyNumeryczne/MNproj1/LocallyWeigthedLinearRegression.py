import numpy as np
import matplotlib.pyplot as plt

def weight_matrix(point, x, tau):
    n = x.shape[0]
    ret = np.eye(n)
    for i in range(n):
        ret[i, i] = np.exp(((x[i] - point).dot(np.transpose(x[i] - point))) / (-2 * tau * tau))
    return ret


def fit(y, tau):
    n = y.shape[0]
    x = np.arange(0, n)

    x_a = np.append(np.ones(n).reshape(n, 1), x.reshape(n, 1), axis=1)
    point = np.array([1, n - 1])
    w = weight_matrix(point, x_a, tau)
    theta = np.linalg.pinv(np.transpose(x_a).dot(w.dot(x_a))).dot(np.transpose(x_a).dot(w.dot(y)))

    # b, a in y = ax + b
    return theta[0], theta[1]

def plot_prediction(m, s, m_a, m_b, s_a, s_b, dist):
    # zakładamy równą długość wektorów
    x_end = len(m) + dist
    x = np.arange(0, len(m))
    plt.figure()
    plt.scatter(x, s, color='#ff0000')
    plt.scatter(x, m, color='#0000ff')
    plt.plot([0, x_end], [m_b, m_a*x_end + m_b], color='#0fff00')
    plt.plot([0, x_end], [s_b, s_a*x_end + s_b], color='#00fff0')
    plt.legend(["SIGNAL", "MACD", "aproksymacja MACD", "aproksymacja SIGNAL"])
