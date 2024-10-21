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

int main() {
    crow::SimpleApp app;  // Define Crow web application

    CROW_ROUTE(app, "/")([](){
        return "Welcome to the Data Analysis Web App!";
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        // Handle CSV upload (for simplicity, this assumes a file upload is handled here)
        std::string filename = "uploaded_data.csv";
        std::ofstream file(filename);
        file << req.body;
        file.close();

        arma::mat data = load_csv(filename);

        // Compute missing values and statistics
        std::map<int, int> missing_values = count_missing_values(data);
        std::map<std::string, arma::vec> stats = calculate_statistics(data);

        // Create response JSON
        crow::json::wvalue response;
        response["missing_values"] = crow::json::wvalue::list();
        for (const auto& [col, count] : missing_values) {
            response["missing_values"].push_back({{"column", col}, {"missing_count", count}});
        }

        response["statistics"] = crow::json::wvalue::object();
        for (const auto& [stat, values] : stats) {
            response["statistics"][stat] = crow::json::wvalue::list();
            for (double value : values) {
                response["statistics"][stat].push_back(value);
            }
        }

        return response;
    });

    CROW_ROUTE(app, "/visualize").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        // Visualize the data (assumes the histogram is requested)
        arma::mat data = load_csv("uploaded_data.csv");

        // Generate histogram for the first column as an example
        generate_histogram(data.col(0), "histogram.png");

        return crow::response(200, "Histogram generated. Check the file 'histogram.png'.");
    });
    // Specify the port below
    app.port(numport).multithreaded().run();
}
