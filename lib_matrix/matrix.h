// Copyright 2022 Filatov Maxim

#ifndef LIB_MATRIX_MATRIX_H_
#define LIB_MATRIX_MATRIX_H_

#define MAX_VECTOR_SIZE 100000000
#define MAX_MATRIX_SIZE 10000

#include <iostream>
#include <algorithm>
#include <utility>

#pragma region TDynamicVector

template<typename T>
class TDynamicVector {
protected:
    size_t sz;
    T* pMem;

    void border_check() { //функция которая проверяет размер вектора
        sz <= 0 ? //sz<=0
            throw std::out_of_range("Vector size should be greater than zero") : //исключение, размер вектора больше 0
        sz > MAX_VECTOR_SIZE ? //если так
            throw std::out_of_range("Vector size must be less than 100000000") : NULL; //то вектор должен быть меньше чем 100000000
    }
public:
    explicit TDynamicVector(int size = 1) : sz(size) { 
        border_check(); //проверяет размер вектора
        if (sz <= 0) {
            throw std::out_of_range("Vector size should be greater than zero"); //конструкор
        }
        pMem = new T[sz]();
    }

    explicit TDynamicVector(T* arr, int s) : sz(s) {
        border_check(); //проверяет размер вектора
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg"); //конструктор копирования
        pMem = new T[sz];
        std::copy(arr, arr + sz, pMem);
    }

    explicit TDynamicVector(const TDynamicVector& v) { //конструктор копирования
        sz = v.sz; //копирует размер
        pMem = new T[sz](); //создает массив размером sz
        for (int i = 0; i < sz; i++) { //заполнить элементами цикла
            pMem[i] = v.pMem[i];
        }
    }

    explicit TDynamicVector(TDynamicVector&& v) noexcept { //конструктор перемещения
        if (this != &v) { //если массив не равен данному массиву, то он возвращает его же
            std::swap(*this, v);
        }
        return *this;
    }

    ~TDynamicVector() {}//деструктор
    template <class T>
    TVector<T>:⁓TVector(){
        if (data!=nullptr)
            delete[]data;
        size = 0;
        data=nullptr;
    }


    TDynamicVector& operator=(const TDynamicVector& v) { //оператор присваивания
        if (this == &v) { //если значение, которое мы подаем, равно значению, которое хотим присвоить, возвращаем этот вектор
            return *this;
        }
        delete[] pMem; //если нет, удаляем pMem, зануляемся
        pMem = nullptr;
        sz = 0;
        this->equating(v);
        return *this; //возвращает значение
    }

    void allocate_new_memory(int size) { //функция, удаляет массив, зануляет его, проверяет исключение
        delete[] pMem;              // release resource in *this
        pMem = nullptr;
        sz = 0;                     // preserve invariants in case next line throws
        pMem = new T[size]; //создает массив pMem с размером size
        sz = size;
    }

    void equating(const TDynamicVector& v, bool enable_auto_resize = true) { 
        if (sz != v.sz && enable_auto_resize) {
            allocate_new_memory(v.sz);
        }
        int size = sz <= v.sz ? sz : v.sz;
        for (int i = 0; i < size; i++) {
            pMem[i] = v.pMem[i];
        }
    }

    void resize(int size) { //функция меняет размер вектора
        if (size < 0) { //если size < 0, то ошибка, иначе создает массив нужного размера
            throw std::logic_error("Size < 0");
        }
        TDynamicVector temp;
        temp.equating(*this);
        *this = TDynamicVector(size);
        this->equating(temp, false);
    }

    TDynamicVector& operator=(TDynamicVector&& v) noexcept { //функция присваивания с перемещением
        delete[] pMem; //удаляет массив, зануляет, удаляет и возвращает
        pMem = nullptr;
        sz = 0;
        swap(*this, v);
        return *this;
    }

    size_t size() const noexcept { return sz; } //возвращает размер вектора

    // indexing

    T& operator[](size_t ind) { 
        return pMem[ind]; //возвращает индекс элемента
    }

    const T& operator[](size_t ind) const { //возвращает индекс элемента
        return pMem[ind];
    }

    // indexing with control

    T& at(size_t ind) { //если номер элемента больше чем size, выдает исключение, иначе возвращает этот элемент
        if (ind >= sz) {
            throw std::out_of_range("out of range");
        }
        return pMem[ind];
    }

    const T& at(size_t ind) const { 
        if (ind >= sz) {
            throw std::out_of_range("out of range");
        }
        return pMem[ind];
    }

    // comparisons

