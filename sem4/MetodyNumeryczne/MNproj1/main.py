import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def weight_matrix(point, x, tau):
    n = x.shape[0]
    ret = np.eye(n)
    for i in range(n):
        ret[i, i] = np.exp(((x[i] - point).dot(np.transpose(x[i] - point))) / (-2 * tau * tau))
    return ret


def predict(y, tau):
    n = y.shape[0]
    x = np.arange(0, n)
    point = n

    x_a = np.append(np.ones(n).reshape(n, 1), x.reshape(n, 1), axis=1)
    point_a = np.array([1, point])
    w = weight_matrix(point_a, x_a, tau)
    a = np.linalg.inv(np.transpose(x_a).dot(w.dot(x_a)))
    b = (np.transpose(x_a).dot(w.dot(y)))
    theta = a.dot(b)

    ret = 0
    t = theta[1]
    if t > 0:
        ret = t / (1 + t)
    elif t < 0:
        ret = -t / (1 - t)

    return ret




def buy(actions, money, rate, price):
    amount = int(rate * money / price)
    return actions + amount, money - amount * price


def sell(actions, money, rate, price):
    amount = int(actions * rate)
    return actions - amount, money + price * amount


def main():
    column = -1
    data_dir = 'C:/Users/PCu/Downloads/data.csv'
    data = pd.read_csv(data_dir)
    data_vector = data.iloc[:, column].to_numpy()
    macd_arr = np.empty(0)
    signal_arr = np.empty(0)

    current_macd = 0
    current_signal = 0

    prev_dif = 0
    actions = 1000
    money = 0
    start_value = money + actions * data_vector[0]

    for i in range(26, 35):
        current_macd = MACD(data_vector[:i])
        macd_arr = np.append(macd_arr, current_macd)

    signal_arr = np.append(signal_arr, SIGNAL(macd_arr))
    fig, ax1 = plt.subplots()
    for i in range(35, len(data_vector)):
        current_macd = MACD(data_vector[:i])
        macd_arr = np.append(macd_arr, current_macd)
        current_signal = SIGNAL(macd_arr)
        signal_arr = np.append(signal_arr, current_signal)

        ax1.plot([i - 1, i], [signal_arr[-1], signal_arr[-2]], color='b', label='SIGNAL', zorder=1)
        ax1.plot([i - 1, i], [macd_arr[-1], macd_arr[-2]], color='r', label='MACD', zorder=1)
        """
        dif = current_signal - current_macd
        if np.sign(dif) != np.sign(prev_dif):
            print(f"BEFORE : actions: {actions}, money: {money}, value : {money + actions * data_vector[i]}")
            if dif - prev_dif < 0:
                # buy
                actions, money = buy(actions, money, predict(data_vector[:i], 11), data_vector[i])
            else:
                # sell
                actions, money = sell(actions, money, predict(data_vector[:i], 11), data_vector[i])
            print(f"AFTER: actions: {actions}, money: {money}, value : {money + actions * data_vector[i]}")
        prev_dif = dif

    end_value = money + actions * data_vector[len(data_vector) - 1]
    print(f"start value: {start_value}, end value : {end_value}, difference: {end_value - start_value}")
"""

    handles, labels = plt.gca().get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    plt.legend(by_label.values(), by_label.keys(), bbox_to_anchor=(0.1, 1.15))


    axes = fig.gca()
    (y_min, y_max) = axes.get_ylim()


    ax1.bar(np.arange(0, len(data_vector)), data_vector * (abs(y_max) + abs(y_min))/max(data_vector),
            bottom=y_min, width=1, zorder=0, color='#80ede2')

    plt.show()


def EMA(v):
    n = len(v)
    a = 2 / (n + 1)
    numerator = 0
    denominator = 0
    for i in range(n):
        numerator += v[i] * ((1 - a) ** i)
        denominator += (1 - a) ** i
    return numerator / denominator


def MACD(v):
    ema_12 = EMA(v[-12:])
    ema_26 = EMA(v[-26:])
    return ema_12 - ema_26


def SIGNAL(macd):
    return EMA(macd[-9:])


if __name__ == "__main__":
    main()


