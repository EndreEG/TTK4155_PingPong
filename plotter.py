import numpy as np
import matplotlib.pyplot as plt

# Original slider outputs
slider_output = [5, 33, 65, 100, 133, 166, 194, 225, 252, 255, 255]

def map_slider_outputs(slider_output, new_min=0, new_max=100):
	old_min = min(slider_output)
	old_max = max(slider_output)
	return [(new_max - new_min) * (x - old_min) / (old_max - old_min) + new_min for x in slider_output]

def map_and_plot_slider_outputs(slider_output):
	# Map the slider outputs
	mapped_outputs = map_slider_outputs(slider_output)
	
	# Use the original slider outputs as x values
	x = slider_output
	
	# Plot the resulting function
	plt.plot(x, mapped_outputs, marker='o')
	plt.title('Mapped Slider Outputs')
	plt.xlabel('Original Slider Outputs')
	plt.ylabel('Mapped Outputs (0 to 100)')
	plt.grid(True)
	plt.show()

# Call the function to map and plot the slider outputs
map_and_plot_slider_outputs(slider_output)