//
// Created by matei on 31/05/2025.
//

#include "../../include/ui/MainWindow.h"
#include "../../include/ui/ArtifactDialog.h"


#include "../../include/repository/CSVRepository.h"
#include "../../include/repository/JSONRepository.h"
#include "../../include/filters/TypeFilter.h"
#include "../../include/filters/MaterialFilter.h"
#include "../../include/filters/PeriodFilter.h"
#include "../../include/filters/LocationFilter.h"
#include "../../include/filters/AndFilter.h"
#include "../../include/filters/OrFilter.h"
#include "../../include/filters/NameFilter.h"
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QGridLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    controller_ = std::make_unique<ArtifactController>(
        std::make_unique<JSONRepository>("artifacts.json"));

    setupUI();
    connectSignals();
    refreshTable();
}

void MainWindow::setupUI() {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);

    mainLayout_ = new QVBoxLayout(centralWidget_);

    setupMenuBar();
    setupToolbar();
    setupFilterPanel();
    setupTable();

    setWindowTitle("Archaeological Artifacts Inventory");
    setMinimumSize(1200, 800);
    resize(1400, 900);
}

void MainWindow::setupMenuBar() {
    auto fileMenu = menuBar()->addMenu("&File");

    auto csvAction = fileMenu->addAction("Switch to &CSV");
    connect(csvAction, &QAction::triggered, this, &MainWindow::switchToCSV);

    auto jsonAction = fileMenu->addAction("Switch to &JSON");
    connect(jsonAction, &QAction::triggered, this, &MainWindow::switchToJSON);

    fileMenu->addSeparator();

    auto exitAction = fileMenu->addAction("E&xit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    auto editMenu = menuBar()->addMenu("&Edit");

    auto undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    auto redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);
}

void MainWindow::setupToolbar() {
    toolbarLayout_ = new QHBoxLayout();

    addButton_ = new QPushButton("Add Artifact");
    editButton_ = new QPushButton("Edit Artifact");
    deleteButton_ = new QPushButton("Delete Artifact");
    undoButton_ = new QPushButton("Undo");
    redoButton_ = new QPushButton("Redo");
    refreshButton_ = new QPushButton("Refresh");

    toolbarLayout_->addWidget(addButton_);
    toolbarLayout_->addWidget(editButton_);
    toolbarLayout_->addWidget(deleteButton_);
    toolbarLayout_->addWidget(undoButton_);
    toolbarLayout_->addWidget(redoButton_);
    toolbarLayout_->addWidget(refreshButton_);
    toolbarLayout_->addStretch();

    mainLayout_->addLayout(toolbarLayout_);
}

void MainWindow::setupFilterPanel() {
    filterGroup_ = new QGroupBox("Filters");
    auto filterLayout = new QGridLayout(filterGroup_);

    // Filter controls
    filterLayout->addWidget(new QLabel("Name:"), 0, 0);
    nameFilter_ = new QLineEdit();
    nameFilter_->setPlaceholderText("Search by name...");
    filterLayout->addWidget(nameFilter_, 0, 1);

    filterLayout->addWidget(new QLabel("Type:"), 0, 2);
    typeFilter_ = new QComboBox();
    typeFilter_->addItem("All Types", -1);
    typeFilter_->addItem("Pottery", static_cast<int>(ArtifactType::Pottery));
    typeFilter_->addItem("Tool", static_cast<int>(ArtifactType::Tool));
    typeFilter_->addItem("Weapon", static_cast<int>(ArtifactType::Weapon));
    typeFilter_->addItem("Jewelry", static_cast<int>(ArtifactType::Jewelry));
    typeFilter_->addItem("Sculpture", static_cast<int>(ArtifactType::Sculpture));
    typeFilter_->addItem("Coin", static_cast<int>(ArtifactType::Coin));
    typeFilter_->addItem("Tablet", static_cast<int>(ArtifactType::Tablet));
    typeFilter_->addItem("Bone", static_cast<int>(ArtifactType::Bone));
    typeFilter_->addItem("Textile", static_cast<int>(ArtifactType::Textile));
    filterLayout->addWidget(typeFilter_, 0, 3);

    filterLayout->addWidget(new QLabel("Material:"), 1, 0);
    materialFilter_ = new QLineEdit();
    materialFilter_->setPlaceholderText("Search by material...");
    filterLayout->addWidget(materialFilter_, 1, 1);

    filterLayout->addWidget(new QLabel("Period:"), 1, 2);
    periodFilter_ = new QLineEdit();
    periodFilter_->setPlaceholderText("Search by period...");
    filterLayout->addWidget(periodFilter_, 1, 3);

    filterLayout->addWidget(new QLabel("Site:"), 2, 0);
    siteFilter_ = new QLineEdit();
    siteFilter_->setPlaceholderText("Search by site...");
    filterLayout->addWidget(siteFilter_, 2, 1);

    filterLayout->addWidget(new QLabel("Condition:"), 2, 2);
    conditionFilter_ = new QComboBox();
    conditionFilter_->addItem("All Conditions", -1);
    conditionFilter_->addItem("Excellent", static_cast<int>(ArtifactCondition::Excellent));
    conditionFilter_->addItem("Good", static_cast<int>(ArtifactCondition::Good));
    conditionFilter_->addItem("Fair", static_cast<int>(ArtifactCondition::Fair));
    conditionFilter_->addItem("Poor", static_cast<int>(ArtifactCondition::Poor));
    conditionFilter_->addItem("Fragments", static_cast<int>(ArtifactCondition::Fragments));
    filterLayout->addWidget(conditionFilter_, 2, 3);

    // Filter buttons
    auto buttonLayout = new QHBoxLayout();
    applyFilterButton_ = new QPushButton("Apply Filters");
    clearFilterButton_ = new QPushButton("Clear Filters");
    buttonLayout->addWidget(applyFilterButton_);
    buttonLayout->addWidget(clearFilterButton_);
    buttonLayout->addStretch();

    filterLayout->addLayout(buttonLayout, 3, 0, 1, 4);

    mainLayout_->addWidget(filterGroup_);
}

