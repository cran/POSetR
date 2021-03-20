#ifndef matrice_hpp
#define matrice_hpp
#include <algorithm>

class Matrice {
private:
    double *dati;
    std::uint64_t rows;
    std::uint64_t cols;
public:
    Matrice(std::uint64_t rows, std::uint64_t cols) {
        this->rows = rows;
        this->cols = cols;
        dati = new double[rows * cols];
        std::fill(dati, dati + (rows * cols), 0.0);
    }
    
    ~Matrice() {
        delete[] dati;
    }
    
    double& operator()(std::uint64_t row, std::uint64_t col) {
        return dati[row * this->cols + col];
    }
    
    double at(std::uint64_t row, std::uint64_t col) {
        return dati[row * this->cols + col];
    }
    
    
};

#endif /* matrice_hpp */
