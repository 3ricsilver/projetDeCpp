#ifndef ITERATEUR_H
#define ITERATEUR_H

template <class T> class ArrayList;

template <class> struct Cellule;

template <class T> class Iterateur
{

  private:
    Cellule<T> *pCur;
    const ArrayList<T> &list;

  public:
    explicit Iterateur(const ArrayList<T> &i);
    bool end();
    void reset();

    void operator++();
    void operator++(int post);

    operator T() const;
    T &operator&() const;
};

#endif