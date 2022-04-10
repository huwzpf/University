import pandas as pd
from LocallyWeigthedLinearRegression import *


def predict(macd, signal, dist, tau, plot=False, ax=None, point=34, s_color='g', m_color='g', pred_color='g'):
    width = min(len(macd), len(signal))
    if width > 100:
        width = 100
    m = macd[-width:]
    s = signal[-width:]

    m_b, m_a = fit(m, tau)
    s_b, s_a = fit(s, tau)
    if s_a == m_a:
        return "pass"
    cross = (m_b - s_b)/(s_a - m_a)
    if cross >= width and cross - width <= dist:
        if plot:
            # rysowanie przybliżonych linii aproksymujacych MACD i SIGNAL oraz przewidzianego punktu przecięcia
            plot_prediction(m, s, m_a, m_b, s_a, s_b, dist)
            point_y = point - 34
            f_y_s_1 = s_a * point_y + s_b
            f_y_s_2 = s_a * (point_y + cross - width + 1) + s_b
            f_y_m_1 = m_a * point_y + m_b
            f_y_m_2 = m_a * (point_y+(cross - width) + 1) + m_b
            ax.plot([point, point+(cross - width) + 1], [f_y_s_1, f_y_s_2], color=s_color, zorder=100)
            ax.plot([point, point +(cross - width) + 1], [f_y_m_1, f_y_m_2], color=m_color, zorder=100)
            ax.vlines(point+(cross - width) + 1, -1000000, 1000000, colors=pred_color, zorder=1,
                      label="przewidziane przecięcie")
        if abs(s_a) > abs(m_a):
            return "sell"
        else:
            return "buy"
    else:
        return "pass"


def buy(actions, money, rate, price):
    amount = int(rate * money / price)
    return actions + amount, money - amount * price


def sell(actions, money, rate, price):
    amount = int(actions * rate)
    return actions - amount, money + price * amount


def main(data, sim_type="advanced", dist=1.5, tau=0.8, amount=1):
    macd_arr = np.empty(0)
    signal_arr = np.empty(0)

    if sim_type == "basic":
        prev_dif = 0
    else:
        last_action = "none"

    if amount > 1:
        amount = 1

    actions = 1000
    money = 0
    start_value = money + actions * data_vector[0]

    for i in range(26, 35):
        macd_arr = np.append(macd_arr, MACD(data[:i]))

    signal_arr = np.append(signal_arr, SIGNAL(macd_arr))

    # podział wykresu na dwie oddzielne powierzchnie, na jednej wskaźniki na drugiej dane
    fig, ax = plt.subplots(2, sharex=True)
    plt.xlabel("dni")
    ax[1].set_ylabel("cena akcji")
    ax[0].set_ylabel("wartość wskaźników")
    axes_1 = fig.gca()
    # narysowanie wykresu danych wejściowych
    ax[1].bar(np.arange(0, len(data)), data - min(data), width=1, color='#80ede2', bottom=min(data), zorder=2)
    # zapisujemy wartości do późniejszego użycia
    y_min_1, y_max_1 = axes_1.get_ylim()

    for i in range(35, len(data)):
        macd_arr = np.append(macd_arr,  MACD(data[:i]))
        signal_arr = np.append(signal_arr, SIGNAL(macd_arr))

        ax[0].plot([i - 1, i], [signal_arr[-2], signal_arr[-1]], color='#ffcc00', label='SIGNAL', zorder=2)
        ax[0].plot([i - 1, i], [macd_arr[-2], macd_arr[-1]], color='#ccccff', label='MACD', zorder=2)

        if sim_type == "basic":
            prev_dif, actions, money = basicSim(actions, amount, ax, i, macd_arr, money, prev_dif, signal_arr, y_max_1,
                                                y_min_1)
        else:
            last_action, actions, money = advancedSim(actions, amount, ax, dist, i, last_action, macd_arr, money,
                                                      signal_arr, tau, y_max_1, y_min_1)


    end_value = money + actions * data_vector[len(data_vector) - 1]
    print(f"wartość startowa: {start_value}, wartosć końcowa : {end_value}, różnica: {end_value - start_value} "
          f"( {100 * end_value /start_value}  % wartości startowej)")
    y_min_0 = min(min(macd_arr), min(signal_arr))
    y_max_0 = max(max(macd_arr), max(signal_arr))
    ax[0].set_ylim([y_min_0, y_max_0])

    # usunięcie powtarzających się etykiet i wyświetlenie legend dla obu wykresów
    handles, labels = ax[1].get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax[1].legend(by_label.values(), by_label.keys(), loc='lower left')
    handles, labels = ax[0].get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax[0].legend(by_label.values(), by_label.keys(), loc='lower left')

    fig.tight_layout()
    plt.show()


def basicSim(actions, amount, ax, i, macd_arr, money, prev_dif, signal_arr, y_max_1, y_min_1):
    dif = signal_arr[-1] - macd_arr[-1]
    if np.sign(dif) != np.sign(prev_dif) and i > 35:
        if dif - prev_dif > 0:
            ax[1].vlines(i, y_min_1, y_max_1, colors='b', zorder=1, label="sell")
            actions, money = sell(actions, money, amount, data_vector[i])
        else:
            ax[1].vlines(i, y_min_1, y_max_1, colors='r', zorder=1, label="buy")
            actions, money = buy(actions, money, amount, data_vector[i])
    return dif, actions, money


def advancedSim(actions, amount, ax, dist, i, last_action, macd_arr, money, signal_arr, tau, y_max_1, y_min_1):
    what = predict(macd_arr, signal_arr, dist, tau, plot=False, ax=ax[0], point=i)
    if last_action != "buy" and what == "buy":
        ax[1].vlines(i, y_min_1, y_max_1, colors='r', zorder=1, label="buy")
        ax[0].vlines(i, -1000000, 1000000, colors='r', zorder=1, label="buy")
        actions, money = buy(actions, money, amount, data_vector[i])
        last_action = "buy"

    elif last_action != "sell" and what == "sell":
        ax[1].vlines(i, y_min_1, y_max_1, colors='b', zorder=1, label="sell")
        ax[0].vlines(i, -1000000, 1000000, colors='b', zorder=1, label="sell")
        actions, money = sell(actions, money, amount, data_vector[i])
        last_action = "sell"
    return last_action, actions, money


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
    column = -2
    data_dir = 'wig20.csv'
    data = pd.read_csv(data_dir)
    data_vector = data.iloc[:, column].to_numpy()
    main(data_vector[:1000], sim_type="advanced")
