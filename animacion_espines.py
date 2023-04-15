import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Set the name of the file to read
filename = "Espines.dat"
N=5

# Load the entire file as a string
with open(filename, 'r') as f:
    file_str = f.read()

# Split the file string by extra enter to get a list of collections
collection_list = file_str.split('\n\n')

# If there is only one collection, plot it as a static figure
if len(collection_list) == 1:
    # Load the data from the file into a numpy array
    data = np.fromstring(collection_list[0], sep=' ').reshape((N, N))

    # Create a color map for the dots
    colors = {1: 'blue', -1: 'red'}

    # Create a figure and axis object
    fig, ax = plt.subplots()

    # Set the axis limits and ticks
    ax.set_xlim([-0.5, N-0.5])
    ax.set_ylim([-0.5, N-0.5])
    ax.set_xticks(np.arange(N))
    ax.set_yticks(np.arange(N))
    ax.set_aspect('equal', adjustable='box')

    # Loop over each element in the data array and plot a dot
    for i in range(N):
        for j in range(N):
            dot_color = colors[data[i,j]]
            ax.scatter(j, i, c=dot_color, s=100)

    # Save the plot to a file
    plt.savefig('red.pdf')

# If there are multiple collections, create an animation
else:
    # Create a color map for the dots
    colors = {1: 'blue', -1: 'red'}

    # Create a figure and axis object
    fig, ax = plt.subplots()

    # Set the axis limits and ticks
    ax.set_xlim([-0.5, N-0.5])
    ax.set_ylim([-0.5, N-0.5])
    ax.set_xticks(np.arange(N))
    ax.set_yticks(np.arange(N))
    ax.set_aspect('equal', adjustable='box')

    # Create an empty scatter plot
    plot_object = ax.scatter([], [], c=[], s=100)

    # Define the update function for the animation
    def update(time_step):
        # Load the data for this time step into a numpy array
        data = np.fromstring(collection_list[time_step], sep=' ')
        data = data.reshape((N,N))

        # Loop over each element in the data array and plot a dot
        for i in range(N):
            for j in range(N):
                dot_color = colors[data[i,j]]
                ax.scatter(j, i, c=dot_color, s=100)

        # Return the updated plot object
        return plot_object,

    # Create the animation
    anim = animation.FuncAnimation(fig, update, frames=len(collection_list), interval=1000)

    # Save the animation to a file using FFMPEG
    anim.save('evolucion_red.mp4', writer='ffmpeg')   