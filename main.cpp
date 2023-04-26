// Nail Minnemullin
// n.minnemullin@innopolis.university
#include <cmath>
#include "iostream"

using namespace std;

const double epsilon = 1e-10;

class Matrix {
protected:
    // Number of rows
    int n;
    // Number of columns
    int m;
    double **matrix;

public:

    // If some error occurred during multiplication, summation or subtraction
    bool wasErrorOccurred = false;


    Matrix(int n, int m) {
        this->n = n;
        this->m = m;
        // Allocate memory for the matrix
        matrix = (double **) malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++) {
            matrix[i] = (double *) malloc(m * sizeof(double));
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix[i][j] = 0.0;
            }
        }
    }

    // Copy constructor
    Matrix(Matrix &a) : Matrix(a.getN(), a.getM()) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix[i][j] = a[i][j];
            }
        }
        wasErrorOccurred = a.wasErrorOccurred;
    }

    // Destructor
    ~Matrix() {
        for (int i = 0; i < n; ++i) {
            delete matrix[i];
        }
        delete matrix;
    }

    int getN() { return n; }

    int getM() { return m; }

    double *operator[](int index) {
        return matrix[index];
    }


    friend istream &operator>>(istream &stream, Matrix &matrix1) {
        for (int i = 0; i < matrix1.getN(); ++i) {
            for (int j = 0; j < matrix1.getM(); ++j) {
                stream >> matrix1[i][j];
            }
        }
        return stream;

    }

    friend ostream &operator<<(ostream &stream, Matrix &matrix1) {

        for (int i = 0; i < matrix1.n; ++i) {
            for (int j = 0; j < matrix1.m; ++j) {
                if (fabs(matrix1[i][j]) < epsilon) matrix1[i][j] = 0.0;
                stream << matrix1[i][j] << " ";
            }
            stream << endl;
        }
        return stream;
    }

    Matrix operator+(Matrix &input) {
        Matrix resMatrix(n, m);
        if (this->n == input.n && this->m == input.m) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    resMatrix[i][j] = input[i][j] + this->matrix[i][j];
                }
            }
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
            resMatrix.wasErrorOccurred = true;
        }
        return resMatrix;
    }

    Matrix operator-(Matrix &input) {
        Matrix resMatrix(n, m);
        if (this->n == input.n && this->m == input.m) {

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    resMatrix[i][j] = this->matrix[i][j] - input[i][j];
                }
            }
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
            resMatrix.wasErrorOccurred = true;

        }
        return resMatrix;
    }

    Matrix &operator=(const Matrix &input) {
        this->matrix = input.matrix;
        this->wasErrorOccurred = input.wasErrorOccurred;
        this->m = input.m;
        this->n = input.n;
        return *this;
    }


    Matrix &operator*(double c) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                matrix[i][j] = matrix[i][j] * c;

            }
        }
        return *this;
    }

    Matrix &transpose() {
        auto *newMatrix = new Matrix(m, n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                (*newMatrix)[j][i] = matrix[i][j];
            }
        }

        return *newMatrix;

    }

    // Return the index of row (below current row) with maximum absolute value in current column

    int findRowNumberWithMaximumAbsolutValueInColumnBelow(int column) {
        double maxValue = matrix[column][column];
        int resRow = column;
        for (int row = column + 1; row < n; ++row) {
            if (abs(matrix[row][column]) > abs(maxValue)) {
                maxValue = abs(matrix[row][column]);
                resRow = row;
            }
        }
        return resRow;
    }

    Matrix &operator*(Matrix &secondMatrix) {
        auto resMatrix = new Matrix{n, secondMatrix.m};
        if (m == secondMatrix.n) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < secondMatrix.m; ++j) {
                    double res = 0.0;
                    for (int k = 0; k < m; ++k) {
                        res = res + matrix[i][k] * secondMatrix[k][j];

                    }
                    (*resMatrix)[i][j] = res;

                }
            }
        } else {
            cout << "Error: the dimensional problem occurred" << endl;
            resMatrix->wasErrorOccurred = true;
        }
        return *resMatrix;
    }


};


class SquareMatrix : public Matrix {
public:
    SquareMatrix(int n) : Matrix(n, n) {}

    SquareMatrix(Matrix &a) : Matrix(a) {

    }

    int getSize() {
        return n;
    }

    using Matrix::operator=;
};

class IdentityMatrix : public SquareMatrix {
public:
    IdentityMatrix(int n) : SquareMatrix(n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) matrix[i][j] = 1.0;
                else matrix[i][j] = 0.0;
            }
        }
    }

    using Matrix::operator=;

};

class EliminationMatrix : public IdentityMatrix {
public:
    EliminationMatrix(SquareMatrix &a, int i, int j) : IdentityMatrix(a.getSize()) {
        matrix[i][j] = -a[i][j] / a[j][j];

    }
};

class PermutationMatrix : public IdentityMatrix {
public:
    PermutationMatrix(int n, int row1, int row2) : IdentityMatrix(n) {

        swap(matrix[row1], matrix[row2]);

    }
};

class ColumnVector : public SquareMatrix {
public:
    ColumnVector(int n) : SquareMatrix(n) {

    }

    ColumnVector(Matrix &a) : SquareMatrix(a) {

    }

