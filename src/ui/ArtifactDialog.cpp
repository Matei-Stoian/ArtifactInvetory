//
// Created by matei on 31/05/2025.
//

#include "../../include/ui/ArtifactDialog.h"

ArtifactDialog::ArtifactDialog(QWidget *parent)
    : QDialog(parent), discoveryDateEdit(nullptr) {
    setWindowTitle(tr("Add New Artifact"));
    setupUi();
    // Set default values for new artifact
    discoveryDateEdit->setDate(QDate::currentDate());
}

// Constructor for editing an existing artifact
ArtifactDialog::ArtifactDialog(const Artifact& artifact, QWidget *parent)
    : QDialog(parent), catalogNumberEdit(nullptr) {
    setWindowTitle(tr("Edit Artifact"));
    setupUi();
    loadArtifactData(artifact);
    // Catalog number should not be editable when editing
    catalogNumberEdit->setReadOnly(true);
}

void ArtifactDialog::setupUi()
{
    QGridLayout *mainLayout = new QGridLayout(this);

    // Row 0: Catalog Number
    mainLayout->addWidget(new QLabel(tr("Catalog Number:")), 0, 0);
    catalogNumberEdit = new QLineEdit(this);
    mainLayout->addWidget(catalogNumberEdit, 0, 1);

    // Row 1: Name
    mainLayout->addWidget(new QLabel(tr("Name:")), 1, 0);
    nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameEdit, 1, 1);

    // Row 2: Type
    mainLayout->addWidget(new QLabel(tr("Type:")), 2, 0);
    typeCombo = new QComboBox(this);
    populateTypeComboBox();
    mainLayout->addWidget(typeCombo, 2, 1);

    // Row 3: Material
    mainLayout->addWidget(new QLabel(tr("Material:")), 3, 0);
    materialEdit = new QLineEdit(this);
    mainLayout->addWidget(materialEdit, 3, 1);

    // Row 4: Culture
    mainLayout->addWidget(new QLabel(tr("Culture:")), 4, 0);
    cultureEdit = new QLineEdit(this);
    mainLayout->addWidget(cultureEdit, 4, 1);

    // Row 5: Period
    mainLayout->addWidget(new QLabel(tr("Period:")), 5, 0);
    periodEdit = new QLineEdit(this);
    mainLayout->addWidget(periodEdit, 5, 1);

    // Row 6: Site
    mainLayout->addWidget(new QLabel(tr("Site:")), 6, 0);
    siteEdit = new QLineEdit(this);
    mainLayout->addWidget(siteEdit, 6, 1);

    // Row 7: Coordinates
    mainLayout->addWidget(new QLabel(tr("Coordinates:")), 7, 0);
    coordinatesEdit = new QLineEdit(this);
    mainLayout->addWidget(coordinatesEdit, 7, 1);

    // Row 8: Discovery Date
    mainLayout->addWidget(new QLabel(tr("Discovery Date:")), 8, 0);
    discoveryDateEdit = new QDateEdit(this);
    discoveryDateEdit->setCalendarPopup(true);
    mainLayout->addWidget(discoveryDateEdit, 8, 1);

    // Row 9: Condition
    mainLayout->addWidget(new QLabel(tr("Condition:")), 9, 0);
    conditionCombo = new QComboBox(this);
    populateConditionComboBox();
    mainLayout->addWidget(conditionCombo, 9, 1);

    // Row 10: Description (multi-line)
    mainLayout->addWidget(new QLabel(tr("Description:")), 10, 0, 1, 2); // Span 2 columns
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setPlaceholderText(tr("Enter a detailed description..."));
    mainLayout->addWidget(descriptionEdit, 11, 0, 1, 2); // Span 2 columns

    // Row 12: Dimensions (Length, Width, Height, Weight)
    mainLayout->addWidget(new QLabel(tr("Length (cm):")), 12, 0);
    lengthSpinBox = new QDoubleSpinBox(this);
    lengthSpinBox->setRange(0.0, 10000.0); // Example range
    lengthSpinBox->setDecimals(2);
    mainLayout->addWidget(lengthSpinBox, 12, 1);

    mainLayout->addWidget(new QLabel(tr("Width (cm):")), 13, 0);
    widthSpinBox = new QDoubleSpinBox(this);
    widthSpinBox->setRange(0.0, 10000.0);
    widthSpinBox->setDecimals(2);
    mainLayout->addWidget(widthSpinBox, 13, 1);

    mainLayout->addWidget(new QLabel(tr("Height (cm):")), 14, 0);
    heightSpinBox = new QDoubleSpinBox(this);
    heightSpinBox->setRange(0.0, 10000.0);
    heightSpinBox->setDecimals(2);
    mainLayout->addWidget(heightSpinBox, 14, 1);

    mainLayout->addWidget(new QLabel(tr("Weight (grams):")), 15, 0);
    weightSpinBox = new QDoubleSpinBox(this);
    weightSpinBox->setRange(0.0, 1000000.0); // Example range for grams
    weightSpinBox->setDecimals(2);
    mainLayout->addWidget(weightSpinBox, 15, 1);

    // Row 16: Storage Location
    mainLayout->addWidget(new QLabel(tr("Storage Location:")), 16, 0);
    storageLocationEdit = new QLineEdit(this);
    mainLayout->addWidget(storageLocationEdit, 16, 1);

    // Row 17: On Display Checkbox
    onDisplayCheckBox = new QCheckBox(tr("On Display"), this);
    mainLayout->addWidget(onDisplayCheckBox, 17, 0, 1, 2); // Span 2 columns

    // Row 18: Buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ArtifactDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ArtifactDialog::reject);
    mainLayout->addWidget(buttonBox, 18, 0, 1, 2); // Span 2 columns

    setLayout(mainLayout);
    setMinimumWidth(400); // Ensure a reasonable minimum width
}

