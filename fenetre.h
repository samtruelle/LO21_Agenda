MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), pm(ProjetManager::getInstance()),
    em(EvenementManager::getInstance()), currentProjet(0), firstDay(0), firstDate(QDate(2000, 1, 1)) {
    ui->setupUi(this);

    // Général
    QObject::connect(ui->btnChangerSemaine, SIGNAL (clicked()), this, SLOT (dateEditModifie()));
    QObject::connect(ui->btnQuitter, SIGNAL (clicked()), this, SLOT (close()));

    // Partie Projet - Gestion projets
    QObject::connect(ui->btnNouveauProjet, SIGNAL (clicked()), this, SLOT (nouveauProjet()));
    QObject::connect(ui->btnSupprimerProjet, SIGNAL (clicked()), this, SLOT (supprimerProjet()));
    QObject::connect(ui->btnChargerProjet, SIGNAL (clicked()), this, SLOT (rechargerAffichage()));

    // Partie Projet - Gestion tâches
    QObject::connect(ui->btnAjouterTacheUNP, SIGNAL (clicked()), this, SLOT (ajouterTacheNonPreemptableNonPlanifiee()));
    QObject::connect(ui->btnAjouterTacheUP, SIGNAL (clicked()), this, SLOT (ajouterTachePreemptable()));
    QObject::connect(ui->btnAjouterTacheC, SIGNAL (clicked()), this, SLOT (ajouterTacheComposite()));
    QObject::connect(ui->btnAjouterPlanifierTacheU, SIGNAL (clicked()), this, SLOT (ajouterTacheNonPreemptablePlanifiee()));
    QObject::connect(ui->btnSupprimerTache, SIGNAL (clicked()), this, SLOT (supprimerTache()));
    QObject::connect(ui->btnAjouterTacheCP, SIGNAL (clicked()), this, SLOT (ajouterTacheComposantePreemptable()));
    QObject::connect(ui->btnAjouterTacheCNP, SIGNAL (clicked()), this, SLOT (ajouterTacheComposanteUnitaireNP()));
    QObject::connect(ui->btnPlanifierTacheU, SIGNAL (clicked()), this, SLOT (planifierTacheUnitaireNP()));
    QObject::connect(ui->btnAjouterPartie, SIGNAL (clicked()), this, SLOT (planifierPartie()));
    QObject::connect(ui->exportCalendrier, SIGNAL (clicked()), this, SLOT (exporterCalXML()));


    // Partie Calendrier - Gestion Evenements
    QObject::connect(ui->ajoutRDV, SIGNAL (clicked()), this, SLOT (ajoutRDV()));
    QObject::connect(ui->ajoutEvenement, SIGNAL (clicked()), this, SLOT (ajoutEvenement()));
    QObject::connect(ui->ajoutReunion, SIGNAL (clicked()), this, SLOT (ajoutReunion()));
    QObject::connect(ui->supprimerEvt, SIGNAL (clicked()), this, SLOT (supprimerEvt()));

    //Date sélectionnée par défaut dans le sélecteur de date
    ui->dateEdit->setDate(QDate::currentDate());
    dateEditModifie();
    rechargerAffichage();
}

MainWindow::~MainWindow() {
    pm.freeInstance();
    delete ui;
}

void MainWindow::dateEditModifie() {
    QDate nouvelleDate = ui->dateEdit->date();

    // Recherche du lundi de la semaine
    for(unsigned int i = 1; i < 7; i++) {
        if(nouvelleDate.dayOfWeek() == 1) break; // car Lundi <=> dayOfWeek() = 1
        nouvelleDate = nouvelleDate.addDays(-1);
    }

    firstDay = nouvelleDate.dayOfWeek();
    firstDate = nouvelleDate;

   // Affichage des 7 jours de la semaine
   for(unsigned int i = 0; i < 7; i++) {
       ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(nouvelleDate.toString("ddd d MMMM")));
       nouvelleDate = nouvelleDate.addDays(1);
   }

   // Recharger l'affichage des évènements et projets sur le calendrier
    rechargerAffichage();
}

