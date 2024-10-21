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

// Function to calculate missing values per column
std::map<int, int> count_missing_values(const arma::mat &data) {
    std::map<int, int> missing_values;
    for (arma::uword col = 0; col < data.n_cols; col++) {
        int missing_count = 0;
        for (arma::uword row = 0; row < data.n_rows; row++) {
            if (std::isnan(data(row, col))) {
                missing_count++;
            }
        }
        missing_values[col] = missing_count;
    }
    return missing_values;
}

// Function to calculate basic statistics for each column
std::map<std::string, arma::vec> calculate_statistics(const arma::mat &data) {
    std::map<std::string, arma::vec> stats;
    stats["mean"] = arma::mean(data, 0);
    stats["median"] = arma::median(data, 0);
    stats["stddev"] = arma::stddev(data, 0);
    return stats;
}

// Function to generate histogram using Matplot++
void generate_histogram(const arma::vec &data, const std::string &filename) {
    std::vector<double> vec(data.begin(), data.end());  // Convert arma::vec to std::vector
    plt::hist(vec, 10);  // Create histogram with 10 bins
    plt::save(filename);
}

