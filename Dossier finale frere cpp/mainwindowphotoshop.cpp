#include "mainwindowphotoshop.h"
#include "Couleur.h"
#include "Exception.h"
#include "Image.h"
#include "RGBException.h"
#include "ui_mainwindowphotoshop.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QRect>
#include <QScreen>
#include <algorithm>
#include <cctype>
#include <regex>
#include <string>

#include "ImageB.h"
#include "ImageNG.h"
#include "ImageRGB.h"
#include "Iterateur.h"
#include "PhotoShop.h"
#include "Traitements.h"
#include "XYException.h"

MainWindowPhotoShop::MainWindowPhotoShop(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowPhotoShop)
{
    ui->setupUi(this);
    setTitre("Mini-PhotoShop  en C++");

    // Centrage de la fenetre
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    setFixedSize(1021, 845);

    // Configuration de la table des images
    ui->tableWidgetImages->setColumnCount(4);
    ui->tableWidgetImages->setRowCount(0);
    QStringList labelsTableOptions;
    labelsTableOptions << "Id"
                       << "Type"
                       << "Dimension"
                       << "Nom";
    ui->tableWidgetImages->setHorizontalHeaderLabels(labelsTableOptions);
    ui->tableWidgetImages->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetImages->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetImages->horizontalHeader()->setVisible(true);
    ui->tableWidgetImages->horizontalHeader()->setDefaultSectionSize(100);
    ui->tableWidgetImages->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetImages->verticalHeader()->setVisible(false);
    ui->tableWidgetImages->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    // Intialisation des scrollArea

    setImageNG("selection");
    setImageNG("operande1");
    setImageNG("operande2");
    setImageNG("resultat");

    // ComboBox des traitements disponibles
    ajouteTraitementDisponible("Eclaircir (+ val)");
    ajouteTraitementDisponible("Eclaircir (++)");
    ajouteTraitementDisponible("Assombrir (- val)");
    ajouteTraitementDisponible("Assombrir (--)");
    ajouteTraitementDisponible("Différence");
    ajouteTraitementDisponible("Comparaison (==)");
    ajouteTraitementDisponible("Comparaison (<)");
    ajouteTraitementDisponible("Comparaison (>)");
    ajouteTraitementDisponible("Seuillage");
    ajouteTraitementDisponible("Filtre moyenneur");
    ajouteTraitementDisponible("Filtre médian");
    ajouteTraitementDisponible("Erosion");
    ajouteTraitementDisponible("Dilatation");
    ajouteTraitementDisponible("Négatif");

    // Restauration bibliothèque via fichier de sauvegarde
    PhotoShop::getInstance().Load();
    updateTable();
}