void MainWindow::nouveauProjet() {
    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel("Ajout nouveau projet"));

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);
    lineEdit3->setDate(QDate::currentDate());

    form.addRow("Nom du projet :", lineEdit1);
    form.addRow("Début du projet :", lineEdit2);
    form.addRow("Fin du projet :", lineEdit3);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if (pm.testProjet(lineEdit1->text())) throw CalendarException("Un projet de ce nom existe déjà");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date()>lineEdit3->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            pm.addProjet(lineEdit1->text(), lineEdit2->date(), lineEdit3->date());
            ui->listeProjets->addItem(lineEdit1->text());
            ui->listeProjets->setCurrentIndex(ui->listeProjets->findText(lineEdit1->text()));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::supprimerProjet() {
    try {
        std::vector<Tache*> iter = currentProjet->getTaches();

        for(std::vector<Tache*>::iterator it = iter.begin(); it != iter.end(); it++) {
            em.delEvent((*it)->getNomTache());
        }

        delete currentProjet;
        currentProjet = NULL;

        pm.delProjet(ui->listeProjets->currentText());
        ui->listeProjets->removeItem(ui->listeProjets->currentIndex());
        rechargerAffichage();
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::rechargerAffichage() {
    // Suppression de l'affichage du calendrier
    ui->tableWidget->clearContents();
    ui->listeTaches->clear();

    // Affichage des évènements
    std::vector<Evenement*> iter = em.getEvenements();

    for(std::vector<Evenement*>::iterator it = iter.begin(); it != iter.end(); it++) {
        int heureAdditionnelle = 0;
        for(unsigned int nbHeures = 0; nbHeures < (*it)->getDuree(); nbHeures++) {
            if((*it)->getDate() >= firstDate && (*it)->getDate() <= firstDate.addDays(7)) {
                QTableWidgetItem *setdes = new QTableWidgetItem;
                setdes->setText((*it)->getDescription());
                ui->tableWidget->setItem(((*it)->getHeureDebut())+heureAdditionnelle, ((*it)->getDate()).dayOfWeek()-firstDay, setdes);
                heureAdditionnelle++;
            }
        }
    }

    // Affichage du projet sélectionné

    // Charger ici les tâches relatives au projet sélectionné :
    // - dans le calendrier
    // - dans le tree view

    if(ui->listeProjets->currentText() != "") currentProjet = &pm.getProjet(ui->listeProjets->currentText());

    if(currentProjet != NULL) {
        std::vector<Tache*> iter = currentProjet->getTaches();

        for(std::vector<Tache*>::iterator it = iter.begin(); it != iter.end(); it++) {
            // Affichage des tâches non composées dans le treeView
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList((*it)->getNomTache())));
        }
    }

    // Amélioration de l'affichage
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::ajouterTacheNonPreemptablePlanifiee() {
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle tâche unitaire non préemptable planifiée");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);//nom
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);//dateDispo
    lineEdit2->setDate(QDate::currentDate());
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);//dateEcheance
    lineEdit3->setDate(QDate::currentDate());
    QComboBox *lineEdit4 = new QComboBox(&dialog);// Durée
    lineEdit4->addItem("1");
    lineEdit4->addItem("2");
    lineEdit4->addItem("3");
    lineEdit4->addItem("4");
    lineEdit4->addItem("5");
    lineEdit4->addItem("6");
    lineEdit4->addItem("7");
    lineEdit4->addItem("8");
    lineEdit4->addItem("9");
    lineEdit4->addItem("10");
    lineEdit4->addItem("11");
    lineEdit4->addItem("12");
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);//Horaire(heures)
    QDateEdit *lineEdit7 = new QDateEdit(&dialog);//DatePlanification
    lineEdit7->setDate(QDate::currentDate());

    form.addRow("Nom :", lineEdit1);
    form.addRow("Début :", lineEdit2);
    form.addRow("Fin :", lineEdit3);
    form.addRow("Duree en heures :", lineEdit4);
    form.addRow("Date de planification :", lineEdit7);
    form.addRow("Heure début :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");

        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if( lineEdit3->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if( lineEdit7->date()< lineEdit2->date()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}
            if(lineEdit7->date() > lineEdit3->date()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}
            if(lineEdit2->date()>lineEdit3->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TacheNonPreemptable* tache = new TacheNonPreemptable(lineEdit1->text(), lineEdit2->date(), lineEdit3->date(), lineEdit4->currentText().toInt()*60, Horaire(lineEdit5->text().toInt(), 0),lineEdit7->date());
            currentProjet->addTache(tache);
            em.addEvent(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::ajouterTacheNonPreemptableNonPlanifiee() {
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle tâche unitaire non préemptable sans planification");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);//nom
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);//dateDispo
    lineEdit2->setDate(QDate::currentDate());
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);//dateEcheance
    lineEdit3->setDate(QDate::currentDate());
    QComboBox *lineEdit4 = new QComboBox(&dialog);// Durée
    lineEdit4->addItem("1");
    lineEdit4->addItem("2");
    lineEdit4->addItem("3");
    lineEdit4->addItem("4");
    lineEdit4->addItem("5");
    lineEdit4->addItem("6");
    lineEdit4->addItem("7");
    lineEdit4->addItem("8");
    lineEdit4->addItem("9");
    lineEdit4->addItem("10");
    lineEdit4->addItem("11");
    lineEdit4->addItem("12");

    form.addRow("Nom de la tâche :", lineEdit1);
    form.addRow("Début de la tâche :", lineEdit2);
    form.addRow("Fin de la tâche :", lineEdit3);
    form.addRow("Duree en heure :", lineEdit4);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if( lineEdit3->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if(lineEdit2->date()>lineEdit3->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TacheNonPreemptable* tache = new TacheNonPreemptable(lineEdit1->text(), lineEdit2->date(), lineEdit3->date(), lineEdit4->currentText().toInt()*60);
            currentProjet->addTache(tache);
            em.addEvent(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::ajouterTacheComposanteUnitaireNP() {
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle tâche unitaire non préemptable composante");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);//nom de la tache composite
    QLineEdit *lineEdit2 = new QLineEdit(&dialog);//nom de la tache composante
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);//dateDispo
    lineEdit3->setDate(QDate::currentDate());
    QDateEdit *lineEdit4 = new QDateEdit(&dialog);//dateEcheance
    lineEdit4->setDate(QDate::currentDate());
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);//dureee

    form.addRow("Nom de la tâche composite :", lineEdit1);
    form.addRow("Nom de la tâche composante:", lineEdit2);
    form.addRow("Début de la tâche :", lineEdit3);
    form.addRow("Fin de la tâche :", lineEdit4);
    form.addRow("Duree (<12h) :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit3->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if( lineEdit4->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if(lineEdit3->date()>lineEdit4->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TacheNonPreemptable* tache = new TacheNonPreemptable(lineEdit2->text(), lineEdit3->date(), lineEdit4->date(), lineEdit5->text().toInt()*60);
            currentProjet->getTache<TacheComposite>(lineEdit1->text()).addTacheComposante(tache);
            currentProjet->addTache(tache);
            em.addEvent(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::ajouterTacheComposantePreemptable() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouvelle tâche composante préemptable");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QLineEdit *lineEdit2 = new QLineEdit(&dialog);
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);
    lineEdit3->setDate(QDate::currentDate());
    QDateEdit *lineEdit4 = new QDateEdit(&dialog);
    lineEdit4->setDate(QDate::currentDate());
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);

    form.addRow("Nom de la tâche composite :", lineEdit1);
    form.addRow("Nom de la tâche composante :", lineEdit2);
    form.addRow("Début de la tâche :", lineEdit3);
    form.addRow("Fin de la tâche :", lineEdit4);
    form.addRow("Duree :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit3->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if(lineEdit4->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if(lineEdit3->date()>lineEdit4->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TacheComposite tacheC =  currentProjet->getTache<TacheComposite>(lineEdit1->text());
            TachePreemptable* tache = new TachePreemptable(lineEdit2->text(), lineEdit3->date(), lineEdit4->date(), lineEdit5->text().toInt()*60);
            currentProjet->addTache(tache);
            tacheC.addTacheComposante(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::planifierTacheUnitaireNP() {
    QDialog dialog(this);
    dialog.setWindowTitle("Planification tâche unitaire non préemptable");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);//nom
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);//Horaire(heures)
    QDateEdit *lineEdit7 = new QDateEdit(&dialog);//DatePlanification
    lineEdit7->setDate(QDate::currentDate());

    form.addRow("Nom :", lineEdit1);
    form.addRow("Date de planification :", lineEdit7);
    form.addRow("Heure début :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit7->date() < currentProjet->getTache<TacheNonPreemptable>(lineEdit1->text()).getDateDispo()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}
            if(lineEdit7->date() > currentProjet->getTache<TacheNonPreemptable>(lineEdit1->text()).getDateEcheance()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}

            currentProjet->getTache<TacheNonPreemptable>(lineEdit1->text()).TacheNonPreemptable::setDate(lineEdit7->date());
            currentProjet->getTache<TacheNonPreemptable>(lineEdit1->text()).TacheNonPreemptable::setHoraire(TIME::Horaire(lineEdit5->text().toInt(),0));

            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::planifierPartie() {
    QDialog dialog(this);
    dialog.setWindowTitle("Planification Partie");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);//nom Tache Préemptable
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);//Date de planification
    lineEdit2->setDate(QDate::currentDate());
    QLineEdit *lineEdit3 = new QLineEdit(&dialog);//Horaire
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);//id
    QLineEdit *lineEdit7 = new QLineEdit(&dialog);//duree

    form.addRow("Nom de la tâche préemptable:", lineEdit1);
    form.addRow("Date de planification :", lineEdit2);
    form.addRow("Heure de planification :", lineEdit3);
    form.addRow("Duree de la partie :", lineEdit7);
    form.addRow("ID de la partie :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date() < currentProjet->getTache<TachePreemptable>(lineEdit1->text()).getDateDispo()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}
            if(lineEdit2->date() > currentProjet->getTache<TachePreemptable>(lineEdit1->text()).getDateEcheance()){ throw CalendarException("Impossible de planifier la tache en dehors du créneau définit");}

            bool ok;
            Partie * p = new Partie(lineEdit2->date(),lineEdit1->text(),Horaire(lineEdit3->text().toInt(),0),Duree(lineEdit7->text().toInt(),0),lineEdit5->text().toInt(&ok));
            currentProjet->getTache<TachePreemptable>(lineEdit1->text()).addPartie(p);
            em.addEvent(p);
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::ajouterTachePreemptable() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouvelle tâche unitaire préemptable");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);
    lineEdit3->setDate(QDate::currentDate());
    QLineEdit *lineEdit4 = new QLineEdit(&dialog);

    form.addRow("Nom de la tâche :", lineEdit1);
    form.addRow("Début de la tâche :", lineEdit2);
    form.addRow("Fin de la tâche :", lineEdit3);
    form.addRow("Duree :", lineEdit4);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if( lineEdit3->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if(lineEdit2->date()>lineEdit3->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TachePreemptable* tache = new TachePreemptable(lineEdit1->text(), lineEdit2->date(), lineEdit3->date(), lineEdit4->text().toInt()*60);
            currentProjet->addTache(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }

}

void MainWindow::ajouterTacheComposite() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouvelle tâche composite");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QDateEdit *lineEdit3 = new QDateEdit(&dialog);
    lineEdit3->setDate(QDate::currentDate());

    form.addRow("Nom de la tâche :", lineEdit1);
    form.addRow("Début de la tâche :", lineEdit2);
    form.addRow("Fin de la tâche :", lineEdit3);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    try {
        if(ui->listeProjets->currentText() == "") throw CalendarException("Projet Inexistant");
        if (dialog.exec() == QDialog::Accepted) {
            if(lineEdit2->date() < currentProjet->getDateDispo() ) { throw CalendarException("Date de début de  tache antérieur au projet");}
            if( lineEdit3->date()> currentProjet->getEcheance()) { throw CalendarException("Date de fin de tache ultérieur au projet");}
            if(lineEdit2->date()>lineEdit3->date()) throw CalendarException ("Date de debut ulterieure à date de fin");
            TacheComposite* tache = new TacheComposite(lineEdit1->text(), lineEdit2->date(), lineEdit3->date());
            curProjet().addTache(tache);
            ui->listeTaches->addTopLevelItem(new QTreeWidgetItem(QStringList(lineEdit1->text())));
            rechargerAffichage();
        }
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::supprimerTache() {
    try {
        currentProjet->delTache(ui->listeTaches->currentItem()->text(ui->listeTaches->currentColumn()));
        if(em.testEvenement(ui->listeTaches->currentItem()->text(ui->listeTaches->currentColumn()))) em.delEvent(ui->listeTaches->currentItem()->text(ui->listeTaches->currentColumn()));
        rechargerAffichage();
    } catch(CalendarException e) {
        e.getInfoBox();
    }
}

void MainWindow::exporterCalXML() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Export XML"), QDir::currentPath(), tr("Documents (*.xml)"));
    pm.exportXML(filename);
}

void MainWindow::ajoutEvenement() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouvel évènement traditionnel");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QLineEdit *lineEdit3 = new QLineEdit(&dialog);
    QLineEdit *lineEdit4 = new QLineEdit(&dialog);
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);

    form.addRow("Titre :", lineEdit1);
    form.addRow("Date  :", lineEdit2);
    form.addRow("Heure début :", lineEdit3);
    form.addRow("Durée (min) :", lineEdit4);
    form.addRow("Lieu :", lineEdit5);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        ActiviteTrad* e = new ActiviteTrad(lineEdit2->date(), lineEdit1->text(), Horaire(lineEdit3->text().toInt(), 00), Duree(lineEdit4->text().toInt()), lineEdit5->text());
        em.addEvent(e);
        rechargerAffichage();
    }
}

void MainWindow::ajoutReunion() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouvelle réunion");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QLineEdit *lineEdit3 = new QLineEdit(&dialog);
    QLineEdit *lineEdit4 = new QLineEdit(&dialog);
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);
    QLineEdit *lineEdit6 = new QLineEdit(&dialog);

    form.addRow("Titre :", lineEdit1);
    form.addRow("Date  :", lineEdit2);
    form.addRow("Heure début :", lineEdit3);
    form.addRow("Durée (min) :", lineEdit4);
    form.addRow("Participants :", lineEdit5);
    form.addRow("Lieu :", lineEdit6);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        Reunion* e = new Reunion(lineEdit2->date(), lineEdit1->text(), Horaire(lineEdit3->text().toInt(), 00), Duree(lineEdit4->text().toInt()), lineEdit5->text(), lineEdit6->text());
        em.addEvent(e);
        rechargerAffichage();
    }
}

