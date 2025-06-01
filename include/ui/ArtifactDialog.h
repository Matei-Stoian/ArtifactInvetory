//
// Created by matei on 31/05/2025.
//

#ifndef ARTIFACTDIALOG_H
#define ARTIFACTDIALOG_H



#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>

#include "../domain//Artifact.h" // Include your Artifact class

class ArtifactDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor for adding a new artifact
    explicit ArtifactDialog(QWidget *parent = nullptr);
    // Constructor for editing an existing artifact
    explicit ArtifactDialog(const Artifact& artifact, QWidget *parent = nullptr);
    ~ArtifactDialog() override = default;

    // Method to get the artifact data from the dialog
    Artifact getArtifact() const;

private:
    // UI elements
    QLineEdit *catalogNumberEdit;
    QLineEdit *nameEdit;
    QComboBox *typeCombo;
    QLineEdit *materialEdit;
    QLineEdit *cultureEdit;
    QLineEdit *periodEdit;
    QLineEdit *siteEdit;
    QLineEdit *coordinatesEdit;
    QDateEdit *discoveryDateEdit;
    QComboBox *conditionCombo;
    QTextEdit *descriptionEdit;
    QDoubleSpinBox *lengthSpinBox;
    QDoubleSpinBox *widthSpinBox;
    QDoubleSpinBox *heightSpinBox;
    QDoubleSpinBox *weightSpinBox;
    QLineEdit *storageLocationEdit;
    QCheckBox *onDisplayCheckBox;
    QDialogButtonBox *buttonBox;

    // Helper to populate the type combo box
    void populateTypeComboBox();
    // Helper to populate the condition combo box
    void populateConditionComboBox();
    // Helper to setup the common UI layout
    void setupUi();
    // Helper to load artifact data into the UI fields (for editing)
    void loadArtifactData(const Artifact& artifact);
};



#endif //ARTIFACTDIALOG_H
