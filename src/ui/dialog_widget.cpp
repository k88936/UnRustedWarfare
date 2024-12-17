//
// Created by root on 12/12/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dialog_widget.h" resolved

#include "dialog_widget.h"
#include "ui_dialog_widget.h"
#include "utils.h"


dialog_widget::dialog_widget(QWidget *parent) :
    QFrame(parent), ui(new Ui::dialog_widget) {
    ui->setupUi(this);
    cmder_profiles.emplace_back(":/ui/trump1.png");
    cmder_profiles.emplace_back(":/ui/trump2.png");
    cmder_profiles.emplace_back(":/ui/trump3.png");
    cmder_profiles.emplace_back(":/ui/trump4.png");
}

void dialog_widget::dialog(const std::string & msg)
{
    ui->info_label->setText(QString::fromStdString(msg));
    const QImage image(utils::random_element(cmder_profiles));
#ifdef DEBUG
    assert(!image.isNull());
#endif

    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->adjustSize();

}

dialog_widget::~dialog_widget() {
    delete ui;
}