void MainWindow::ajoutRDV() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajout nouveau rendez-vous");
    QFormLayout form(&dialog);

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QDateEdit *lineEdit2 = new QDateEdit(&dialog);
    lineEdit2->setDate(QDate::currentDate());
    QLineEdit *lineEdit3 = new QLineEdit(&dialog);
    QLineEdit *lineEdit4 = new QLineEdit(&dialog);
    QLineEdit *lineEdit5 = new QLineEdit(&dialog);
    QLineEdit *lineEdit6 = new QLineEdit(&dialog);

    form.addRow("Titre :", lineEdit1);
    form.addRow("Date  :", lineEdit2);
    form.addRow("Heure début :", lineEdit3);
    form.addRow("Durée (min) :", lineEdit4);
    form.addRow("Participant :", lineEdit5);
    form.addRow("Lieu :", lineEdit6);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        Rdv* e = new Rdv(lineEdit2->date(), lineEdit1->text(), Horaire(lineEdit3->text().toInt(), 00), Duree(lineEdit4->text().toInt()), lineEdit5->text(), lineEdit6->text());
        em.addEvent(e);
        rechargerAffichage();
    }
}

void MainWindow::supprimerEvt() {
    em.delEvent(ui->tableWidget->currentItem()->text());
    rechargerAffichage();
}