    bool operator==(const TDynamicVector& v) const noexcept { //оператор, коорый определяет ровны вектора или нет
        int size = v.sz;
        if (sz != v.sz) { //если
            return false; //не равны
        }
        // for (int i = 0; i < size; i++) { //смотрим массивы, сравниваем элементы 1 со 2, если они не равны, то возвращаем false, иначе равны
            if (pMem[i] != v.pMem[i]) { 
                return false;
            }
        }
        return true;
    }

    bool operator!=(const TDynamicVector& v) const noexcept { 
        return !(*this == v);
    }

    // scalar operations

    TDynamicVector operator+=(T val) { 
        for (int i = 0; i < sz; i++) { //каждому элементу добавляем значение, возвращаем массив
            pMem[i] += val;
        }
        return this;
    }

    TDynamicVector operator-=(T val) { //минус
        for (int i = 0; i < sz; i++) {
            pMem[i] -= val;
        }
        return this;
    }

    TDynamicVector operator*=(T val) { //умножение
        for (int i = 0; i < sz; i++) {
            pMem[i] *= val;
        }
        return this;
    }

    TDynamicVector operator/=(T val) { //деление
        for (int i = 0; i < sz; i++) {
            pMem[i] /= val;
        }
        return this;
    }

    // vector operations //векторные операции

    TDynamicVector operator+=(const TDynamicVector& v) {
        if (sz != v.sz) { //если вектора не равны по размеру, выдаем исключение, pMem[i] += v.pMem[i], возвращаем массив
            throw std::logic_error("not equals size");
        }
        for (int i = 0; i < sz; i++) { //если равны, 
            pMem[i] += v.pMem[i];
        }
        return this;
    }

    TDynamicVector operator-=(const TDynamicVector& v) { //минус
        if (sz != v.sz) {
            throw std::logic_error("not equals size");
        }
        for (int i = 0; i < sz; i++) {
            pMem[i] -= v.pMem[i];
        }
        return this;
    }

    T operator*(const TDynamicVector& v) noexcept(noexcept(T())) { 
        T elem = 0;
        int size = sz;
        int size2 = v.sz;
        if (sz != v.sz) { //если размер вектора не равен размеру другого вектора, выдаем исключение
            throw std::logic_error("not equals size");
        }
        for (int i = 0; i < size; i++) { //иначе результат умножения двух массивов, возвращаем elem
            elem += pMem[i] * v.pMem[i];
        }
        return elem;
    }

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept { 
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    // input / output //ввод и вывод


    friend std::istream& operator>>(std::istream& istr, TDynamicVector& v) { //оператор ввода
        for (size_t i = 0; i < v.sz; i++) 
            istr >> v.pMem[i]; //вводим массив
        return istr; //возвращаем массив
    }

    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicVector& v) { //оператор вывода
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' ';
        return ostr;
    }
};

#pragma endregion

#pragma region TDynamicMatrix //класс матриц, унаследовано от TVector

