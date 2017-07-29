#include <iostream>
#include <Eigen/Dense>
//#include <mpi.h>
#include <fstream>
#include <ntsid.h>
#include <omp.h>
#include <vector>


using std::cin;
using std::cout;
using std::endl;
using namespace Eigen;
using namespace std;

// install eigen
// mpic++ -I /Users/mustafaatik/Downloads/eigen/ main.cpp -o main && mpirun  --allow-run-as-root  -np 2 ./main
// http://stackoverflow.com/questions/15939757/is-it-possible-to-run-openmpi-on-a-local-computer-and-a-remote-cluster
// http://mpitutorial.com/tutorials/running-an-mpi-cluster-within-a-lan/
// https://www.open-mpi.org/papers/sc-2009/jjhursey-iu-booth.pdf
// http://stackoverflow.com/questions/2546298/vector-usage-in-mpic
// http://stackoverflow.com/questions/38592854/how-to-send-the-data-in-an-eigenmatrixxd-with-mpi

void mat2csv(MatrixXd mat);
void factorize();
void als(std::vector<std::vector<int>> ratings, int maxUserId, int maxMovieId, int rank, int threadNumber);

std::vector< std::vector<int> > load_csv (const std::string & path) {
    std::ifstream indata;
    indata.open(path);
    std::string line;
    std::vector<double> values;
    uint rows = 0;
    std::vector< std::vector<int> > ratings;
    while (std::getline(indata, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<int> data;
        while (std::getline(lineStream, cell, ',')) {
            data.push_back(std::stod(cell));
        }
        // user - movie -rating
        ratings.push_back(data);
    }
    return ratings;
//    return Map<const Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, RowMajor>>(values.data(), rows, values.size()/rows);
}


int main(int argc, char ** argv)
{
    factorize();
    return 0;
}



void factorize() {
    std::vector< std::vector<int> > ratings = load_csv("../data.txt");

    int maxUserId = 7;
    int maxUMovieId= 9;
    int cols = maxUMovieId, rows = maxUserId;

    for (int i = 4; i < 5; ++i) {
        cout << "Thread number: " << i << endl;
        als(ratings, maxUserId, maxUMovieId, 4, 1);
    }
}




void als(std::vector<std::vector<int>> ratings, int maxUserId, int maxMovieId, int rank, int threadNumber) {

    MatrixXd U = MatrixXd::Random(maxUserId, rank);
    MatrixXd M = MatrixXd::Random(rank, maxMovieId);

    MatrixXd A = MatrixXd::Zero(maxUserId, maxMovieId);
    MatrixXd W = MatrixXd::Zero(maxUserId, maxMovieId);
    for (int i = 0; i < ratings.size(); ++i) {
        int ui = ratings.at(i).at(0) - 1; // userId index
        int mi = ratings.at(i).at(1) - 1; // movieId index
        int ri = ratings.at(i).at(2); // rating
        A(ui, mi) = ri;
        W(ui, mi) = 1;
    }

    cout << A << endl << endl;
    double Eta = 0.03;
    double eta = Eta;
    int maxIteration = 2000;
    cout << U * M << endl << endl;

    int ui, mi, ri;
    double error;
    Eigen::VectorXd tempU;
    std::vector<double> errorHistory;

    double start = omp_get_wtime();
    for (int iteration = 0; iteration < maxIteration; ++iteration) {

        #pragma omp parallel for num_threads(threadNumber) schedule(static) private(ui, mi, ri, error, tempU)
        for (int i = 0; i < ratings.size(); ++i) {
            ui = ratings.at(i).at(0) - 1; // userId index
            mi = ratings.at(i).at(1) - 1; // movieId index
            ri = ratings.at(i).at(2); // rating
            error = ri - U.row(ui) * M.col(mi);

            tempU = U.row(ui) + eta * error * M.col(mi).transpose();
            M.col(mi) = M.col(mi) + eta * error * U.row(ui).transpose();
            U.row(ui) = tempU;
        }

        if (iteration % ((int) maxIteration / 15) == 0) {
            eta *= 0.8;
            errorHistory.push_back(W.cwiseProduct(A - U * M).array().square().sum());
        }
    }
    double elapsed = omp_get_wtime() - start;
    cout << "elapsed: " << elapsed << endl;
    cout << "error:" << endl;
    for (int j = 0; j < errorHistory.size(); ++j) {
        cout << "error: " << errorHistory.at(j) << endl;
    }
    cout << U * M << endl << endl;
//    mat2csv(A);
    mat2csv(U*M);
}


void mat2csv(MatrixXd mat) {
    std::stringstream buf;
    for (int i = 0; i < mat.rows(); ++i) {

        for (int j = 0; j < mat.cols(); ++j) {
            buf << mat(i, j) << ", ";
        }
        buf << "\n";
    }
    cout << endl << buf.str() << endl;
}