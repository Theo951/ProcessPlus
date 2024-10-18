#include <crow.h>
#include <armadillo>
#include <matplot/matplot.h>
#include <fstream>
#include <vector>
#include <map>

namespace plt = matplotlibcpp;

// Function to load CSV data
arma::mat load_csv(const std::string &filename) {
    arma::mat data;
    data.load(filename);
    return data;
}