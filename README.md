# Masters-Project
This repo contains the source code for the probabilistic membership calculator and the associated tool to allow easy cutting 
of data below a certain probability.


This set of programs is designed for use with data from the Gaia satellite. The required inputs for each star are the stated
parallax, the error in this parallax, the upper and lower parallax limits that the cluster is believed to lie between. 
The membership_probability program will read in a file of these and produce a probability that each star is in the cluster.
This output file can be fed straight into the output_extractor program which will remove all data points below a specified 
probability value.

This program set makes use of the Gnu Scientific Library under the GPL, documentation for which can be found at:
https://www.gnu.org/software/gsl/doc/html/index.html
