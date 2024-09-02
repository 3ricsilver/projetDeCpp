#include "PhotoShop.h"
#include "Image.h"
#include "ImageB.h"
#include "ImageNG.h"
#include "ImageRGB.h"
#include "Iterateur.h"
#include "RGBException.h"
#include "XYException.h"
#include <sstream>
#include <vector>

int PhotoShop::numCourant(1);

PhotoShop PhotoShop::instance;
Image *PhotoShop::operande1(nullptr);
Image *PhotoShop::operande2(nullptr);
Image *PhotoShop::resultat(nullptr);

PhotoShop::PhotoShop()
{
    cout << "PhotoShop: constructeur par défaut" << endl;
}

PhotoShop::~PhotoShop()
{
    reset();
    cout << "PhotoShop: destructeur" << endl;
}

void PhotoShop::reset()
{
    int taille = images.getNombreElements();

    while (taille)
    {
        supprimeImageParIndice(0);
        taille--;
    }

    numCourant = 1;
}

void PhotoShop::ajouteImage(Image *pImage)
{
    if (pImage->getId() <= 0 || getImageParId(pImage->getId()))
    {
        pImage->setId(numCourant++);
    }
    images.insereElement(pImage);
}

void PhotoShop::afficheImages() const
{
    Iterateur<Image *> it(images);

    for (it.reset(); !it.end(); it++)
        ((Image *)it)->Affiche();
}

void PhotoShop::dessineImages() const
{
    Iterateur<Image *> it(images);

    for (it.reset(); !it.end(); it++)
        ((Image *)it)->Dessine();
}

Image *PhotoShop::getImageParIndice(int indice)
{
    if (indice < 0 || indice > images.getNombreElements() - 1)
        return nullptr;

    return images.getElement(indice);
}

Image *PhotoShop::getImageParId(int id)
{
    Iterateur<Image *> it(images);
    int i = 0;

    while (!it.end() && ((Image *)it)->getId() < id)
    {
        it++;
        i++;
    }

    if (it.end() || ((Image *)it)->getId() != id)
        return nullptr;

    return images.getElement(i);
}

void PhotoShop::supprimeImageParIndice(int ind)
{
    if (ind < 0 || ind > images.getNombreElements() - 1)
        return;

    delete images.retireElement(ind);
}

void PhotoShop::supprimeImageParId(int id)
{
    Iterateur<Image *> it(images);
    int i = 0;
    try
    {
        while (!it.end() && ((Image *)it)->getId() < id)
        {
            it++;
            i++;
        }

        if (it.end() || ((Image *)it)->getId() != id)
            return;

        supprimeImageParIndice(i);
    }

    catch (const XYException &e)
    {
        cout << e.getMsg() << endl;
    }
    catch (const RGBException &e)
    {
        cout << e.getMsg() << endl;
    }
    catch (const Exception &e)
    {
        cout << e.getMsg() << endl;
    }
}

PhotoShop &PhotoShop::getInstance()
{
    return instance;
}

int PhotoShop::importeImages(string nomFichier)
{

    cout << "numcourant:" << numCourant << endl;
    int nbImporte = 0;
    bool erreur = false;

    ifstream file(nomFichier);

    if (!file.is_open())
        return -1;

    string ligne;

    while (getline(file, ligne))
    {
        istringstream ligneStream(ligne);
        string champ;
        vector<string> champs;

        while (getline(ligneStream, champ, ';'))
        {
            champs.push_back(champ);
        }

        ImageNG *imageNG;
        ImageRGB *imageRGB;

        if (champs.size() != 3)
            break;

        switch (champs[0].at(0))
        {
        case 'N':
            imageNG = new ImageNG(champs[1].c_str());
            imageNG->setNom(champs[2].c_str());
            ajouteImage(imageNG);
            nbImporte++;
            break;
        case 'R':
            imageRGB = new ImageRGB(champs[1].c_str());
            imageRGB->setNom(champs[2].c_str());
            ajouteImage(imageRGB);
            nbImporte++;
            break;
        default:
            erreur = true;
        }

        if (erreur)
            break;
    }

    file.close();

    return nbImporte;
}

void PhotoShop::Save()
{
    ofstream file;
    file.open("sauvegarde.dat", std::ios::binary);

    if (!file.is_open())
        return;

    file.write((char *)&numCourant, sizeof(int));
    ImageB::couleurTrue.Save(file);
    ImageB::couleurFalse.Save(file);

    Iterateur<Image *> it(images);
    int type;

    for (it.reset(); !it.end(); it++)
    {
        if (dynamic_cast<ImageNG *>(&it))
        {
            type = 1;
            file.write((char *)&type, sizeof(int));
            reinterpret_cast<ImageNG *>(&it)->Save(file);
        }
        else if (dynamic_cast<ImageRGB *>(&it))
        {
            type = 2;
            file.write((char *)&type, sizeof(int));
            reinterpret_cast<ImageRGB *>(&it)->Save(file);
        }
        else
        {
            type = 3;
            file.write((char *)&type, sizeof(int));
            reinterpret_cast<ImageB *>(&it)->Save(file);
        }
    }

    file.close();
}

void PhotoShop::Load()
{
    ifstream fichier;

    fichier.open("sauvegarde.dat", std::ios::binary);

    if (!fichier.is_open())
        return;

    fichier.read((char *)&numCourant, sizeof(int));
    ImageB::couleurTrue.Load(fichier);
    ImageB::couleurFalse.Load(fichier);
    int type;

    ImageNG *imageNG(nullptr);
    ImageRGB *imageRGB(nullptr);
    ImageB *imageB(nullptr);

    while (fichier.read((char *)&type, sizeof(int)))
    {
        switch (type)
        {
        case 1:
            imageNG = new ImageNG();
            imageNG->Load(fichier);
            ajouteImage(imageNG);
            break;
            ;
        case 2:
            imageRGB = new ImageRGB();
            imageRGB->Load(fichier);
            ajouteImage(imageRGB);
            break;
        default:
            imageB = new ImageB();
            imageB->Load(fichier);
            ajouteImage(imageB);
            break;
            ;
        }
    }

    fichier.close();
}