#include "ListaEnc.hpp"

template <typename T>
ListaEnc<T>::ListaEnc() {
    head = nullptr;
    size = 0;
}
/**
 * Destructor. Deletes the contents[] array.
 */
template <typename T>
ListaEnc<T>::~ListaEnc() {
    destroiLista();
}
/**
 * Clears the list.
 */
template <typename T>
void ListaEnc<T>::destroiLista() {
    while (!listaVazia()) {
        retiraDoInicio();
    }
}
/**
 * Checks if the list is listaVazia.
 * \return boolean
 */
template <typename T>
bool ListaEnc<T>::listaVazia() const {
    return size == 0;
}
/**
 * Adds an element to the front of the list.
 */
template <typename T>
void ListaEnc<T>::adicionaNoInicio(const T& dado) {
    Element<T>* first = new Element<T>(dado);
    if (first == nullptr) {
        throw std::out_of_range("Lista cheia.");
    }
    first->setProximo(head);
    head = first;
    ++size;
}
/**
 * Removes the first element.
 * \return The first element of type T.
 */
template <typename T>
T ListaEnc<T>::retiraDoInicio() {
    if (listaVazia()) {
        throw std::out_of_range("Lista vazia");
    } else {
        Element<T>* first = head;
        T out = first->getInfo();
        head = first->getProximo();
        delete first;
        --size;
        return out;
    }
}
/**
 * Inserts an element in the specified location.
 * \param data data reference;
 * \param index index where the element will be put in.
 */
template <typename T>
void ListaEnc<T>::adicionaNaPosicao(const T& dado, int pos) {
  Element<T>* input;
  Element<T>* prev;
  if (pos > size) {
    throw std::out_of_range("Index inválido");
  } else if (pos == 0) {
        adicionaNoInicio(dado);
  } else {
        input = new Element<T>(dado);
        if (input == nullptr) {
            throw std::out_of_range("Lista cheia.");
        }
        prev = head;
        for (int i = 0; i < pos - 1; i++) {
            prev = prev->getProximo();
        }
        input->setProximo(prev->getProximo());
        prev->setProximo(input);
        ++size;
    }
}
/**
 * Takes an element out by its index.
 * \param index element index.
 */
template <typename T>
T ListaEnc<T>::retiraDaPosicao(int pos) {
    T output;
    Element<T>* popped;
    Element<T>* prev;
    if (listaVazia()) {
        throw std::out_of_range("Lista vazia");
    } else if (pos > size - 1) {
        throw std::out_of_range("Parâmetro inválido");
    } else if (pos == 0) {
        return retiraDoInicio();
    } else {
        prev = head;
        for (int i = 0; i < pos - 1; i++) {
            prev = prev->next();
        }
        popped = prev->getProximo();
        output = popped->getInfo();
        prev->setProximo(popped->getProximo());
        --size;
        delete popped;
        return output;
    }
}
/**
 * Inserts an element respecting a specific progression.
 * \param data data.
 */
template <typename T>
void ListaEnc<T>::adicionaEmOrdem(const T& dado) {
    Element<T>* current;
    if (listaVazia()) {
        adicionaNoInicio(dado);
    } else {
        current = head;
        std::size_t pos;
        for (pos = 0; pos < size && dado > current->getInfo(); pos++) {
            current = current->getProximo();
        }
        adicionaNaPosicao(dado, pos);
    }
}
/**
 * Adds an element to the end of the list.
 */
template <typename T>
void ListaEnc<T>::adiciona(const T& dado) {
    adicionaNaPosicao(dado, size);
}
/**
 * Removes the last element.
 */
template <typename T>
T ListaEnc<T>::retira() {
    return retiraDaPosicao(size - 1);
}
/**
 * Checks if the list has a certain element, and if so, returns its index.
 * \return specified element index.
 */
template <typename T>
int ListaEnc<T>::posicao(const T& dado) const {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *current = head;
    for (std::size_t i = 0; i < size; ++i) {
        if (current->getInfo() == dado) return i;
        current = current->getProximo();
    }
    return size;
}

template <typename T>
T* ListaEnc<T>::posicaoMem(const T& dado) const {
	if (listaVazia())
        throw std::out_of_range("List is empty");

    Elemento<T> *current = head;
    for (std::size_t i = 0; i < size; ++i) {
        if (current->getInfo() == dado) return current;
        current = current->getProximo();
    }
    return nullptr;
}
/**
 * Removes an element by its content.
 * \param data data.
 */
template <typename T>
void ListaEnc<T>::retiraEspecifico(const T& dado) {
    std::size_t test = find(data);
    if (test != size) {
        retiraDaPosicao(test);
    }
}
/**
 * Checks if the list contains a certain element by its content.
 * \return boolean
 */
template <typename T>
bool ListaEnc<T>::contem(const T& dado) const {
    return posicao(dado) != size;
}

template <typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) {
	return dado1 == dado2;
}

template <typename T>
bool ListaEnc<T>::maior(T dado1, T dado2) {
	return dado1 > dado2;
}

template <typename T>
bool ListaEnc<T>::menor(T dado1, T dado2) {
	return dado1 < dado2;
}