void MainWindow::setupTable() {
    artifactTable_ = new QTableWidget();
    artifactTable_->setColumnCount(11);

    QStringList headers;
    headers << "Catalog #" << "Name" << "Type" << "Material" << "Culture"
            << "Period" << "Site" << "Discovery Date" << "Condition"
            << "Storage Location" << "On Display";
    artifactTable_->setHorizontalHeaderLabels(headers);

    artifactTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    artifactTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    artifactTable_->setAlternatingRowColors(true);
    artifactTable_->setSortingEnabled(true);

    // Resize columns to content
    artifactTable_->horizontalHeader()->setStretchLastSection(true);
    artifactTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mainLayout_->addWidget(artifactTable_);

    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals() {
    connect(addButton_, &QPushButton::clicked, this, &MainWindow::addArtifact);
    connect(editButton_, &QPushButton::clicked, this, &MainWindow::editArtifact);
    connect(deleteButton_, &QPushButton::clicked, this, &MainWindow::deleteArtifact);
    connect(undoButton_, &QPushButton::clicked, this, &MainWindow::undo);
    connect(redoButton_, &QPushButton::clicked, this, &MainWindow::redo);
    connect(refreshButton_, &QPushButton::clicked, this, &MainWindow::refreshTable);

    connect(applyFilterButton_, &QPushButton::clicked, this, &MainWindow::applyFilters);
    connect(clearFilterButton_, &QPushButton::clicked, this, &MainWindow::clearFilters);

    connect(artifactTable_, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);

    connect(artifactTable_, &QTableWidget::itemDoubleClicked,
            this, &MainWindow::editArtifact);

    // Update button states initially
    updateButtonStates();
}

void MainWindow::addArtifact() {
    ArtifactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Artifact artifact = dialog.getArtifact();
        if (artifact != Artifact()) {
            controller_->addArtifact(std::make_unique<Artifact>(artifact));
            refreshTable();
            statusBar()->showMessage("Artifact added successfully", 2000);
        }
    }
}

void MainWindow::editArtifact() {
    int currentRow = artifactTable_->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select an artifact to edit.");
        return;
    }

    QString catalogNumber = artifactTable_->item(currentRow, 0)->text();
    auto artifact = controller_->findArtifact(catalogNumber);

    if (artifact) {
        // Pass by reference, not pointer
        ArtifactDialog dialog(*artifact, this);
        if (dialog.exec() == QDialog::Accepted) {
            // getArtifact() returns an Artifact by value
            auto updatedArtifact = dialog.getArtifact();
            // Wrap into a unique_ptr when calling updateArtifact
            controller_->updateArtifact(std::make_unique<Artifact>(updatedArtifact));
            refreshTable();
            statusBar()->showMessage("Artifact updated successfully", 2000);
        }
    }
}

void MainWindow::deleteArtifact() {
    int currentRow = artifactTable_->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select an artifact to delete.");
        return;
    }

    QString catalogNumber = artifactTable_->item(currentRow, 0)->text();
    QString name = artifactTable_->item(currentRow, 1)->text();

    int ret = QMessageBox::question(this, "Confirm Deletion",
        QString("Are you sure you want to delete artifact '%1' (%2)?")
        .arg(name, catalogNumber),
        QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        controller_->removeArtifact(catalogNumber);
        refreshTable();
        statusBar()->showMessage("Artifact deleted successfully", 2000);
    }
}

void MainWindow::refreshTable() {
    auto artifacts = controller_->getAllArtifacts();
    populateTable(artifacts);
    updateButtonStates();
}

