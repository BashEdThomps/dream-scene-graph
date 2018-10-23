#include "ModelMaterialTableDelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <DreamCore.h>

ModelMaterialTableDelegate::ModelMaterialTableDelegate
(QObject* parent)
    : QItemDelegate (parent)
{
    auto log = spdlog::get("MaterialShaderTableDelegate");
    if (log==nullptr)
    {
       log=spdlog::stdout_color_mt("MaterialShaderTableDelegate");
    }
    log->debug("Constructing");
}

ModelMaterialTableDelegate::~ModelMaterialTableDelegate()
{
    auto log = spdlog::get("MaterialShaderTableDelegate");
    log->debug("Destructing");
}

QWidget*
ModelMaterialTableDelegate::createEditor
(QWidget* parent, const QStyleOptionViewItem& option, const  QModelIndex& index)
const
{
    Q_UNUSED(option)

    auto log = spdlog::get("MaterialShaderTableDelegate");

    QWidget* widget = nullptr;
    QString name;
    QComboBox* combo = nullptr;

    switch (index.column())
    {
        case 0:
           name = index.data().toString();
           log->debug("Creating QLineEdit Delegate for {}",name.toStdString());
           widget = new QLineEdit(name, parent);
           break;
        case 1:
            log->debug("Creating QComboBox Delegate for {}",name.toStdString());
            widget = new QComboBox(parent);
            combo = dynamic_cast<QComboBox*>(widget);
            for (auto shaderDef : mShaderDefinitions)
            {
                combo->addItem(
                    QString::fromStdString(shaderDef->getName()),
                    QString::fromStdString(shaderDef->getUuid())
                );
            }
            break;
    }
    return widget;
}

void
ModelMaterialTableDelegate::setMaterialDefinitions
(vector<MaterialDefinition*>& shaderDefinitions)
{
    mShaderDefinitions = shaderDefinitions;
}

void
ModelMaterialTableDelegate::setMaterialList
(vector<string> materialList)
{
   mMaterialList = materialList;
}


void
ModelMaterialTableDelegate::setModelData
(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* lineEdit = nullptr;
    QComboBox* comboBox = nullptr;

    switch (index.column())
    {
        case 0:
            lineEdit = dynamic_cast<QLineEdit*>(editor);
            model->setData(index,lineEdit->text());
            break;
        case 1:
            comboBox = dynamic_cast<QComboBox*>(editor);
            model->setData(index,comboBox->currentData());
            break;
    }
}


void
ModelMaterialTableDelegate::setEditorData
(QWidget* editor, const QModelIndex& index)
const
{
    QLineEdit* lineEdit = nullptr;
    QComboBox* comboBox = nullptr;

    switch (index.column())
    {
        case 0:
            lineEdit = dynamic_cast<QLineEdit*>(editor);
            lineEdit->setText(index.data().toString());
            break;
        case 1:
            comboBox = dynamic_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findData(index.data()));
            break;
    }

}
