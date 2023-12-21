#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mmobject.h"
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

static QString homeLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
static QString defaultExportLocation = homeLocation + "/Test";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the file list
    model.setHorizontalHeaderLabels({ "Path", "Video codec", "Avg. frame rate", "Audio codec", "Sample rate" });
    ui->tableView->setModel(&model);

    // Set default export directory
    ui->lineEdit->setText(defaultExportLocation);
    QDir defaultExportDir(defaultExportLocation);
    if (!defaultExportDir.exists()) {
        if (!defaultExportDir.mkpath(".")) {
            QMessageBox::warning(this, "Warning", "Failed to create default export directory. Select a new one.");

            QString directory;
            do {
                directory = QFileDialog::getExistingDirectory(this, "Select output folder", defaultExportLocation, QFileDialog::ShowDirsOnly);
            } while (directory.isEmpty());
            ui->lineEdit->setText(directory);
        }
    }

    // Resize combo-boxes dynamically
    ui->comboBox->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    ui->comboBox_2->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    ui->comboBox_3->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    ui->comboBox_4->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
    ui->comboBox_5->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select input files",
        homeLocation, "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.mpeg *.mpg)", nullptr,
        QFileDialog::ReadOnly);
    if (!filePaths.isEmpty()) {
        for (QString filePath : filePaths) {
            try {
                MMObject object(filePath.toStdString());

                QStringList rowData = {
                    filePath,
                    object.getVideoCodec(),
                    QString::number(object.getFrameRate()),
                    object.getAudioCodec(),
                    QString::number(object.getSampleRate()),
                };
                QList<QStandardItem*> rowItems;
                for (QString cellData : rowData) {
                    QStandardItem* cellItem = new QStandardItem(cellData);
                    rowItems.append(cellItem);
                }
                model.appendRow(rowItems);
            } catch (MMObject::Exception& ex) {
                QMessageBox::warning(this, "Warning", filePath + ": " + ex.what());
            } catch (...) {
                QMessageBox::warning(this, "Warning", filePath + ": " + "Unknown error occurred while reading");
            }
        }

        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();

    for (const QModelIndex& index : selectedIndexes) {
        ui->tableView->model()->removeRow(index.row());
    }

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select output folder", defaultExportLocation, QFileDialog::ShowDirsOnly);
    if (!directory.isEmpty()) {
        ui->lineEdit->setText(directory);
    }
}

void MainWindow::on_comboBox_textActivated(const QString& arg1)
{
    // Repopulate combo boxes
    if (arg1 == "MP4") {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({ "H.264/AVC", "H.265/HEVC" });
        ui->comboBox_4->clear();
        ui->comboBox_4->addItems({ "AAC", "MP3" });
    } else if (arg1 == "MOV") {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({ "H.264/AVC", "H.265/HEVC" });
        ui->comboBox_4->clear();
        ui->comboBox_4->addItems({ "AAC", "MP3" });
    } else if (arg1 == "MKV") {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({ "H.264/AVC", "H.265/HEVC", "VP9", "AV1" });
        ui->comboBox_4->clear();
        ui->comboBox_4->addItems({ "AAC", "MP3", "FLAC", "AC3" });
    } else {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({ "VP9", "AV1" });
        ui->comboBox_4->clear();
        ui->comboBox_4->addItems({ "Opus", "Vorbis" });
    }
}
