import argparse

from App import App
import matplotlib.pyplot as plt


def run_cli():
    a = App()
    a.run_cli()


def run_experiment():
    a = App()
    phases_t = []
    phases_r = []
    ops_t = []
    ops_r = []
    i_range = [2**i for i in range(3, 15)]

    for i in i_range:
        data = a.get_stats(i)
        phases_r.append(data['phases']['real'])
        phases_t.append(data['phases']['theoretical'])
        ops_r.append(data['disk operations']['real'])
        ops_t.append(data['disk operations']['theoretical'])

    fig = plt.figure()
    plt.subplots_adjust(hspace=0.4)
    ax0 = fig.add_subplot(211)
    ax1 = fig.add_subplot(212)
    ax0.plot(i_range, phases_r, label="real", c='b')
    ax0.plot(i_range, phases_t, label="theoretical", c='r')
    ax1.plot(i_range, ops_r, label="real", c='b')
    ax1.plot(i_range, ops_t, label="theoretical", c='r')
    ax0.legend(loc='lower right')
    ax0.title.set_text("Phases")
    ax1.legend(loc='lower right')
    ax1.title.set_text("Disk operations")
    ax0.set_xscale("log")
    ax0.set_yscale("log")
    ax1.set_xscale("log")
    ax1.set_yscale("log")
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', '--experiment',
                        action='store_true')
    args = parser.parse_args()
    if args.experiment:
        run_experiment()
    else:
        run_cli()


