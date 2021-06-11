import matplotlib.pyplot as plt
import numpy as np

from scipy.signal import savgol_filter
from scipy.fft import fft, fftfreq

def getData(source):
    
    data = np.loadtxt(source)
    for i in range(len(data)):
        data[i, 1] *= -1


    if len(data[0]) == 4:
        x = data[:, 2]
        CA = [data[i, 1] + data[i, 2] for i in range(len(data))]
        CD = data[:, 1]

        return x, CD, CA, data[:, 0]

    else:
        step = data[:, 0]
        CD = data[:, 1]

        return step, CD


step, CDnoJet = getData("noJet.txt")
x, CD, CA, t = getData("Jet0p05.txt")
x0p15, CD0p15, CA0p15, t = getData("Jet0p15.txt")


_, CD_0p1, _, cd_time = getData("CT0p1.txt")
_, CD_0p3, _, _ = getData("CT0p3.txt")
_, CD_0p5, _, _ = getData("CT0p5.txt")
_, CD_1p5, _, _ = getData("CT1p5.txt")
_, CD_2p5, _, _ = getData("CT2p5.txt")

CDMean =  np.mean(CDnoJet)
print("Average CD = ", CDMean)

data = np.loadtxt("JetRamp.txt")
for i in range(len(data)):
    data[i, 1] *= -1

CT_Ramp = {"0":[], "1":[]}
CD_Ramp = {"0":[], "1":[]}
CA_Ramp = {"0":[], "1":[]}

direction = "0";
for i, row in enumerate(data):
    val_cd = row[1]
    val_ct = row[2]
    val_ca = val_cd + val_ct

    if val_ct > 0.9999:
        direction = "1"

    CT_Ramp[direction].append(val_ct)
    CD_Ramp[direction].append(val_cd)
    CA_Ramp[direction].append(val_ca)

def ctca():

    ax = plt.subplot(111)

    # plt.plot(x, CA, color = "black", linewidth = 0.8)
    # plt.plot(x, CD, color = "black", linewidth = 0.8, alpha = 0.5)

    # plt.plot(0.1, np.mean(CD_0p1) + 0.1, 'bo')
    # plt.plot(0.3, np.mean(CD_0p3) + 0.3, 'bo')
    # plt.plot(0.5, np.mean(CD_0p5) + 0.5, 'bo')
    # plt.plot(1.5, np.mean(CD_1p5) + 1.5, 'bo')
    # plt.plot(2.5, np.mean(CD_2p5) + 2.5, 'bo')


    # plt.plot(x0p15, CA0p15, color = "black", linewidth = 0.8)
    # plt.plot(x0p15, CD0p15, color = "black", linewidth = 0.8, alpha = 0.5)

    # plt.plot(x, savgol_filter(CA, 1111, 3), color = "red", linewidth = 0.8)
    # plt.plot(x, savgol_filter(CD, 1111, 3), color = "red", linewidth = 0.8, alpha = 0.5)
    # plt.plot(x, savgol_filter(CA0p15, 1111, 3), color = "blue", linewidth = 0.8)
    # plt.plot(x, savgol_filter(CD0p15, 1111, 3), color = "blue", linewidth = 0.8, alpha = 0.5)


    plt.plot(CT_Ramp["0"], CA_Ramp["0"], color = "blue", linewidth = 0.8)
    plt.plot(CT_Ramp["1"], CA_Ramp["1"], color = "red", linewidth = 0.8)
    plt.plot([0, 1], [CDMean, CDMean], 'k--', linewidth = 0.8, alpha = 0.6)
    plt.arrow(0.68, 1.6, -0.24, -0.15, head_width=0.03, head_length=0.03, fc='r', ec='r')
    plt.annotate(r"Increasing $C_T$", (0.6, 0.75))
    plt.arrow(0.49, 0.7, 0.2, 0.25, head_width=0.03, head_length=0.03, fc='b', ec='b')
    plt.annotate(r"Decreasing $C_T$", (0.35, 1.55))



    plt.annotate(r"Baseline $C_D = 1.14$", (2, 1.2))

    #plt.plot([0, 3], [0.5, 3.5], 'k--', alpha = 0.4)
    #plt.plot([0, 3], [CDMean, CDMean], 'k--', linewidth = 0.8, alpha = 0.6)



    #plt.annotate(r"Averaged $C_D$", (2, 1.2))

    plt.xlabel(r"$C_T$")
    plt.ylabel("Coefficient value")
    #plt.legend([r"$C_A$", "$C_D$"])

    plt.show()

def cdTime():

    ax = plt.subplot(111)


    cut = 2
    
    plt.plot([(i * 2e-7 * 1000) for i in cd_time[cut:]], CD_1p5[cut:], color = "black", linewidth = 0.8)


    #plt.plot([(i * 2e-7 * 1000) for i in cd_time[cut:]], savgol_filter(CD_1p5[cut:], 501, 3), color = "red", linewidth = 0.8)
    # plt.plot(x, savgol_filter(CD, 1111, 3), color = "red", linewidth = 0.8, alpha = 0.5)
    # plt.plot(x, savgol_filter(CA0p15, 1111, 3), color = "blue", linewidth = 0.8)
    # plt.plot(x, savgol_filter(CD0p15, 1111, 3), color = "blue", linewidth = 0.8, alpha = 0.5)


    #plt.plot(CT_Ramp["0"], CA_Ramp["0"], color = "blue", linewidth = 0.8)
    #plt.plot(CT_Ramp["1"], CA_Ramp["1"], color = "red", linewidth = 0.8)
    #plt.plot([0, 1], [CDMean, CDMean], 'k--', linewidth = 0.8, alpha = 0.6)
    #plt.arrow(0.68, 1.6, -0.24, -0.15, head_width=0.03, head_length=0.03, fc='r', ec='r')
    #plt.annotate(r"Increasing $C_T$", (0.6, 0.75))
    #plt.arrow(0.49, 0.7, 0.2, 0.25, head_width=0.03, head_length=0.03, fc='b', ec='b')
    #plt.annotate(r"Decreasing $C_T$", (0.35, 1.55))
    #plt.legend([r"$C_A$: Increasing", r"$C_D$: Increasing", r"$C_A$: Decreasing", r"$C_D$: Decreasing"])

    #plt.plot([0, 3], [CDMean, CDMean], 'k--', linewidth = 0.8, alpha = 0.6)

    #plt.annotate(r"Averaged $C_D$", (2, 1.2))

    plt.xlabel(r"Time (ms)")
    plt.ylabel(r"$C_D$")
    #plt.legend([r"$C_A$", "$C_D$"])

    plt.show()

ctca()