MainWindowPhotoShop::~MainWindowPhotoShop()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Méthodes de la fenêtre (ne pas modifier) ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setTitre(string titre)
{
    this->setWindowTitle(titre.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setNomImage(string nom)
{
    ui->lineEditNom->setText(QString::fromStdString(nom));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowPhotoShop::getNomImage() const
{
    return ui->lineEditNom->text().toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setParametresImageNG(int max, int min, int luminance, float contraste)
{
    if (max == -1)
    {
        ui->lineEditMax->setText("");
        ui->lineEditMin->setText("");
        ui->lineEditLuminance->setText("");
        ui->lineEditContraste->setText("");
        return;
    }
    char tmp[20];
    sprintf(tmp, "%d", max);
    ui->lineEditMax->setText(tmp);
    sprintf(tmp, "%d", min);
    ui->lineEditMin->setText(tmp);
    sprintf(tmp, "%d", luminance);
    ui->lineEditLuminance->setText(tmp);
    sprintf(tmp, "%f", contraste);
    ui->lineEditContraste->setText(tmp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindowPhotoShop::setImageNG(string destination, const ImageNG *imageng)
{
    QPixmap *pixmap = NULL;
    if (imageng != NULL)
    {
        // Creation du QPixmap
        int largeur = imageng->getDimension().getLargeur();
        int hauteur = imageng->getDimension().getHauteur();
        QImage im1(largeur, hauteur, QImage::Format_RGB32);
        for (int x = 0; x < largeur; x++)
            for (int y = 0; y < hauteur; y++)
            {
                int valeur = imageng->getPixel(x, y);
                im1.setPixel(x, y, qRgb(valeur, valeur, valeur));
            }
        pixmap = new QPixmap(QPixmap::fromImage(im1));
    }

    // Mise en place du QPixmap
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    if (pixmap)
        label->setPixmap(*pixmap);
    else
        label->setText("vide");
    if (destination == "selection")
        ui->scrollAreaSelection->setWidget(label);
    if (destination == "operande1")
        ui->scrollAreaOperande1->setWidget(label);
    if (destination == "operande2")
        ui->scrollAreaOperande2->setWidget(label);
    if (destination == "resultat")
        ui->scrollAreaResultat->setWidget(label);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setImageRGB(string destination, const ImageRGB *imagergb)
{
    QPixmap *pixmap = NULL;
    if (imagergb != NULL)
    {
        // Creation du QPixmap
        int largeur = imagergb->getDimension().getLargeur();
        int hauteur = imagergb->getDimension().getHauteur();
        QImage im1(largeur, hauteur, QImage::Format_RGB32);
        for (int x = 0; x < largeur; x++)
            for (int y = 0; y < hauteur; y++)
            {
                Couleur valeur = imagergb->getPixel(x, y);
                im1.setPixel(x, y, qRgb(valeur.getRouge(), valeur.getVert(), valeur.getBleu()));
            }
        pixmap = new QPixmap(QPixmap::fromImage(im1));
    }

    // Mise en place du QPixmap
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    if (pixmap)
        label->setPixmap(*pixmap);
    else
        label->setText("vide");
    if (destination == "selection")
        ui->scrollAreaSelection->setWidget(label);
    if (destination == "operande1")
        ui->scrollAreaOperande1->setWidget(label);
    if (destination == "operande2")
        ui->scrollAreaOperande2->setWidget(label);
    if (destination == "resultat")
        ui->scrollAreaResultat->setWidget(label);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setImageB(string destination, const ImageB *imageb)
{
    QPixmap *pixmap = NULL;
    if (imageb != NULL)
    {
        // Creation du QPixmap
        int largeur = imageb->getDimension().getLargeur();
        int hauteur = imageb->getDimension().getHauteur();
        QImage im1(largeur, hauteur, QImage::Format_RGB32);
        for (int x = 0; x < largeur; x++)
            for (int y = 0; y < hauteur; y++)
            {
                bool valeur = imageb->getPixel(x, y);
                if (valeur)
                    im1.setPixel(x, y,
                                 qRgb(ImageB::couleurTrue.getRouge(), ImageB::couleurTrue.getVert(),
                                      ImageB::couleurTrue.getBleu()));
                else
                    im1.setPixel(x, y,
                                 qRgb(ImageB::couleurFalse.getRouge(), ImageB::couleurFalse.getVert(),
                                      ImageB::couleurFalse.getBleu()));
            }
        pixmap = new QPixmap(QPixmap::fromImage(im1));
    }

    // Mise en place du QPixmap
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    if (pixmap)
        label->setPixmap(*pixmap);
    else
        label->setText("vide");
    if (destination == "selection")
        ui->scrollAreaSelection->setWidget(label);
    if (destination == "operande1")
        ui->scrollAreaOperande1->setWidget(label);
    if (destination == "operande2")
        ui->scrollAreaOperande2->setWidget(label);
    if (destination == "resultat")
        ui->scrollAreaResultat->setWidget(label);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::setResultatBoolean(int val)
{
    if (val == 1) // vrai
    {
        ui->checkBoxResultat->setChecked(true);
        ui->checkBoxResultat->setText("VRAI");
        ui->checkBoxResultat->setStyleSheet("background-color: lightgreen;border: 1px solid black;");
        return;
    }
    if (val == 0) // faux
    {
        ui->checkBoxResultat->setChecked(false);
        ui->checkBoxResultat->setText("FAUX");
        ui->checkBoxResultat->setStyleSheet("background-color: red;border: 1px solid black;");
        return;
    }
    // ni vrai, ni faux
    ui->checkBoxResultat->setChecked(false);
    ui->checkBoxResultat->setText("Résultat");
    ui->checkBoxResultat->setStyleSheet("background-color: lightyellow;border: 1px solid black;");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des Images (ne pas modifier) ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::ajouteTupleTableImages(int id, string type, string dimension, string nom)
{
    char Id[20];
    sprintf(Id, "%d", id);

    char Type[20];
    strcpy(Type, type.c_str());

    char Dimension[20];
    strcpy(Dimension, dimension.c_str());

    char Nom[80];
    strcpy(Nom, nom.c_str());

    // Ajout possible
    int nbLignes = ui->tableWidgetImages->rowCount();
    nbLignes++;
    ui->tableWidgetImages->setRowCount(nbLignes);
    ui->tableWidgetImages->setRowHeight(nbLignes - 1, 20);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Id);
    ui->tableWidgetImages->setItem(nbLignes - 1, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Type);
    ui->tableWidgetImages->setItem(nbLignes - 1, 1, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Dimension);
    ui->tableWidgetImages->setItem(nbLignes - 1, 2, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setText(Nom);
    ui->tableWidgetImages->setItem(nbLignes - 1, 3, item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::videTableImages()
{
    ui->tableWidgetImages->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainWindowPhotoShop::getIndiceImageSelectionnee()
{
    QModelIndexList liste = ui->tableWidgetImages->selectionModel()->selectedRows();
    if (liste.size() == 0)
        return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Combox des traitements disponibles (ne pas modifier) ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::ajouteTraitementDisponible(string operation)
{
    ui->comboBoxTraitements->addItem(operation.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::videTraitementsDisponibles()
{
    ui->comboBoxTraitements->clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowPhotoShop::getTraitementSelectionne() const
{
    return ui->comboBoxTraitements->currentText().toStdString();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue (ne pas modifier) ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::dialogueMessage(const char *titre, const char *message)
{
    QMessageBox::information(this, titre, message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::dialogueErreur(const char *titre, const char *message)
{
    QMessageBox::critical(this, titre, message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowPhotoShop::dialogueDemandeTexte(const char *titre, const char *question)
{
    QString chaine = QInputDialog::getText(this, titre, question);
    return chaine.toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainWindowPhotoShop::dialogueDemandeInt(const char *titre, const char *question)
{
    return QInputDialog::getInt(this, titre, question);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MainWindowPhotoShop::dialogueDemandeFloat(const char *titre, const char *question)
{
    return QInputDialog::getDouble(this, titre, question);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowPhotoShop::dialogueDemandeFichierOuvrir(const char *question)
{
    QString fileName = QFileDialog::getOpenFileName(this, question, "", "Tous les fichiers (*.*)");
    return fileName.toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowPhotoShop::dialogueDemandeFichierEnregistrer(const char *question)
{
    QString fileName = QFileDialog::getSaveFileName(this, question, "", "Tous les fichiers (*.*)");
    return fileName.toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::dialogueDemandeCouleur(const char *message, int *pRouge, int *pVert, int *pBleu)
{
    QColor color = QColorDialog::getColor(Qt::red, this, message);
    *pRouge = color.red();
    *pVert = color.green();
    *pBleu = color.blue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Clic sur la croix de la fenêtre ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::closeEvent(QCloseEvent *event)
{
    if (event == NULL)
    {
    } // pour éviter le warning à la compilation
    PhotoShop::getInstance().Save();

    QApplication::exit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les items de menu //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionQuitter_triggered()
{
    PhotoShop::getInstance().Save();

    QApplication::exit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionCharger_ImageNB_triggered()
{
    string path = dialogueDemandeFichierOuvrir("Choisir une image NG");

    if (path.length() < 1)
        return;

    if (!validateFileExtension(path))
    {
        dialogueErreur("Erreur", "Fichier de type invalide");
        return;
    }

    Image *img = new ImageNG();

    reinterpret_cast<ImageNG *>(img)->importFromFile(path.c_str());
    img->setNom(path.c_str());

    PhotoShop::getInstance().ajouteImage(img);

    updateTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionCharger_ImageRGB_triggered()
{
    string path = dialogueDemandeFichierOuvrir("Choisir une image RGB");

    if (path.length() < 1)
        return;

    if (!validateFileExtension(path))
    {
        dialogueErreur("Erreur", "Fichier de type invalide");
        return;
    }

    Image *img = new ImageRGB();

    reinterpret_cast<ImageRGB *>(img)->importFromFile(path.c_str());
    img->setNom(path.c_str());

    PhotoShop::getInstance().ajouteImage(img);

    updateTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionEnregistrer_ImageNB_triggered()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
    {
        dialogueErreur("Erreur", "Pas d'image sélectionée");
        return;
    }

    Image *img = PhotoShop::getInstance().getImageParIndice(indice);

    if (getImageType(img) != "NG")
    {
        dialogueErreur("Erreur", "L'image n'est pas du type NG");
        return;
    }

    string path(dialogueDemandeFichierEnregistrer("Enregistrer une image"));

    if (path.length() < 1)
        return;

    if (!validateFileExtension(path))
    {
        dialogueErreur("Erreur", "extension incorrecte, extensions reconnues : bmp,jpg,png,");
        return;
    }

    img->exportToFile(path.c_str(), path.substr(path.find_last_of('.') + 1).c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionEnregistrer_ImageRGB_triggered()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
    {
        dialogueErreur("Erreur", "Pas d'image sélectionée");
        return;
    }

    Image *img = PhotoShop::getInstance().getImageParIndice(indice);

    if (getImageType(img) != "RGB")
    {
        dialogueErreur("Erreur", "L'image n'est pas du type RGB");
        return;
    }

    string path(dialogueDemandeFichierEnregistrer("Enregistrer une image"));

    if (path.length() < 1)
        return;

    if (!validateFileExtension(path))
    {
        dialogueErreur("Erreur", "extension incorrecte, extensions reconnues : bmp,jpg,png,");
        return;
    }

    img->exportToFile(path.c_str(), path.substr(path.find_last_of('.') + 1).c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionEnregistrer_ImageB_triggered()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
    {
        dialogueErreur("Erreur", "Pas d'image sélectionée");
        return;
    }

    Image *img = PhotoShop::getInstance().getImageParIndice(indice);

    if (getImageType(img) != "B")
    {
        dialogueErreur("Erreur", "L'image n'est pas du type B");
        return;
    }

    string path(dialogueDemandeFichierEnregistrer("Enregistrer une image"));

    if (path.length() < 1)
        return;

    if (!validateFileExtension(path))
    {
        dialogueErreur("Erreur", "extension incorrecte, extensions reconnues : bmp,jpg,png,");
        return;
    }

    img->exportToFile(path.c_str(), path.substr(path.find_last_of('.') + 1).c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionImage_selectionn_e_triggered()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
    {
        dialogueErreur("Erreur", "Pas d'image sélectionée");
        return;
    }

    PhotoShop::getInstance().supprimeImageParIndice(indice);
    updateTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionImage_par_id_triggered()
{
    PhotoShop::getInstance().supprimeImageParId(dialogueDemandeInt("Id", "Entrer un Id valide"));
    updateTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionCouleur_TRUE_pour_ImageB_triggered()
{
    int rouge(0), vert(0), bleu(0);

    dialogueDemandeCouleur("Choisir couleur True", &rouge, &vert, &bleu);

    ImageB::couleurTrue.setRouge(rouge);
    ImageB::couleurTrue.setVert(vert);
    ImageB::couleurTrue.setBleu(bleu);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionCouleur_FALSE_pour_imageB_triggered()
{
    int rouge(255), vert(255), bleu(255);

    dialogueDemandeCouleur("Choisir couleur False", &rouge, &vert, &bleu);

    ImageB::couleurFalse.setRouge(rouge);
    ImageB::couleurFalse.setVert(vert);
    ImageB::couleurFalse.setBleu(bleu);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionImporterCSV_triggered()
{
    string path = dialogueDemandeFichierOuvrir("choisir un fichier csv");

    if (path.length() < 1)
        return;

    if (path.substr(path.find_last_of('.') + 1) != "csv")
    {
        dialogueErreur("Erreur", "Seuls les fichiers csv sont supportés");
        return;
    }

    int nbImporte = PhotoShop::getInstance().importeImages(path);

    if (nbImporte == -1)
    {
        dialogueErreur("Erreur", "Erreur de lecture du fichier");
        return;
    }

    if (!nbImporte)
    {
        dialogueErreur("Erreur", "Aucune image n'a pu être importée");
        return;
    }

    updateTable();

    string msg = to_string(nbImporte);
    msg += " : images importées";

    dialogueMessage("csv", msg.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_actionReset_triggered()
{
    PhotoShop::getInstance().reset();
    ImageB::couleurFalse = Couleur::BLANC;
    ImageB::couleurTrue = Couleur::NOIR;
    updateTable();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions selection sur la table des images ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_tableWidgetImages_itemSelectionChanged()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
    {
        setImageNG("selection");
        return;
    }

    Image *img = PhotoShop::getInstance().getImageParIndice(indice);

    setNomImage(img->getNom());

    string type = getImageType(img);

    if (type == "NG")
    {
        ImageNG *imgNG = reinterpret_cast<ImageNG *>(img);
        setParametresImageNG(imgNG->getMaximum(), imgNG->getMinimum(), imgNG->getLuminance(), imgNG->getContraste());
        setImageNG("selection", imgNG);
    }
    else
    {
        setParametresImageNG();
        if (type == "RGB")
        {
            setImageRGB("selection", reinterpret_cast<ImageRGB *>(img));
        }
        else
        {
            setImageB("selection", reinterpret_cast<ImageB *>(img));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonModifierNom_clicked()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
        return;

    string nom = getNomImage();

    if (nom.length() < 1)
    {
        dialogueErreur("Erreur", "Le nom ne peut pas être vide");
        return;
    }

    PhotoShop::getInstance().getImageParIndice(indice)->setNom(nom.c_str());
    updateTable();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonOperande1_clicked()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
        return;

    PhotoShop::operande1 = PhotoShop::getInstance().getImageParIndice(indice);

    string type = getImageType(PhotoShop::operande1);

    if (type == "NG")
    {
        ImageNG *imgNG = reinterpret_cast<ImageNG *>(PhotoShop::operande1);
        setParametresImageNG(imgNG->getMaximum(), imgNG->getMinimum(), imgNG->getLuminance(), imgNG->getContraste());
        setImageNG("operande1", imgNG);
    }
    else
    {
        setParametresImageNG();
        if (type == "RGB")
        {
            setImageRGB("operande1", reinterpret_cast<ImageRGB *>(PhotoShop::operande1));
        }
        else
        {
            setImageB("operande1", reinterpret_cast<ImageB *>(PhotoShop::operande1));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonSupprimeOperande1_clicked()
{
    PhotoShop::operande1 = nullptr;

    setImageNG("operande1");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonOperande2_clicked()
{
    int indice;

    if ((indice = getIndiceImageSelectionnee()) < 0)
        return;

    PhotoShop::operande2 = PhotoShop::getInstance().getImageParIndice(indice);

    string type = getImageType(PhotoShop::operande2);

    if (type == "NG")
    {
        ImageNG *imgNG = reinterpret_cast<ImageNG *>(PhotoShop::operande2);
        setParametresImageNG(imgNG->getMaximum(), imgNG->getMinimum(), imgNG->getLuminance(), imgNG->getContraste());
        setImageNG("operande2", imgNG);
    }
    else
    {
        setParametresImageNG();
        if (type == "RGB")
        {
            setImageRGB("operande2", reinterpret_cast<ImageRGB *>(PhotoShop::operande2));
        }
        else
        {
            setImageB("operande2", reinterpret_cast<ImageB *>(PhotoShop::operande2));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonSupprimerOperande2_clicked()
{
    PhotoShop::operande2 = nullptr;

    setImageNG("operande2");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonResultat_clicked()
{
    if (!PhotoShop::resultat)
        return;

    PhotoShop::getInstance().ajouteImage(PhotoShop::resultat);
    updateTable();
    PhotoShop::resultat = nullptr;
    setImageNG("resultat");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonSupprimerResultat_clicked()
{
    if (!PhotoShop::resultat)
        return;

    delete PhotoShop::resultat;
    PhotoShop::resultat = nullptr;
    setImageNG("resultat");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowPhotoShop::on_pushButtonTraitement_clicked()
{
    setResultatBoolean();

    if (PhotoShop::resultat)
        on_pushButtonSupprimerResultat_clicked();

    if (!PhotoShop::operande1)
    {
        dialogueErreur("Erreur", "L'opérande 1 ne peut être nulle");
        return;
    }

    string traitement = getTraitementSelectionne();
    string type1 = getImageType(PhotoShop::operande1);
    cout << traitement << endl;
    try
    {

        if (traitement == "Eclaircir (+ val)")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Éclaircir", "Entrer une valeur");
                PhotoShop::resultat = new ImageNG(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) + val);
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Eclaircir (++)")
        {
            if (type1 == "NG")
            {
                PhotoShop::resultat = new ImageNG(*reinterpret_cast<ImageNG *>(PhotoShop::operande1));
                (*reinterpret_cast<ImageNG *>(PhotoShop::resultat))++;
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Assombrir (- val)")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Assombrir", "Entrer une valeur");
                PhotoShop::resultat = new ImageNG(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) - val);
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Assombrir (--)")
        {
            if (type1 == "NG")
            {
                PhotoShop::resultat = new ImageNG(*reinterpret_cast<ImageNG *>(PhotoShop::operande1));
                (*reinterpret_cast<ImageNG *>(PhotoShop::resultat))--;
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Différence")
        {
            if (!PhotoShop::operande2)
            {
                dialogueErreur("Erreur", "Une seconde opérande est nécéssaire pour cette  opération");
                return;
            }

            string type2 = getImageType(PhotoShop::operande2);

            if (type1 != type2)
            {
                dialogueErreur("Erreur", "Les 2 opérandes sont de type différent");
                return;
            }

            if (type1 == "NG")
            {
                PhotoShop::resultat = new ImageNG(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) -
                                                  *reinterpret_cast<ImageNG *>(PhotoShop::operande2));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Comparaison (==)")
        {
            if (!PhotoShop::operande2)
            {
                dialogueErreur("Erreur", "Une seconde opérande est nécéssaire pour cette  opération");
                return;
            }

            string type2 = getImageType(PhotoShop::operande2);

            if (type1 != type2)
            {
                dialogueErreur("Erreur", "Les 2 opérandes sont de type différent");
                return;
            }

            if (type1 == "NG")
            {
                setResultatBoolean(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) ==
                                   *reinterpret_cast<ImageNG *>(PhotoShop::operande2));
                return;
            }
        }
        else if (traitement == "Comparaison (<)")
        {
            if (!PhotoShop::operande2)
            {
                dialogueErreur("Erreur", "Une seconde opérande est nécéssaire pour cette  opération");
                return;
            }

            string type2 = getImageType(PhotoShop::operande2);

            if (type1 != type2)
            {
                dialogueErreur("Erreur", "Les 2 opérandes sont de type différent");
                return;
            }

            if (type1 == "NG")
            {
                setResultatBoolean(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) <
                                   *reinterpret_cast<ImageNG *>(PhotoShop::operande2));
                return;
            }
        }
        else if (traitement == "Comparaison (>)")
        {
            if (!PhotoShop::operande2)
            {
                dialogueErreur("Erreur", "Une seconde opérande est nécéssaire pour cette  opération");
                return;
            }

            string type2 = getImageType(PhotoShop::operande2);

            if (type1 != type2)
            {
                dialogueErreur("Erreur", "Les 2 opérandes sont de type différent");
                return;
            }

            if (type1 == "NG")
            {
                setResultatBoolean(*reinterpret_cast<ImageNG *>(PhotoShop::operande1) >
                                   *reinterpret_cast<ImageNG *>(PhotoShop::operande2));
                return;
            }
        }
        else if (traitement == "Seuillage")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Seuillage", "Entrer la valeur du seuil");
                PhotoShop::resultat =
                    new ImageB(Traitements::Seuillage(*reinterpret_cast<ImageNG *>(PhotoShop::operande1), val));
                setImageB("resultat", reinterpret_cast<ImageB *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Filtre moyenneur")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Filtre moyenneur", "Entrer la taille du filtre");
                PhotoShop::resultat =
                    new ImageNG(Traitements::FiltreMoyenneur(*reinterpret_cast<ImageNG *>(PhotoShop::operande1), val));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Filtre médian")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Filtre médian", "Entrer la taille du filtre");
                PhotoShop::resultat =
                    new ImageNG(Traitements::FiltreMedian(*reinterpret_cast<ImageNG *>(PhotoShop::operande1), val));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Erosion")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Erosion", "Entrer la taille du filtre");
                PhotoShop::resultat =
                    new ImageNG(Traitements::Erosion(*reinterpret_cast<ImageNG *>(PhotoShop::operande1), val));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Dilatation")
        {
            int val;

            if (type1 == "NG")
            {
                val = dialogueDemandeInt("Dilatation", "Entrer la taille du filtre");
                PhotoShop::resultat =
                    new ImageNG(Traitements::Dilatation(*reinterpret_cast<ImageNG *>(PhotoShop::operande1), val));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
        else if (traitement == "Négatif")
        {
            if (type1 == "NG")
            {
                PhotoShop::resultat =
                    new ImageNG(Traitements::Negatif(*reinterpret_cast<ImageNG *>(PhotoShop::operande1)));
                setImageNG("resultat", reinterpret_cast<ImageNG *>(PhotoShop::resultat));
                return;
            }
        }
    }
    catch (XYException &e)
    {
        dialogueErreur("Erreur", e.getMsg().c_str());
    }
    catch (RGBException &e)
    {
        dialogueErreur("Erreur", e.getMsg().c_str());
    }
    catch (Exception &e)
    {
        dialogueErreur("Erreur", e.getMsg().c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MainWindowPhotoShop::validateFileExtension(const std::string &filename)
{
    // Regular expression to match valid file extensions: jpg, bmp, png
    std::regex extension_regex(R"(.*\.(jpg|bmp|png)$)", std::regex::icase);

    // Check if the filename matches the regular expression
    return std::regex_match(filename, extension_regex);
}

void MainWindowPhotoShop::updateTable()
{
    videTableImages();

    int i = 0;
    Image *img;

    while ((img = PhotoShop::getInstance().getImageParIndice(i)))
    {
        ajouteTupleTableImages(img->getId(), getImageType(img),
                               to_string(img->getDimension().getLargeur()) + "x" +
                                   to_string(img->getDimension().getHauteur()),
                               img->getNom());
        i++;
    }

    PhotoShop::getInstance().afficheImages();
}

string MainWindowPhotoShop::getImageType(Image *image)
{
    string type;

    if (dynamic_cast<ImageNG *>(image))
    {
        type = "NG";
    }
    else if (dynamic_cast<ImageRGB *>(image))
    {
        type = "RGB";
    }
    else
    {
        type = "B";
    }

    return type;
}