void MainWindow::applyFilters() {
    std::vector<std::unique_ptr<ArtifactFilter>> filters;

    // Name filter (searches in artifact name)
    if (!nameFilter_->text().isEmpty()) {
        filters.push_back(std::make_unique<NameFilter>(nameFilter_->text()));
    }

    // Type filter
    if (typeFilter_->currentData().toInt() != -1) {
        auto typeFilterObj = std::make_unique<TypeFilter>(
            static_cast<ArtifactType>(typeFilter_->currentData().toInt()));
        filters.push_back(std::move(typeFilterObj));
    }

    // Material filter
    if (!materialFilter_->text().isEmpty()) {
        auto materialFilterObj = std::make_unique<MaterialFilter>(materialFilter_->text());
        filters.push_back(std::move(materialFilterObj));
    }

    // Period filter
    if (!periodFilter_->text().isEmpty()) {
        auto periodFilterObj = std::make_unique<PeriodFilter>(periodFilter_->text());
        filters.push_back(std::move(periodFilterObj));
    }

    // Site filter
    if (!siteFilter_->text().isEmpty()) {
        auto siteFilterObj = std::make_unique<LocationFilter>(siteFilter_->text());
        filters.push_back(std::move(siteFilterObj));
    }

    std::vector<std::unique_ptr<Artifact>> results;

    if (filters.empty()) {
        results = controller_->getAllArtifacts();
    } else {
        // Combine filters with AND logic
        std::unique_ptr<ArtifactFilter> combinedFilter = std::move(filters[0]);
        for (size_t i = 1; i < filters.size(); ++i) {
            combinedFilter = std::make_unique<AndFilter>(
                std::move(combinedFilter), std::move(filters[i]));
        }

        results = controller_->filterArtifacts(*combinedFilter);
    }


    populateTable(results);
    statusBar()->showMessage(QString("Filter applied - %1 artifacts found").arg(results.size()), 2000);
}

void MainWindow::clearFilters() {
    nameFilter_->clear();
    typeFilter_->setCurrentIndex(0);
    materialFilter_->clear();
    periodFilter_->clear();
    siteFilter_->clear();
    conditionFilter_->setCurrentIndex(0);

    refreshTable();
    statusBar()->showMessage("Filters cleared", 2000);
}

void MainWindow::undo() {
    controller_->undo();
    refreshTable();
    updateButtonStates();
    statusBar()->showMessage("Undo completed", 2000);
}

void MainWindow::redo() {
    controller_->redo();
    refreshTable();
    updateButtonStates();
    statusBar()->showMessage("Redo completed", 2000);
}

void MainWindow::switchToCSV() {
    controller_->switchRepository(std::make_unique<CSVRepository>("artifacts.csv"));
    refreshTable();
    statusBar()->showMessage("Switched to CSV repository", 2000);
}

void MainWindow::switchToJSON() {
    controller_->switchRepository(std::make_unique<JSONRepository>("artifacts.json"));
    refreshTable();
    statusBar()->showMessage("Switched to JSON repository", 2000);
}

void MainWindow::onTableSelectionChanged() {
    updateButtonStates();
}
void MainWindow::populateTable(const std::vector<std::unique_ptr<Artifact>>& artifacts) {
    artifactTable_->clearContents();
    artifactTable_->setRowCount(static_cast<int>(artifacts.size()));

    for (int row = 0; row < static_cast<int>(artifacts.size()); ++row) {
        const auto& art = artifacts[row];
        artifactTable_->setItem(row, 0, new QTableWidgetItem(art->getCatalogNumber()));
        artifactTable_->setItem(row, 1, new QTableWidgetItem(art->getName()));
        artifactTable_->setItem(row, 2, new QTableWidgetItem(Artifact::typeToString(art->getType())));
        artifactTable_->setItem(row, 3, new QTableWidgetItem(art->getMaterial()));
        artifactTable_->setItem(row, 4, new QTableWidgetItem(art->getCulture()));
        artifactTable_->setItem(row, 5, new QTableWidgetItem(art->getPeriod()));
        artifactTable_->setItem(row, 6, new QTableWidgetItem(art->getSite()));
        artifactTable_->setItem(row, 7, new QTableWidgetItem(art->getDiscoveryDate().toString(Qt::ISODate)));
        artifactTable_->setItem(row, 8, new QTableWidgetItem(Artifact::conditionToString(art->getCondition())));
        artifactTable_->setItem(row, 9, new QTableWidgetItem(art->getStorageLocation()));
        artifactTable_->setItem(row, 10, new QTableWidgetItem(art->isOnDisplay() ? "Yes" : "No"));
    }
}

void MainWindow::updateButtonStates() {
    bool hasSelection = artifactTable_->currentRow() >= 0;
    editButton_->setEnabled(hasSelection);
    deleteButton_->setEnabled(hasSelection);
    undoButton_->setEnabled(controller_->canUndo());
    redoButton_->setEnabled(controller_->canRedo());
}