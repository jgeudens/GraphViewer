/********************************************************************************
** Form generated from reading UI file 'axisscaledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AXISSCALEDIALOG_H
#define UI_AXISSCALEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AxisScaleDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QSpinBox *spinMinimum;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinMaximum;
    QLabel *labelUnit;
    QComboBox *comboUnit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AxisScaleDialog)
    {
        if (AxisScaleDialog->objectName().isEmpty())
            AxisScaleDialog->setObjectName(QStringLiteral("AxisScaleDialog"));
        AxisScaleDialog->resize(268, 123);
        verticalLayout = new QVBoxLayout(AxisScaleDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        spinMinimum = new QSpinBox(AxisScaleDialog);
        spinMinimum->setObjectName(QStringLiteral("spinMinimum"));
        spinMinimum->setMinimum(-86400000);
        spinMinimum->setMaximum(86400000);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinMinimum);

        label = new QLabel(AxisScaleDialog);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(AxisScaleDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        spinMaximum = new QSpinBox(AxisScaleDialog);
        spinMaximum->setObjectName(QStringLiteral("spinMaximum"));
        spinMaximum->setMinimum(-86400000);
        spinMaximum->setMaximum(86400000);
        spinMaximum->setValue(10);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinMaximum);

        labelUnit = new QLabel(AxisScaleDialog);
        labelUnit->setObjectName(QStringLiteral("labelUnit"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelUnit);

        comboUnit = new QComboBox(AxisScaleDialog);
        comboUnit->setObjectName(QStringLiteral("comboUnit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, comboUnit);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(AxisScaleDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AxisScaleDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AxisScaleDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AxisScaleDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AxisScaleDialog);
    } // setupUi

    void retranslateUi(QDialog *AxisScaleDialog)
    {
        AxisScaleDialog->setWindowTitle(QApplication::translate("AxisScaleDialog", "Dialog", 0));
        label->setText(QApplication::translate("AxisScaleDialog", "Minimum", 0));
        label_2->setText(QApplication::translate("AxisScaleDialog", "Maximum", 0));
        labelUnit->setText(QApplication::translate("AxisScaleDialog", "Unit scale", 0));
    } // retranslateUi

};

namespace Ui {
    class AxisScaleDialog: public Ui_AxisScaleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AXISSCALEDIALOG_H
