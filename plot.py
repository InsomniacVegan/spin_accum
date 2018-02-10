# Require numpy, pyplot, argparse
import numpy as np
import matplotlib.pyplot as plt
import argparse as ap
import glob
import warnings
warnings.filterwarnings("ignore")

parser = ap.ArgumentParser(description='Spin-accumulation plotter')
parser.add_argument('-f', metavar='f', type=str, help='Folder path')

args = parser.parse_args()



# System plotting

sys_titles = [r'Equilibrium spin accumulation, m$_\infty$', r'Spin polarization of the conductivity, $\beta$', r'Spin polarization of the diffusivity, $\beta\prime$', r'Diffusivity, D$_0$', r'Precession length, $\lambda_J$', r'Dephasing length, $\lambda_\phi$', r'Spin-flip length, $\lambda_{sf}$' ]

# Read data
sys_data = np.loadtxt(args.f+'system.dat', unpack=True)

# Begin figure
sys_fig = plt.figure()
sys_fig.suptitle('System plot')

for i in range(1, len(sys_data)):
    plt_sub = sys_fig.add_subplot(420+i)
    plt_sub.plot(sys_data[0], sys_data[i])
    plt_sub.axis([np.min([0, np.min(sys_data[0])]), np.max(sys_data[0]), np.min([0, np.min(sys_data[i])]), 1.1*np.max(sys_data[i])])
    plt_sub.set_title(sys_titles[i-1])
    plt_sub.set_xlabel('Position, x')
    plt_sub.set_ylabel(sys_titles[i-1].split(', ')[1])

sys_fig.set_size_inches(20,10)
sys_fig.tight_layout()
sys_fig.savefig(args.f+'system.png')
plt.close(sys_fig)

# Data plot
data_titles = ['Spin current, j$_{mx}$','Spin current, j$_{my}$', 'Spin current, j$_{mz}$', 'Spin accumulation, m$_x$', 'Spin accumulation, m$_y$', 'Spin accumulation, m$_z$']


# Plot first 10 results
# for i in range(0, 1001, 100):
#     data = np.loadtxt(str(i)+'.dat', unpack=True)
#     data_fig = plt.figure()


#     for j in range(1,len(data)):
#         plt_sub = data_fig.add_subplot(230+j)
#         plt_sub.plot(data[0], data[j])
#         plt_sub.axis([np.min(data[0]), np.max(data[0]), np.min([0, np.min(data[j])]), 1.1*np.max(data[j])])
#         plt_sub.set_title(data_titles[j-1])
#         plt_sub.set_xlabel('Position, x')
#         plt_sub.set_ylabel(data_titles[j-1].split(', ')[1])

#     data_fig.set_size_inches(20,10)
#     data_fig.tight_layout()
#     data_fig.savefig(str(i)+'.png')

for filename in glob.iglob(args.f+'[0-9]*.dat'):
    print('Plotting: ', filename)
    data = np.loadtxt(filename, unpack=True)
    data_fig = plt.figure()
    for j in range(1,len(data)):
        plt_sub = data_fig.add_subplot(230+j)
        plt_sub.plot(data[0], data[j])
        plt_sub.axis([np.min(data[0]), np.max(data[0]), np.min([0, np.min(data[j])]), 1.1*np.max(data[j])])
        plt_sub.set_title(data_titles[j-1])
        plt_sub.set_xlabel('Position, x')
        plt_sub.set_ylabel(data_titles[j-1].split(', ')[1])

    data_fig.set_size_inches(20,10)
    data_fig.tight_layout()
    data_fig.savefig(filename.replace('.dat', '')+'.png')
    plt.close(data_fig)