template<typename T> 
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>> { //присваивание с перемещением
private:
    using TDynamicVector<TDynamicVector<T>>::pMem; //удаляем pMem
    using TDynamicVector<TDynamicVector<T>>::sz; //заняляем 

    void border_check();

public:
    explicit TDynamicMatrix(size_t s = 1);

    TDynamicMatrix(const TDynamicMatrix& other);

    TDynamicMatrix& operator=(const TDynamicMatrix<T>& m);

    void allocate_new_memory(int size);

    void equating(const TDynamicMatrix& v, bool enable_auto_resize = true);

    void resize(int size);

    TDynamicMatrix operator=(TDynamicMatrix&& v) noexcept {
        delete[] pMem;
        pMem = nullptr;     
        sz = 0;
        swap(*this, v);
        return *this;
    }
    using TDynamicVector<TDynamicVector<T>>::operator=;
    using TDynamicVector<TDynamicVector<T>>::operator[];
    using TDynamicVector<TDynamicVector<T>>::at;

    // comparison

    bool operator==(const TDynamicMatrix& m) const noexcept;

    bool operator!=(const TDynamicMatrix& m) const noexcept {
        return !(*this == m); //все выше унаследовалось от TVector
    }

    size_t size() const noexcept;

    // matrix-scalar operations //скалярная операция

    TDynamicMatrix operator*(const T& val) {
        int size = sz;
        for (int i = 0; i < size; i++) { //пробегаем по всем строкам и столбцам
            for (int j = 0; j < size; j++) {
                pMem *= val; //умножаем все значения матрицы на определенное число
            }
        }
        return this;
    }

    // matrix-vector operations //оператор умножения

    TDynamicVector<T> operator*(const TDynamicVector<T>& v) { //умножение между матрицами
        int size = sz[0]; //если размеры матриц не равны бросаем в исключение
        int size2 = v.sz;
        if (size != size2) {
            throw std::logic_error("not equals sizes");
        }
        for (int i = 0; i < size; i++) { //иначе
            for (int j = 0; j < size; j++) {
                pMem[i][0] += pMem[i][j] * v.pMem[i]; //перемножаем матрицы
            }
        }
        TDynamicVector<T> tmp(size); //умножение матриц
        for (int i = 0; i < size; i++) {
            tmp.pMem[i] = pMem[i][0];
        }
        return this;
    }

    // matrix-matrix operations 

    TDynamicMatrix operator+(const TDynamicMatrix& m); 

    TDynamicMatrix operator+=(const TDynamicMatrix& m) { //возвращает матрицу, к которой прибавили m
        *this = *this + m;
        return *this;
    }

    TDynamicMatrix operator-(const TDynamicMatrix& m) { //если матрица из которой хотим отнять не равна матрицы, которую вычитаем, выдает ошибку
        if (sz != m.sz) {
            throw std::logic_error("Sizes not equal");
        }
        TDynamicMatrix tmp(sz); //иначе создаем матрицу tmp, записываем результат вычитания данной матрицы и матрицы, которая дается
        TDynamicMatrix t;
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                tmp[i][j] = (*this)[i][j] - m[i][j];
            }
        }
        return tmp;
    }

    TDynamicMatrix operator-=(const TDynamicMatrix& m) { //минус
        *this = *this - m;
        return *this;
    }

    TDynamicMatrix operator*(const TDynamicMatrix& m) { //умножение
        if (sz != m.sz) { //если размеры матриц не равны, выдаем исключение
            throw std::logic_error("Sizes not equal");
        }
        int size = this.sz; 
        TDynamicMatrix C(size); //создаем матрицу С
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                C[i][j] = 0; //результат умножения двух матриц
                for (int k = 0; k < size; k++)
                    C[i][j] += this[i][k] * m[k][j];
            }
        }
        return C; //возвращаем матрицу
    }

    // input / output

    friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& v) { //оператор вывода
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                std::cout << pMem[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    /*friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& v) {
    * // TODO
    }*/
};

#pragma endregion

#endif  // LIB_MATRIX_MATRIX_H_

template<typename T> //проверяет размер матрицы
void TDynamicMatrix<T>::border_check() { 
    sz <= 0 ?
        throw std::out_of_range("Matrix size should be greater than zero") :
        sz > MAX_MATRIX_SIZE ?
        throw std::out_of_range("Matrix size must be less than 10000") : NULL;
}

template<typename T>
TDynamicMatrix<T>::TDynamicMatrix(size_t s) : TDynamicVector<TDynamicVector<T>>(s) { //проверяем размер матрицы, если все нормально, инициальзируем ее
    border_check();
    for (size_t i = 0; i < sz; i++) {
        pMem[i] = TDynamicVector<T>(sz);
    }
}

template<typename T>
TDynamicMatrix<T>::TDynamicMatrix(const TDynamicMatrix& other) { //конструктор копирования
    pMem = other.pMem; //копируем матрицу
    sz = other.sz; //копируем размер
}

template<typename T> //оператор присваивания
TDynamicMatrix<T>& TDynamicMatrix<T>::operator=(const TDynamicMatrix<T>& m) { //если матрица равна матрице, котрую хоти присвоить, возвращаем, иначе копируем ее
    if (this == &m) {
        return *this;
    }
    this->equating(m);
    return *this; //возвращаем матрицу, котая скопировала знач второй матрицы
}

template<typename T>
void TDynamicMatrix<T>::allocate_new_memory(int size) {
    delete[] pMem;              // удаляем pMem
    pMem = nullptr;
    sz = 0;                     
    pMem = new TDynamicVector<T>[size]; /создаем новую матрицу
    sz = size;
}

template<typename T>
void TDynamicMatrix<T>::equating(const TDynamicMatrix<T>& v, bool enable_auto_resize) { //проверяем равны ли матрицы
    if (sz != v.sz && enable_auto_resize) { 
        allocate_new_memory(v.sz); //память
    }
    int size = sz <= v.sz ? sz : v.sz;
    for (int i = 0; i < size; i++) {
        pMem[i] = v.pMem[i];
    }
}

template<typename T>
void TDynamicMatrix<T>::resize(int size) { //меняет знач матрицы
    if (size < 0) { //если размер меньше 0, исключение
        throw std::logic_error("Size < 0");
    }
    TDynamicMatrix temp; //создаем матрицу с новым размером
    temp.equating(*this);
    *this = TDynamicMatrix(size);
    this->equating(temp, false);
    for (int i = 0; i < sz; i++) {
        pMem[i].resize(size);
    }
}

template<typename T>
bool TDynamicMatrix<T>::operator==(const TDynamicMatrix& m) const noexcept { 
    int size = sz; //если размеры не равны, то возвращаем 
    if (size != m.sz) {
        return false;
    }
    for (int i = 0; i < size; i++) { //проверяем элементы матриц
        if (pMem[i] != m.pMem[i]) {
            return false;
        }
    }
    return true;
}

template<typename T> //возвращает размер матрицы
size_t TDynamicMatrix<T>::size() const noexcept {
    return sz; 
}

template<typename T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator+(const TDynamicMatrix& m) { //если размер матриц не равны, исключение
    if (sz != m.sz) {
        throw std::logic_error("Sizes not equal");
    }
    TDynamicMatrix tmp(sz); //иначе создаем матрицу tmp
    TDynamicMatrix t;
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            tmp[i][j] = (*this)[i][j] + m[i][j]; //записываем результат сложения двух матриц
        }
    }
    return tmp; //вовращаем матрицу
}
