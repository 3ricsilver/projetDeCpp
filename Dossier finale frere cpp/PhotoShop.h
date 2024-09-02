#pragma once

#include "ArrayList.h"
#include "Image.h"

class PhotoShop
{
  private:
    ArrayList<Image *> images;
    static int numCourant;
    static PhotoShop instance;

    PhotoShop();
    PhotoShop(PhotoShop &copie) = default;
    PhotoShop &operator=(PhotoShop &PhotoShop) = default;

  public:
    static Image *operande1;
    static Image *operande2;
    static Image *resultat;

    ~PhotoShop();
    void reset();
    void ajouteImage(Image *pImage);

    void afficheImages() const;
    void dessineImages() const;
    Image *getImageParIndice(int indice);
    Image *getImageParId(int id);
    void supprimeImageParIndice(int ind);
    void supprimeImageParId(int id);
    int importeImages(string nomFichier);
    void Save();
    void Load();

    static PhotoShop &getInstance();
};
