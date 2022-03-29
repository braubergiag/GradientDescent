#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H

#include <QDialog>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include "model.h"
#include "functionhandler.h"
namespace Ui {
class DialogModel;
}

class DialogModel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModel(QWidget *parent = nullptr,
                         Model * model = nullptr);
    ~DialogModel();


    void UpdateCoordinatesListWidget();
    void LoadModelConfig();
    StoppingCriterion GetStoppingCriterion();
    void InitStoppingCriterion();

private slots:
    void on_buttonBox_accepted();


    void on_rb_gd_clicked();

    void on_rb_rs_clicked();

    void on_rb_gd_toggled(bool checked);

    void on_rb_rs_toggled(bool checked);

private:
    Ui::DialogModel *ui;
    Model * model_ = nullptr;
    bool isInit{false};

};

#endif // DIALOGMODEL_H
