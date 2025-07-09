import numpy as np
import matplotlib.pyplot as plt

def plot_solution(lx,ly,nx,ny,u, output='./u_xy.png'):
    x = np.linspace(0, lx, nx)
    y = np.linspace(0, ly, ny)
    X, Y = np.meshgrid(x, y)
    fig,ax=plt.subplots(figsize=(7, 6/lx*ly))
    ax.set_aspect('equal')
    a = ax.contourf(X, Y, u, cmap='seismic', levels=50)
    cax = fig.add_axes([ax.get_position().x1+0.02,ax.get_position().y0,0.03,ax.get_position().height])
    cbar = fig.colorbar(a, cax=cax)
    ax.set_xlabel('$x$')
    ax.set_ylabel('$y$')
    ax.set_title(f'$u(x,y)$')
    ax.set_xlim(0, lx)
    ax.set_ylim(0, ly)
    fig.savefig(output, bbox_inches='tight', dpi=300)
    plt.show()

def plot_solution_line(lx,ly,nx,ny,u, x = None, y = None, output='./u_line.png'):
    if x != None:
        x_list = np.linspace(0, lx, nx)
        y_list = np.linspace(0, ly, ny)
        index = (np.abs(x_list - x)).argmin()
        u_line=u.reshape([ny, nx])[:,index]
        fig,ax=plt.subplots(figsize=(6, 6))
        ax.plot(y_list,u_line)
        ax.set_xlabel('$y$')
        ax.set_ylabel('$u(y)$')
        ax.set_title(f'$u(x,y)$ at lx={x:.2f}')
        fig.savefig(output, bbox_inches='tight', dpi=300)
        plt.show()
    elif y != None:
        x_list = np.linspace(0, lx, nx)
        y_list = np.linspace(0, ly, ny)
        index = (np.abs(y_list - y)).argmin()
        u_line=u.reshape([ny, nx])[index,:]
        fig,ax=plt.subplots(figsize=(6, 6))
        ax.plot(x_list,u_line)
        ax.set_xlabel('$x$')
        ax.set_ylabel('$u(x)$')
        ax.set_title(f'$u(x,y)$ at ly={y:.2f}')
        fig.savefig(output, bbox_inches='tight', dpi=300)
        plt.show()
    else:
        print(f'x or y are not set.')

if __name__ == '__main__':
    path='../out/build/x86-release/bin'
    filename=f'{path}/u_sol_mat.csv'
    with open(filename, 'r') as file:
        first_line = file.readline()
        lx, ly, nx, ny = map(float, first_line.split())
    u_sol_mat = np.loadtxt(filename, delimiter=' ', skiprows=1)
    nx, ny = int(nx), int(ny)
    plot_solution(lx,ly,nx,ny,u_sol_mat,output=f'{path}/u_num.png')
    plot_solution_line(lx,ly,nx,ny,u_sol_mat, y=.5,output=f'{path}/u_line.png')

    try:
        filename_anal=f'{path}/u_anal.csv'
        u_anal = np.loadtxt(filename_anal, delimiter=' ', skiprows=1)
        plot_solution(lx,ly,nx,ny,u_anal,output=f'{path}/u_anal.png')

        plot_solution(lx,ly,nx,ny,u_sol_mat-u_anal,output=f'{path}/u_diff.png')
    except:
        print('No analytical solution found.')