void ArtifactDialog::populateTypeComboBox()
{
    for (int i = static_cast<int>(ArtifactType::Pottery); i <= static_cast<int>(ArtifactType::Unknown); ++i) {
        typeCombo->addItem(Artifact::typeToString(static_cast<ArtifactType>(i)));
    }
}

void ArtifactDialog::populateConditionComboBox()
{
    for (int i = static_cast<int>(ArtifactCondition::Excellent); i <= static_cast<int>(ArtifactCondition::Fragments); ++i) {
        conditionCombo->addItem(Artifact::conditionToString(static_cast<ArtifactCondition>(i)));
    }
}

void ArtifactDialog::loadArtifactData(const Artifact& artifact)
{
    catalogNumberEdit->setText(artifact.getCatalogNumber());
    nameEdit->setText(artifact.getName());
    typeCombo->setCurrentText(Artifact::typeToString(artifact.getType()));
    materialEdit->setText(artifact.getMaterial());
    cultureEdit->setText(artifact.getCulture());
    periodEdit->setText(artifact.getPeriod());
    siteEdit->setText(artifact.getSite());
    coordinatesEdit->setText(artifact.getCoordinates());
    discoveryDateEdit->setDate(artifact.getDiscoveryDate());
    conditionCombo->setCurrentText(Artifact::conditionToString(artifact.getCondition()));
    descriptionEdit->setText(artifact.getDescription());
    lengthSpinBox->setValue(artifact.getLength());
    widthSpinBox->setValue(artifact.getWidth());
    heightSpinBox->setValue(artifact.getHeight());
    weightSpinBox->setValue(artifact.getWeight());
    storageLocationEdit->setText(artifact.getStorageLocation());
    onDisplayCheckBox->setChecked(artifact.isOnDisplay());
}

Artifact ArtifactDialog::getArtifact() const
{
    Artifact artifact;
    artifact.setCatalogNumber(catalogNumberEdit->text());
    artifact.setName(nameEdit->text());
    artifact.setType(Artifact::stringToType(typeCombo->currentText()));
    artifact.setMaterial(materialEdit->text());
    artifact.setCulture(cultureEdit->text());
    artifact.setPeriod(periodEdit->text());
    artifact.setSite(siteEdit->text());
    artifact.setCoordinates(coordinatesEdit->text());
    artifact.setDiscoveryDate(discoveryDateEdit->date());
    artifact.setCondition(Artifact::stringToCondition(conditionCombo->currentText()));
    artifact.setDescription(descriptionEdit->toPlainText());
    artifact.setLength(lengthSpinBox->value());
    artifact.setWidth(widthSpinBox->value());
    artifact.setHeight(heightSpinBox->value());
    artifact.setWeight(weightSpinBox->value());
    artifact.setStorageLocation(storageLocationEdit->text());
    artifact.setOnDisplay(onDisplayCheckBox->isChecked());
    return artifact;
}