#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class Matrix {
private:
  int numRows, numColumns;
  double **matrixData;

public:
  Matrix();
  Matrix(int rowCount, int columnCount);
  Matrix(const Matrix &otherMatrix);
  ~Matrix();

  int getRowCount() const;
  int getColumnCount() const;
  double getValue(int rowIndex, int columnIndex) const;
  void setValue(int rowIndex, int columnIndex, double value);

  // Vector3D toVector3D() const;

  double calculateDeterminant() const;
  double calculateCofactor(int, int) const;
  Matrix calculateAdjoint() const;
  Matrix calculateTranspose() const;
  Matrix calculateInverse() const;
  Matrix normalizeMatrix() const;

  Matrix operator+(const Matrix &otherMatrix) const;
  Matrix operator-(const Matrix &otherMatrix) const;
  Matrix operator*(const Matrix &otherMatrix) const;
  Matrix operator*(double scalarValue) const;
  Matrix operator/(double scalarValue) const;
  Matrix operator-() const;

  Matrix &operator=(const Matrix &otherMatrix);

  bool operator==(const Matrix &otherMatrix) const;
  bool operator!=(const Matrix &otherMatrix) const;

  friend std::ostream &operator<<(std::ostream &outputStream, const Matrix &matrix);
};

Matrix::Matrix() : numRows(0), numColumns(0), matrixData(nullptr) {}
/**
 * Creates a matrix of size row x col
 * @param row number of rows
 * @param col number of columns
 * @return void
 */
Matrix::Matrix(int rowCount, int columnCount) : numRows(rowCount), numColumns(columnCount) {
  matrixData = new double *[numRows];
  
  for (int i = 0; i < numRows; i++) {
    matrixData[i] = new double[numColumns];
    for (int j = 0; j < numColumns; j++) {
      matrixData[i][j] = 0.0;
    }
  }
}

/**
 * Creates a copy of the given matrix m
 * @param m matrix of size row x col
 * @return void
 */
Matrix::Matrix(const Matrix &otherMatrix) : numRows(otherMatrix.numRows), numColumns(otherMatrix.numColumns) {
  matrixData = new double *[numRows];
  for (int i = 0; i < numRows; i++) {
    matrixData[i] = new double[numColumns];
    for (int j = 0; j < numColumns; j++) {
      matrixData[i][j] = otherMatrix.matrixData[i][j];
    }
  }
}

/**
 * destructor
 */
Matrix::~Matrix() {
  for (int i = 0; i < numRows; i++) {
    delete[] matrixData[i];
  }
  delete[] matrixData;
}

/**
 * Returns the number of rows in the matrix
 * @return number of rows
 */
int Matrix::getRowCount() const { return numRows; }

int Matrix::getColumnCount() const { return numColumns; }

double Matrix::getValue(int rowIndex, int columnIndex) const { return matrixData[rowIndex][columnIndex]; }

void Matrix::setValue(int rowIndex, int columnIndex, double value) { matrixData[rowIndex][columnIndex] = value; }

/**
 * Adds two matrices
 * @param m matrix to be added
 * @return sum of the two matrices
 */
Matrix Matrix::operator+(const Matrix &otherMatrix) const {
  if (numRows != otherMatrix.numRows || numColumns != otherMatrix.numColumns) {
    throw "Matrix dimensions do not match";
  }

  Matrix result(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      result.matrixData[i][j] = matrixData[i][j] + otherMatrix.matrixData[i][j];
    }
  }
  return result;
}

/**
 * Subtracts two matrices
 * @param m matrix to be subtracted
 * @return difference of the two matrices
 */
Matrix Matrix::operator-(const Matrix &otherMatrix) const {
  if (numRows != otherMatrix.numRows || numColumns != otherMatrix.numColumns) {
    throw "Matrix dimensions do not match";
  }

  Matrix result(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      result.matrixData[i][j] = matrixData[i][j] - otherMatrix.matrixData[i][j];
    }
  }
  return result;
}

/**
 * Multiplies two matrices
 * @param m matrix to be multiplied
 * @return product of the two matrices
 */
Matrix Matrix::operator*(const Matrix &otherMatrix) const {
  if (numColumns != otherMatrix.numRows) {
    throw "Matrices of dimensions " + std::to_string(numRows) + "x" +
        std::to_string(numColumns) + " and " + std::to_string(otherMatrix.numRows) + "x" +
        std::to_string(otherMatrix.numColumns) + " cannot be multiplied";
  }

  Matrix result(numRows, otherMatrix.numColumns);

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < otherMatrix.numColumns; j++) {
      for (int k = 0; k < numColumns; k++) {
        result.matrixData[i][j] += matrixData[i][k] * otherMatrix.matrixData[k][j];
      }
    }
  }

  return result;
}

/**
 * Multiplies a matrix with a scalar
 * @param val scalar to be multiplied
 * @return product of the matrix and the scalar
 */
Matrix Matrix::operator*(double scalarValue) const {
  Matrix result(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      result.matrixData[i][j] = matrixData[i][j] * scalarValue;
    }
  }
  return result;
}

/**
 * Divides a matrix with a scalar
 * @param val scalar to be divided
 * @return quotient of the matrix and the scalar
 */
