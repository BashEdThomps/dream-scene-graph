#pragma onceo

#include <QItemDelegate>
#include <vector>
#include <string>
#include <memory>

namespace Dream
{
    class ShaderDefinition;
}

using Dream::ShaderDefinition;
using std::shared_ptr;
using std::vector;
using std::string;

class MaterialShaderTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MaterialShaderTableDelegate(QObject* parent  = nullptr);
    ~MaterialShaderTableDelegate() override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const  override;
    void setShaderDefinitions(vector<ShaderDefinition*>& shaderDefinitions);
    void setMaterialList(vector<string> materialList);
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

protected:
    vector<ShaderDefinition*> mShaderDefinitions;
    vector<string> mMaterialList;
};

