import matplotlib.pyplot as plt
import numpy as np

def getData(source):
    
    data = np.loadtxt(source)
    for i in range(len(data)):
        data[i, 1] *= -1


    if len(data[0]) == 4:

        print(source + " is a jet file")
        x = data[:, 2]
        CA = [data[i, 1] + data[i, 2] for i in range(len(data))]
        CD = data[:, 1]

        return x, CD, CA

    else:
        print(source + " is NOT a jet file")
        step = data[:, 0]
        CD = data[:, 1]

        return step, CD


#step, CDnoJet = getData("noJet.txt")
x, CD, CA = getData("Jet0p15.txt")

#CDMean =  np.mean(CDnoJet)
#print("Average CD = ", CDMean)

plt.figure(figsize=(10,8))
plt.plot(x, CA, color = "black")
plt.plot(x, CD, color = "grey")
plt.plot([0, 3], [0.5, 3.5], 'k--', alpha = 0.4)
#plt.plot([0, 3], [CDMean, CDMean], 'k--', alpha = 0.6)

plt.xlabel(r"$C_T$")
plt.ylabel("Coefficient value")
plt.grid()
plt.legend(["Axial Force", "Drag"])

plt.show()