/********************************************************************************
** Form generated from reading UI file 'loadfiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADFILEDIALOG_H
#define UI_LOADFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_LoadFileDialog
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_3;
    QSpinBox *spinColumn;
    QLabel *label_4;
    QSpinBox *spinLabelRow;
    QLabel *label_2;
    QSpinBox *spinDataRow;
    QLineEdit *lineDataFile;
    QLabel *label_5;
    QPushButton *btnDataFile;
    QComboBox *comboFieldSeparator;
    QLabel *label_6;
    QComboBox *comboPreset;
    QFormLayout *formLayout_2;
    QLabel *label_7;
    QLineEdit *lineCustomFieldSeparator;
    QComboBox *comboDecimalSeparator;
    QLabel *label_8;
    QComboBox *comboGroupSeparator;
    QLabel *label_9;
    QCheckBox *checkDynamicSession;
    QLabel *label_10;

    void setupUi(QDialog *LoadFileDialog)
    {
        if (LoadFileDialog->objectName().isEmpty())
            LoadFileDialog->setObjectName(QStringLiteral("LoadFileDialog"));
        LoadFileDialog->resize(395, 429);
        gridLayout = new QGridLayout(LoadFileDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(LoadFileDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(LoadFileDialog);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label);

        label_3 = new QLabel(LoadFileDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_3);

        spinColumn = new QSpinBox(LoadFileDialog);
        spinColumn->setObjectName(QStringLiteral("spinColumn"));
        spinColumn->setMinimum(1);
        spinColumn->setMaximum(9999);
        spinColumn->setValue(1);

        formLayout->setWidget(9, QFormLayout::FieldRole, spinColumn);

        label_4 = new QLabel(LoadFileDialog);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(10, QFormLayout::LabelRole, label_4);

        spinLabelRow = new QSpinBox(LoadFileDialog);
        spinLabelRow->setObjectName(QStringLiteral("spinLabelRow"));
        spinLabelRow->setMinimum(1);
        spinLabelRow->setMaximum(9999);
        spinLabelRow->setValue(1);

        formLayout->setWidget(10, QFormLayout::FieldRole, spinLabelRow);

        label_2 = new QLabel(LoadFileDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(11, QFormLayout::LabelRole, label_2);

        spinDataRow = new QSpinBox(LoadFileDialog);
        spinDataRow->setObjectName(QStringLiteral("spinDataRow"));
        spinDataRow->setMinimum(2);
        spinDataRow->setMaximum(9999);
        spinDataRow->setValue(2);

        formLayout->setWidget(11, QFormLayout::FieldRole, spinDataRow);

        lineDataFile = new QLineEdit(LoadFileDialog);
        lineDataFile->setObjectName(QStringLiteral("lineDataFile"));
        lineDataFile->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineDataFile);

        label_5 = new QLabel(LoadFileDialog);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        btnDataFile = new QPushButton(LoadFileDialog);
        btnDataFile->setObjectName(QStringLiteral("btnDataFile"));

        formLayout->setWidget(3, QFormLayout::FieldRole, btnDataFile);

        comboFieldSeparator = new QComboBox(LoadFileDialog);
        comboFieldSeparator->setObjectName(QStringLiteral("comboFieldSeparator"));

        formLayout->setWidget(5, QFormLayout::FieldRole, comboFieldSeparator);

        label_6 = new QLabel(LoadFileDialog);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_6);

        comboPreset = new QComboBox(LoadFileDialog);
        comboPreset->setObjectName(QStringLiteral("comboPreset"));

        formLayout->setWidget(4, QFormLayout::FieldRole, comboPreset);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_7 = new QLabel(LoadFileDialog);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        lineCustomFieldSeparator = new QLineEdit(LoadFileDialog);
        lineCustomFieldSeparator->setObjectName(QStringLiteral("lineCustomFieldSeparator"));
        lineCustomFieldSeparator->setEnabled(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineCustomFieldSeparator);


        formLayout->setLayout(6, QFormLayout::FieldRole, formLayout_2);

        comboDecimalSeparator = new QComboBox(LoadFileDialog);
        comboDecimalSeparator->setObjectName(QStringLiteral("comboDecimalSeparator"));

        formLayout->setWidget(7, QFormLayout::FieldRole, comboDecimalSeparator);

        label_8 = new QLabel(LoadFileDialog);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_8);

        comboGroupSeparator = new QComboBox(LoadFileDialog);
        comboGroupSeparator->setObjectName(QStringLiteral("comboGroupSeparator"));

        formLayout->setWidget(8, QFormLayout::FieldRole, comboGroupSeparator);

        label_9 = new QLabel(LoadFileDialog);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_9);

        checkDynamicSession = new QCheckBox(LoadFileDialog);
        checkDynamicSession->setObjectName(QStringLiteral("checkDynamicSession"));
        checkDynamicSession->setLayoutDirection(Qt::LeftToRight);

        formLayout->setWidget(12, QFormLayout::FieldRole, checkDynamicSession);

        label_10 = new QLabel(LoadFileDialog);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(12, QFormLayout::LabelRole, label_10);


        horizontalLayout->addLayout(formLayout);


        gridLayout->addLayout(horizontalLayout, 0, 2, 3, 1);

        QWidget::setTabOrder(lineDataFile, btnDataFile);
        QWidget::setTabOrder(btnDataFile, comboPreset);
        QWidget::setTabOrder(comboPreset, comboFieldSeparator);
        QWidget::setTabOrder(comboFieldSeparator, lineCustomFieldSeparator);
        QWidget::setTabOrder(lineCustomFieldSeparator, comboDecimalSeparator);
        QWidget::setTabOrder(comboDecimalSeparator, comboGroupSeparator);
        QWidget::setTabOrder(comboGroupSeparator, spinColumn);
        QWidget::setTabOrder(spinColumn, spinLabelRow);
        QWidget::setTabOrder(spinLabelRow, spinDataRow);
        QWidget::setTabOrder(spinDataRow, checkDynamicSession);
        QWidget::setTabOrder(checkDynamicSession, buttonBox);

        retranslateUi(LoadFileDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LoadFileDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LoadFileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(LoadFileDialog);
    } // setupUi

    void retranslateUi(QDialog *LoadFileDialog)
    {
        LoadFileDialog->setWindowTitle(QApplication::translate("LoadFileDialog", "Load Data File", 0));
        label->setText(QApplication::translate("LoadFileDialog", "Field Separator", 0));
        label_3->setText(QApplication::translate("LoadFileDialog", "Column", 0));
        label_4->setText(QApplication::translate("LoadFileDialog", "Label Row", 0));
        label_2->setText(QApplication::translate("LoadFileDialog", "Data Row", 0));
        label_5->setText(QApplication::translate("LoadFileDialog", "Data File", 0));
        btnDataFile->setText(QApplication::translate("LoadFileDialog", "Select file", 0));
        label_6->setText(QApplication::translate("LoadFileDialog", "Apply preset", 0));
        label_7->setText(QApplication::translate("LoadFileDialog", "Custom", 0));
        label_8->setText(QApplication::translate("LoadFileDialog", "Decimal separator", 0));
        label_9->setText(QApplication::translate("LoadFileDialog", "Thousands separator", 0));
        checkDynamicSession->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_10->setToolTip(QApplication::translate("LoadFileDialog", "Enables auto reload", 0));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("LoadFileDialog", "Dynamic session", 0));
    } // retranslateUi

};

namespace Ui {
    class LoadFileDialog: public Ui_LoadFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADFILEDIALOG_H
