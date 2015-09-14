#! /usr/local/bin/python


def flow():
    # Tin = 20  # deg C
    Q = 50    # W
    C = 4.18  # J/(gm*degC)

    for i in range(5, 100, 5):
        deltaT = i/100.0
        M = Q / (C*deltaT)
        M = M * 1.2  # Convert gm/s (cc/s) to gal/hour
        print("deltaT = {0:<6} M = {1}".format(deltaT, M))


def peltier():
    Qmax = 50.0  # W
    Vmax = 15.0  # V
    Imax = 5.8   # A
    Rp = Vmax / Imax  # Ohm
    P = (Qmax + Imax * Imax * Rp/2)/Imax
    print("Qmax (W)    = {}".format(Qmax))
    print("Vmax (V)    = {}".format(Vmax))
    print("Imax (V)    = {}".format(Imax))
    print("Rp   (Ohm)  = {}".format(Rp))
    print("Peltier (P) = {}".format(P))

if __name__ == "__main__":
    peltier()
    print('\n')
    flow()
