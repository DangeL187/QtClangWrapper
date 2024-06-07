#include "Panel/CustomComboBox.hpp"

#include "Application/Application.hpp"

CustomComboBox::CustomComboBox(QWidget* parent, Application* application): QComboBox(parent), _application(application) {
    setEditable(true);
    connect(this->lineEdit(), &QLineEdit::editingFinished, this, &CustomComboBox::handleEditingFinished);
    connect(this, &QComboBox::currentIndexChanged, this, &CustomComboBox::handleCurrentIndexChanged);
    setMaxCount(3);
}

void CustomComboBox::handleCurrentIndexChanged() {
    _old_index = currentIndex();
}

void CustomComboBox::handleEditingFinished() {
    QString text = currentText();

    if (_old_index == 0) {
        _application->setCompilerOptions(text);
    } else if (_old_index == 1) {
        _application->setLinkerOptions(text);
    }

    setItemText(_old_index, text);
    handleCurrentIndexChanged();
}