    // Returns the length of a ColumnVector
    double computeNorm() {
        double res = 0.0;
        for (int i = 0; i < n; ++i) {
            res += (*this)[i] + (*this)[i];
        }
        return ::sqrt(res);
    }

    // Overloaded operators
    double &operator[](int index) {
        return matrix[index][0];
    }

    using Matrix::operator=;
    using Matrix::operator*;
    using Matrix::operator+;

};


istream &operator>>(istream &stream, ColumnVector &columnVector) {
    for (int i = 0; i < columnVector.getN(); ++i) {
        stream >> columnVector[i];
    }
    return stream;
}

ostream &operator<<(ostream &stream, ColumnVector &columnVector) {
    for (int i = 0; i < columnVector.getN(); ++i) {
        if (fabs(columnVector[i]) < epsilon) columnVector[i] = 0.0;
        stream << columnVector[i] << endl;
    }
    return stream;
}


class Utils {
public:
    static SquareMatrix &calculateInverse(SquareMatrix &a) {
        int n = a.getSize();
        static IdentityMatrix augmentedMatrix{n};

        int step = 0;
        static SquareMatrix res{a};
        bool isDirectWay = true;
        bool isWayBack = false;

        for (int column = 0; column < n; ++column) {
            for (int row = 0; row < n; ++row) {
                // Direct way

                if (column < row) {

                    isDirectWay = false;
                    int rowNumberWithMaximumValue = res.findRowNumberWithMaximumAbsolutValueInColumnBelow(column);
                    if (rowNumberWithMaximumValue != column) {
                        PermutationMatrix p{n, rowNumberWithMaximumValue, column};
                        res = p * res;
                        augmentedMatrix = p * augmentedMatrix;
                    }
                    if (res[row][column] != 0.0) {
                        EliminationMatrix e1{res, row, column};
                        EliminationMatrix e2{res, row, column};
                        res = e1 * res;
                        augmentedMatrix = e2 * augmentedMatrix;
                    }
                }

            }

        }
        for (int row = n - 1; row >= 0; --row) {
            for (int column = n - 1; column >= 0; --column) {
                // Direct way

                if (column < row) {


                    int rowNumberWithMaximumValue = res.findRowNumberWithMaximumAbsolutValueInColumnBelow(column);
                    if (rowNumberWithMaximumValue != column) {
                        PermutationMatrix p{n, rowNumberWithMaximumValue, column};
                        res = p * res;
                        augmentedMatrix = p * augmentedMatrix;
                    }
                    if (res[column][row] != 0.0) {
                        EliminationMatrix e1{res, column, row};
                        EliminationMatrix e2{res, column, row};
                        res = e1 * res;
                        augmentedMatrix = e2 * augmentedMatrix;
                    }
                }

            }

        }
        // Normalization
        for (int column = 0; column < n; ++column) {
            for (int row = 0; row < n; ++row) {
                augmentedMatrix[column][row] = augmentedMatrix[column][row] / res[column][column];

            }
            res[column][column] = 1.0;


        }
        return augmentedMatrix;
    }


    static void leastSquares(int n, int degree, ColumnVector &t, ColumnVector &b) {
        Matrix a{n, degree + 1};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < degree + 1; ++j) {
                a[i][j] = ::pow(t[i], j);

            }
        }
        cout << "A:\n" << a;
        Matrix a_t = a.transpose();
        SquareMatrix a_ta = a_t * a;
        cout << "A_T*A:\n" << a_ta;
        Matrix inversed = calculateInverse(a_ta);
        cout << "(A_T*A)^-1:\n" << inversed;
        ColumnVector a_tb = a_t * b;
        cout << "A_T*b:\n" << a_tb;
        ColumnVector x = inversed * a_tb;
        cout << "x~:\n" << x;
        drawGraph(x, "temp.txt");
    }

    static void printPointsToFile(ColumnVector &t, ColumnVector &b, char *name) {
        static FILE *tempDataFile;
        tempDataFile = ::fopen(name, "w");
        for (int i = 0; i < t.getN(); ++i) {
            ::fprintf(tempDataFile, "%f %f\n", t[i], b[i]);
        }
        ::fclose(tempDataFile);
        fflush(tempDataFile);
    }

    static void drawGraph(ColumnVector &x, char *tempDataFileName) {
        static FILE *pipe;
        pipe = popen("gnuplot -persistent", "w");
        ::fprintf(pipe, "set xrange [-10:10]; set yrange [-10:20];\n");


        string function;
        for (int i = x.getN() - 1; i >= 0; --i) {
            function = function + to_string(x[i]) + "*x**" + to_string(i);
            if (i > 0) function = function + " + ";
        }
        cout << function;
        ::fprintf(pipe, "plot '%s',%s\n", tempDataFileName, function.c_str());
        pclose(pipe);
    }
};


int main() {
    cout << std::fixed;
    cout.precision(4);
    int n, degree;
    cin >> n;
    ColumnVector t{n};
    ColumnVector b{n};


    for (int i = 0; i < n; ++i) {
        cin >> t[i] >> b[i];
    }

    cin >> degree;
    Utils::printPointsToFile(t, b, "temp.txt");

    Utils::leastSquares(n, degree, t, b);
}
