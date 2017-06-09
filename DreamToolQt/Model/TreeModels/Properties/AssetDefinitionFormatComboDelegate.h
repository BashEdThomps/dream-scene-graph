/*
 * AssetDefinitionFormatComboDelegate.h
 *
 * Created: 30 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#ifndef ASSETDEFINITIONFORMATCOMBODELEGATE_H
#define ASSETDEFINITIONFORMATCOMBODELEGATE_H

#include <DreamCore.h>
#include <QItemDelegate>

using namespace Dream;

class AssetDefinitionFormatComboDelegate : public QItemDelegate
{
public:
    AssetDefinitionFormatComboDelegate(std::string type, QObject* parent = 0);
    ~AssetDefinitionFormatComboDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
private:
    Dream::AssetType mAssetType;
};

#endif // ASSETDEFINITIONFORMATCOMBODELEGATE_H