#include "ArrayList.h"
#include "Couleur.h"
#include "Exception.h"
#include "Image.h"
#include <string>

template <class T> ArrayList<T>::ArrayList()
{
    pTete = nullptr;
}

template <class T> ArrayList<T>::ArrayList(const ArrayList &l)
{
    pTete = nullptr;
    Cellule<T> *tmp = l.pTete;

    while (tmp)
    {
        insereElement(tmp->valeur);
        tmp = tmp->suivant;
    }
}

template <class T> ArrayList<T>::~ArrayList()
{
    while (pTete)
    {
        Cellule<T> *tmp = pTete;
        pTete = pTete->suivant;
        delete tmp;
    }
}

template <class T> bool ArrayList<T>::estVide()
{
    if (pTete == nullptr)
    {
        return true;
    }
    return false;
}

template <class T> int ArrayList<T>::getNombreElements() const
{
    int nb = 0;

    Cellule<T> *tmp = pTete;

    while (tmp)
    {
        nb++;
        tmp = tmp->suivant;
    }

    return nb;
}

template <class T> void ArrayList<T>::Affiche() const
{

    Cellule<T> *tmp = pTete;

    while (tmp != nullptr)
    {
        cout << tmp->valeur << "\n";

        tmp = tmp->suivant;
    }
}

template <class T> void ArrayList<T>::insereElement(const T &val)
{

    Cellule<T> *nouveau = new Cellule<T>;

    nouveau->valeur = val;

    nouveau->suivant = NULL;

    if (pTete == NULL)
    {
        pTete = nouveau;
    }

    else
    {
        Cellule<T> *courant = pTete;
        while (courant->suivant)
        {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}

template <class T> T &ArrayList<T>::getElement(int ind)
{
    int i;
    Cellule<T> *tmp = pTete;

    if (ind > getNombreElements() || ind < 0)
        throw Exception("Arraylist: indice hors interval");

    for (i = 0; i < ind; i++)
    {

        tmp = tmp->suivant;
    }

    return tmp->valeur;
}

template <class T> T ArrayList<T>::retireElement(int ind)
{
    if (ind > getNombreElements() || ind < 0)
        throw Exception("Arraylist: indice hors interval");

    Cellule<T> *tmp = pTete, *prec = nullptr;
    T valeurSupprimee;

    if (ind == 0)
    {
        pTete = pTete->suivant;
        valeurSupprimee = tmp->valeur;
        delete tmp;
    }
    else
    {
        while (ind > 0)
        {
            prec = tmp;
            tmp = tmp->suivant;
            ind--;
        }
        prec->suivant = tmp->suivant;
        valeurSupprimee = tmp->valeur;
        delete tmp;
    }

    return valeurSupprimee; // Return the value of the deleted element
}

template <class T> T &ArrayList<T>::operator=(const ArrayList &l)
{

    Cellule<T> *tmp = pTete;

    while (pTete)
    {
        tmp = pTete;
        pTete = pTete->suivant;
        delete tmp;
    }

    pTete = NULL;

    Cellule<T> *nouveau = l.pTete;

    while (nouveau)
    {
        insereElement(nouveau->valeur);
        nouveau = nouveau->suivant;
    }

    return nouveau->valeur;
}

template class ArrayList<int>;

template class ArrayList<Couleur>;

template class ArrayList<Image *>;