Matrix Matrix::operator/(double scalarValue) const {
  if (scalarValue == 0.0) {
    throw "Division by zero";
  }
  const double inverseValue = 1.0 / scalarValue;
  return *this * inverseValue;
}

/**
 * Assigns a matrix to another matrix
 * @param m matrix to be assigned
 * @return void
 */
Matrix &Matrix::operator=(const Matrix &otherMatrix) {
  if (this == &otherMatrix) {
    return *this;
  }

  if (numRows != otherMatrix.numRows || numColumns != otherMatrix.numColumns) {
    for (int i = 0; i < numRows; i++) {
      delete[] matrixData[i];
    }
    delete[] matrixData;

    numRows = otherMatrix.numRows;
    numColumns = otherMatrix.numColumns;
    matrixData = new double *[numRows];
  }

  for (int i = 0; i < numRows; i++) {
    matrixData[i] = new double[numColumns];
    for (int j = 0; j < numColumns; j++) {
      matrixData[i][j] = otherMatrix.matrixData[i][j];
    }
  }

  return *this;
}

/**
 * Scalar multiplication with -1
 * @return Matrix
 */
Matrix Matrix::operator-() const {
  Matrix zeroMatrix(numRows, numColumns);
  return zeroMatrix - *this;
}

/**
 * Checks if two matrices are equal
 * @param m matrix to be compared
 * @return true if equal, false otherwise
 */
bool Matrix::operator==(const Matrix &otherMatrix) const {
  if (numRows != otherMatrix.numRows || numColumns != otherMatrix.numColumns) {
    return false;
  }

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      if (matrixData[i][j] != otherMatrix.matrixData[i][j]) {
        return false;
      }
    }
  }
  return true;
}

/**
 * Checks if two matrices are not equal
 * @param m matrix to be compared
 * @return true if not equal, false otherwise
 */
bool Matrix::operator!=(const Matrix &otherMatrix) const { 
  return !(*this == otherMatrix); 
}

/**
 * Outputs the matrix in the form of a string
 * @return string representation of the matrix
 */
std::ostream &operator<<(std::ostream &outputStream, const Matrix &matrix) {
  for (int i = 0; i < matrix.numRows; i++) {
    for (int j = 0; j < matrix.numColumns; j++) {
      outputStream << matrix.matrixData[i][j] << " ";
    }
    outputStream << std::endl;
  }
  return outputStream;
}

/**
 * Returns the cofactor of the matrix at the given index
 * @param i row index
 * @param j column index
 * @return cofactor of the matrix at index (i, j)
 */
double Matrix::calculateCofactor(int i, int j) const {
  if (numRows != numColumns) {
    throw "Matrix is not square";
  }

  Matrix subMatrix(numRows - 1, numColumns - 1);
  int rowIdx = 0, colIdx = 0;
  for (int k = 0; k < numRows; k++) {
    if (k == i) {
      continue;
    }
    colIdx = 0;
    for (int l = 0; l < numColumns; l++) {
      if (l == j) {
        continue;
      }
      subMatrix.matrixData[rowIdx][colIdx] = matrixData[k][l];
      colIdx++;
    }
    rowIdx++;
  }

  double signValue = (i + j) % 2 == 0 ? 1.0 : -1.0;
  return signValue * subMatrix.calculateDeterminant();
}

/**
 * Returns the transpose of the matrix
 * @return transpose of the matrix
 */
Matrix Matrix::calculateTranspose() const {
  Matrix result(numColumns, numRows);
  for (int i = 0; i < numColumns; i++) {
    for (int j = 0; j < numRows; j++) {
      result.matrixData[i][j] = matrixData[j][i];
    }
  }

  return result;
}

/**
 * Returns the adjoint of the matrix
 * @return adjoint of the matrix
 */
Matrix Matrix::calculateAdjoint() const {
  if (numRows != numColumns) {
    throw "Matrix is not square";
  }

  Matrix result(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      result.matrixData[i][j] = calculateCofactor(i, j);
    }
  }
  return result.calculateTranspose();
}

/**
 * Returns the determinant of the matrix
 * @return determinant of the matrix
 */
double Matrix::calculateDeterminant() const {
  if (numRows != numColumns) {
    throw "Matrix is not square";
  }

  if (numRows == 1) {
    return matrixData[0][0];
  }

  double determinantValue = 0.0;
  for (int i = 0; i < numRows; i++) {
    determinantValue += matrixData[0][i] * calculateCofactor(0, i);
  }
  return determinantValue;
}

/**
 * Returns the inverse of the matrix
 * @return inverse of the matrix
 */
Matrix Matrix::calculateInverse() const {
  if (numRows != numColumns) {
    throw "Matrix is not square";
  }

  const double det = calculateDeterminant();
  if (det == 0.0) {
    throw "Matrix is not invertible";
  }

  Matrix adjointMatrix = calculateAdjoint();
  return adjointMatrix / det;
}

/**
 * Converts the matrix to its normal form
 * @return Matrix
 */
Matrix Matrix::normalizeMatrix() const {
  const double det = calculateDeterminant();
  if (det == 0.0) {
    throw "Matrix is not invertible";
  }

  return *this / det;
}

#endif