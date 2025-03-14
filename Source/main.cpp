#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QStyleFactory>

class FlightReservation : public QMainWindow {
    Q_OBJECT

public:
    FlightReservation(QWidget *parent = nullptr) : QMainWindow(parent) {
        setStyle(QStyleFactory::create("Fusion"));  // Apply modern theme
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        QLabel *titleLabel = new QLabel("<h1 style='color:#fff;'>✈ Flight Reservation System ✈</h1>", this);
        titleLabel->setAlignment(Qt::AlignCenter);

        // Form Layout
        QFormLayout *formLayout = new QFormLayout();
        nameEdit = new QLineEdit(this);
        nameEdit->setPlaceholderText("Enter Passenger Name");

        emailEdit = new QLineEdit(this);
        emailEdit->setPlaceholderText("Enter Email for Confirmation");

        flightNoEdit = new QLineEdit(this);
        flightNoEdit->setPlaceholderText("Enter Flight Number");

        sourceEdit = new QLineEdit(this);
        sourceEdit->setPlaceholderText("Enter Source");

        destinationEdit = new QLineEdit(this);
        destinationEdit->setPlaceholderText("Enter Destination");

        seatType = new QComboBox(this);
        seatType->addItems({"Economy", "Business", "First Class"});

        formLayout->addRow("Passenger Name:", nameEdit);
        formLayout->addRow("Email:", emailEdit);
        formLayout->addRow("Flight Number:", flightNoEdit);
        formLayout->addRow("Source:", sourceEdit);
        formLayout->addRow("Destination:", destinationEdit);
        formLayout->addRow("Seat Type:", seatType);

        // Buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *bookFlightButton = new QPushButton("✈ Book Flight", this);
        QPushButton *clearButton = new QPushButton("🗑 Clear All", this);
        QPushButton *searchButton = new QPushButton("🔍 Search", this);
        QPushButton *deleteButton = new QPushButton("❌ Delete Booking", this);
        QPushButton *redeemButton = new QPushButton("🎟 Redeem Points", this);

        buttonLayout->addWidget(bookFlightButton);
        buttonLayout->addWidget(searchButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addWidget(redeemButton);
        buttonLayout->addWidget(clearButton);

        // Table View
        tableModel = new QStandardItemModel(0, 5, this);
        tableModel->setHorizontalHeaderLabels({"Passenger Name", "Flight No", "Source", "Destination", "Seat Type"});

        tableView = new QTableView(this);
        tableView->setModel(tableModel);
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tableView->setSortingEnabled(true);

        mainLayout->addWidget(titleLabel);
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(tableView);

        centralWidget->setStyleSheet(R"(
            QWidget { background-color: #2E2E2E; color: white; }
            QLineEdit, QComboBox, QPushButton { font-size: 16px; padding: 10px; border-radius: 8px; }
            QLineEdit, QComboBox { background-color: #555; color: white; }
            QPushButton { background-color: #0078D7; color: white; font-weight: bold; }
            QPushButton:hover { background-color: #0053A6; }
            QTableView { background-color: #3E3E3E; color: white; font-size: 14px; }
        )");

        setCentralWidget(centralWidget);
        setWindowTitle("Flight Reservation System");
        showMaximized();

        connect(bookFlightButton, &QPushButton::clicked, this, &FlightReservation::bookFlight);
        connect(clearButton, &QPushButton::clicked, this, &FlightReservation::clearAll);
        connect(searchButton, &QPushButton::clicked, this, &FlightReservation::searchFlight);
        connect(deleteButton, &QPushButton::clicked, this, &FlightReservation::deleteBooking);
        connect(redeemButton, &QPushButton::clicked, this, &FlightReservation::redeemPoints);
    }

private slots:
    void bookFlight() {
        QString name = nameEdit->text();
        QString email = emailEdit->text();
        QString flightNo = flightNoEdit->text();
        QString source = sourceEdit->text();
        QString destination = destinationEdit->text();
        QString seat = seatType->currentText();

        if (name.isEmpty() || email.isEmpty() || flightNo.isEmpty() || source.isEmpty() || destination.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "All fields must be filled!");
            return;
        }

        QList<QStandardItem *> row;
        row.append(new QStandardItem(name));
        row.append(new QStandardItem(flightNo));
        row.append(new QStandardItem(source));
        row.append(new QStandardItem(destination));
        row.append(new QStandardItem(seat));

        tableModel->appendRow(row);

        // Reward system
        rewardPoints += 10;  // Each booking earns 10 points

        QMessageBox::information(this, "Success", "Flight booked successfully!\nEmail Confirmation Sent.\nYou earned 10 points!");

        nameEdit->clear();
        emailEdit->clear();
        flightNoEdit->clear();
        sourceEdit->clear();
        destinationEdit->clear();
        seatType->setCurrentIndex(0);
    }

    void searchFlight() {
        QString searchText = nameEdit->text().trimmed();
        if (searchText.isEmpty()) {
            QMessageBox::warning(this, "Search Error", "Please enter a passenger name or flight number!");
            return;
        }

        bool found = false;
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            if (tableModel->item(row, 0)->text().contains(searchText, Qt::CaseInsensitive) ||
                tableModel->item(row, 1)->text().contains(searchText, Qt::CaseInsensitive)) {

                tableView->selectRow(row);
                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::information(this, "Not Found", "No matching flight found!");
        }
    }

    void deleteBooking() {
        QModelIndex index = tableView->currentIndex();
        if (index.isValid()) {
            tableModel->removeRow(index.row());
            QMessageBox::information(this, "Deleted", "Booking removed successfully!");
        } else {
            QMessageBox::warning(this, "Delete Error", "Please select a booking to delete!");
        }
    }

    void redeemPoints() {
        if (rewardPoints >= 50) {
            rewardPoints -= 50;
            QMessageBox::information(this, "Redeemed", "You redeemed 50 points for a discount!");
        } else {
            QMessageBox::warning(this, "Not Enough Points", "You need at least 50 points to redeem a discount!");
        }
    }

    void clearAll() {
        if (QMessageBox::question(this, "Confirm", "Are you sure you want to clear all bookings?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            tableModel->removeRows(0, tableModel->rowCount());
            QMessageBox::information(this, "Cleared", "All reservations have been cleared!");
        }
    }

private:
    QLineEdit *nameEdit, *emailEdit, *flightNoEdit, *sourceEdit, *destinationEdit;
    QComboBox *seatType;
    QStandardItemModel *tableModel;
    QTableView *tableView;
    int rewardPoints = 0;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FlightReservation window;
    window.show();
    return app.exec();
}



