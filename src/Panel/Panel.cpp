#include "Panel/Panel.hpp"

#include "Application/Application.hpp"

Panel::Panel(Application* parent): _parent(parent) {
    initOptions();
    initRunButton();
    initOpenFileButton();
    initSaveButton();

    initLayout();
    setStyleSheet("color: white; background-color: rgb(68, 68, 68);");
    setMaximumHeight(QFrame::sizeHint().height());
}

void Panel::initLayout() {
    _layout = std::make_shared<QHBoxLayout>(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addWidget(_open_file_button.get());
    _layout->addWidget(_save_button.get());
    _layout->addWidget(_view_selector.get());
    _layout->addWidget(_run_button.get());
}

#include <iostream>

void Panel::initOpenFileButton() {
    _open_file_button = std::make_shared<QPushButton>(this);
    _open_file_button->setText("Open source file");
    _open_file_button->setStyleSheet("background-color: grey");
    connect(_open_file_button.get(), &QPushButton::clicked, this, [this](){
        QString file_path = QFileDialog::getOpenFileName(this,tr("Open source file"),".");
        _parent->setSourceFile(file_path);
    });
}

void Panel::initOptions() {
    _view_selector = std::make_shared<QComboBox>(this);

    auto* model = new QStandardItemModel(this);

    auto* title_item = new QStandardItem("View Selector");
    title_item->setFlags(Qt::NoItemFlags);
    model->appendRow(title_item);

    QStringList items = {"Editor", "Preprocessor", "IR Code", "Assembler", "Disassembly", "Headers", "Metrics", "Executor"};
    for (const QString& item: items) {
        auto* item_model = new QStandardItem(item);
        item_model->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        if (item == "Executor" || item == "Editor") {
            item_model->setData(Qt::Checked, Qt::CheckStateRole);
        } else {
            item_model->setData(Qt::Unchecked, Qt::CheckStateRole);
        }
        model->appendRow(item_model);
    }

    _view_selector->setModel(model);
    _view_selector->setCurrentIndex(0);

    connect(model, &QStandardItemModel::itemChanged, this, [this](QStandardItem *item) {
        bool is_checked = item->checkState() == Qt::Checked;

        if (is_checked) _parent->increaseWorkspacesCount();
        else _parent->decreaseWorkspacesCount();

        if (item->text() == "Editor") {
            _parent->setEditVisible(is_checked);
        } else if (item->text() == "Preprocessor") {
            _parent->setPrepVisible(is_checked);
        } else if (item->text() == "IR Code") {
            _parent->setIrVisible(is_checked);
        } else if (item->text() == "Assembler") {
            _parent->setAsmVisible(is_checked);
        } else if (item->text() == "Disassembly") {
            _parent->setDisasmVisible(is_checked);
        } else if (item->text() == "Headers") {
            _parent->setHeadersVisible(is_checked);
        } else if (item->text() == "Metrics") {
            _parent->setMetricsVisible(is_checked);
        } else if (item->text() == "Executor") {
            _parent->setExecVisible(is_checked);
        }

        _parent->updateSize();
    });
}

void Panel::initRunButton() {
    _run_button = std::make_shared<QPushButton>(this);
    _run_button->setText("Run");
    _run_button->setStyleSheet("background-color: green");
    connect(_run_button.get(), &QPushButton::clicked, this, [this](){
        _parent->compile();
    });
}

void Panel::initSaveButton() {
    _save_button = std::make_shared<QPushButton>(this);
    _save_button->setText("Save files");
    _save_button->setStyleSheet("background-color: gray");
    connect(_save_button.get(), &QPushButton::clicked, this, [this](){
        QString folder_path = QFileDialog::getExistingDirectory(this, tr("Save in"),".",
                                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        _parent->saveOutputFiles(folder_path);
    });
}