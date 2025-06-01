//
// Created by matei on 31/05/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include <QGroupBox>
#include "../controller/ArtifactController.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void addArtifact();
    void editArtifact();
    void deleteArtifact();
    void refreshTable();
    void applyFilters();
    void clearFilters();
    void undo();
    void redo();
    void switchToCSV();
    void switchToJSON();
    void onTableSelectionChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolbar();
    void setupFilterPanel();
    void setupTable();
    void connectSignals();
    void populateTable(const std::vector<std::unique_ptr<Artifact>>& artifacts);
    void updateButtonStates();

    // UI Components
    QWidget* centralWidget_;
    QVBoxLayout* mainLayout_;
    QHBoxLayout* toolbarLayout_;
    QGroupBox* filterGroup_;
    QTableWidget* artifactTable_;

    // Toolbar buttons
    QPushButton* addButton_;
    QPushButton* editButton_;
    QPushButton* deleteButton_;
    QPushButton* undoButton_;
    QPushButton* redoButton_;
    QPushButton* refreshButton_;

    // Filter components
    QLineEdit* nameFilter_;
    QComboBox* typeFilter_;
    QLineEdit* materialFilter_;
    QLineEdit* periodFilter_;
    QLineEdit* siteFilter_;
    QComboBox* conditionFilter_;
    QComboBox* displayFilter_;
    QPushButton* applyFilterButton_;
    QPushButton* clearFilterButton_;

    // Controller
    std::unique_ptr<ArtifactController> controller_;
};




#endif //MAINWINDOW_H
