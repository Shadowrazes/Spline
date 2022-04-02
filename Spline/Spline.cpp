#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Eq {
    double x = 0;
    double y = 0;
};

void columnDivison(vector<vector<double>>& matrix, int column, bool max) {
    for (int i = column; i < matrix.size(); i++) {
        double firstElem = matrix[i][column];
        bool nullColumn = false;
        if (firstElem == 0 && i == column) {
            for (int j = column + 1; j < matrix.size(); j++) {
                if (matrix[j][column] != 0) {
                    swap(matrix[i], matrix[j]);
                    firstElem = matrix[i][column];
                    nullColumn = false;
                    break;
                }
                else
                    nullColumn = true;
            }
        }
        if (nullColumn) {
            cout << "Error: Null column" << endl;
            exit(1);
        }
        if (max) {
            for (int j = column + 1; j < matrix.size(); j++) {
                if (matrix[j][column] > firstElem && matrix[j][column] != 0) {
                    swap(matrix[i], matrix[j]);
                    firstElem = matrix[i][column];
                }
            }
            max = false;
        }
        if (firstElem != 0)
            for (auto& elem : matrix[i]) {
                elem /= firstElem;
            }
    }
}

void strokeAddition(vector<vector<double>>& matrix, int column) {
    int sign = 1;
    for (int i = column + 1; i < matrix.size(); i++) {
        if (matrix[i][column] == 0)
            continue;
        if (matrix[column][column] * matrix[i][column] < 0)
            sign = 1;
        else
            sign = -1;
        for (int j = column; j < matrix[i].size(); j++) {
            matrix[i][j] += sign * matrix[column][j];
        }
    }
}

void printVector(vector <Eq>& vect) {
    cout << "X: ";
    for (const auto& it : vect)
        cout << it.x << " ";
    cout << endl;
    cout << "Y: ";
    for (const auto& it : vect)
        cout << it.y << " ";
    cout << endl << endl;
}

void strokeWrap(string& strokeX, string& strokeY, vector <Eq>& equations) {
    while (strokeX.find(" ") != -1) {
        Eq buff = { stod(strokeX.substr(0, strokeX.find(" "))), stod(strokeY.substr(0, strokeY.find(" "))) };
        strokeX.erase(0, strokeX.find(" ") + 1);
        strokeY.erase(0, strokeY.find(" ") + 1);
        equations.push_back(buff);
    }
}

void findRoots(vector<vector<double>>& matrix, vector <double>& roots) {
    int strokeNum = matrix.size() - 1;
    while (strokeNum >= 0) {
        int i = 0;
        for (i; i < matrix[strokeNum].size(); i++) {
            if (matrix[strokeNum][i] != 0)
                break;
        }
        if (i == matrix[strokeNum].size() - 1 && matrix[strokeNum][i] != 0) {
            cout << "Error: Incompatible system of equations" << endl;
            return;
        }
        int j = strokeNum - 1;
        while (j >= 0) {
            matrix[j][i] *= *(matrix[strokeNum].end() - 1) / matrix[strokeNum][i];
            *(matrix[j].end() - 1) -= matrix[j][i];
            j--;
        }
        roots.push_back(*(matrix[strokeNum].end() - 1) / matrix[strokeNum][i]);
        strokeNum--;
    }
}

double inputX() {
    cout << "Input X: ";
    double x;
    cin >> x;
    return x;
}

vector<double> findIntervalLength(vector <Eq>& equations, double x, int& index) {
    vector<double> h;
    for (int i = 0; i < equations.size() - 1; i++) {
        h.push_back(equations[i + 1].x - equations[i].x);
        if (x >= equations[i].x && x <= equations[i + 1].x) {
            cout << endl << "Interval #" << i + 1;
            index = i + 1;
        }
    }
    return h;
}

void findMatrixM(vector<vector<double>>& matrix, vector<double>& h) {
    cout << endl << "Matrix C:" << endl << endl;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            if (i == j)
                matrix[i][j] = (h[i] + h[i + 1]) / 3.0;
            else if (j == i - 1)
                matrix[i][j] = h[i] / 6.0;
            else if (j == i + 1)
                matrix[i][j] = h[j + 1] / 6.0;
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

vector<double> findVectorD(vector <Eq>& equations, vector<double>& h) {
    vector<double> d;
    cout << endl << "Vector D:" << endl << endl;
    for (int i = 1; i < equations.size() - 1; i++) {
        d.push_back
        (
            (equations[i + 1].y - equations[i].y) / h[i] - (equations[i].y - equations[i - 1].y) / h[i - 1] // ...
        );
        cout << d[i - 1] << " ";
    }
    cout << endl;
    return d;
}

double calcS(vector <Eq>& equations, vector<double>& h, vector <double>& roots, int& i, double& x) {
    cout << "S_" << i << "(" << x << ")" << " = ";
    double s = roots[i - 1] * pow(equations[i].x - x, 3) / (6 * h[i - 1]) + roots[i] * pow(x - equations[i - 1].x, 3) / (6 * h[i - 1])
        + (equations[i - 1].y - roots[i - 1] * pow(h[i - 1], 2) / 6.0) * (equations[i].x - x) / h[i - 1]
        + (equations[i].y - roots[i] * pow(h[i - 1], 2) / 6.0) * (x - equations[i - 1].x) / h[i - 1];
    return s;
}

int main()
{
    double x = inputX();
    int index;
    fstream matrixData("Polynomial.txt");
    vector <Eq> equations;

    while (!matrixData.eof()) {
        string buffX;
        string buffY;
        getline(matrixData, buffX);
        getline(matrixData, buffY);
        strokeWrap(buffX, buffY, equations);
    }

    vector<vector<double>> matrix(equations.size() - 2, vector<double>(equations.size() - 2, 0));
    vector <double> roots;

    vector<double> h = findIntervalLength(equations, x, index);
    vector<double> d = findVectorD(equations, h);
    findMatrixM(matrix, h);

    for (int i = 0; i < matrix.size(); i++) {
        matrix[i].push_back(d[i]);
    }

    cout.precision(4);
    cout << endl << "Original Matrix:" << endl;
    for (auto& stroke : matrix) {
        for (auto& column : stroke) {
            if (column == *(stroke.end() - 1))
                cout << "| ";
            cout << fixed << column << " ";
        }
        cout << endl << endl;
    }

    for (int i = 0; i < matrix.size(); i++) {
        columnDivison(matrix, i, false);
        strokeAddition(matrix, i);
    }

    cout << "Triangle Matrix:" << endl;
    for (auto& column : matrix) {
        for (auto& stroke : column) {
            if (stroke == *(column.end() - 1))
                cout << "| ";
            cout << fixed << stroke << " ";
        }
        cout << endl << endl;
    }

    findRoots(matrix, roots);
    roots.insert(roots.begin(), 0);
    roots.push_back(0);
    cout << "Roots" << endl;
    for (auto it = roots.rbegin(); it != roots.rend(); ++it) {
        cout << fixed << *it << " ";
    }
    cout << endl << endl;
    reverse(roots.begin(), roots.end());
    cout << calcS(equations, h, roots, index, x) << endl;
}