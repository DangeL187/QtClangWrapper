#ifndef QTCLANGWRAPPER_CUSTOMCOMBOBOX_HPP
#define QTCLANGWRAPPER_CUSTOMCOMBOBOX_HPP

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QVBoxLayout>

class Application;

class CustomComboBox: public QComboBox {
Q_OBJECT
public:
    explicit CustomComboBox(QWidget* parent, Application* application);

private slots:
    void handleCurrentIndexChanged();
    void handleEditingFinished();

private:
    Application*    _application;
    int             _old_index = 0;
};

#endif //QTCLANGWRAPPER_CUSTOMCOMBOBOX_HPP
