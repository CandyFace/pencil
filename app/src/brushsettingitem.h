#ifndef BRUSHSETTINGITEM_H
#define BRUSHSETTINGITEM_H

#include <QTreeWidgetItem>

class BrushSettingItem : public QTreeWidgetItem
{

public:
    enum Category {
        Basic,
        Advanced,
        Active,
        Opacity,
        Dab,
        Speed,
        Random,
        Offset,
        Tracking,
        Color,
        Smudge,
        Eraser,
        Stroke,
        Custom_Input,
        Elliptical_Dab,
        Other,
        Unknown
    };

    BrushSettingItem(Category category, QTreeWidget* parent = nullptr);
    BrushSettingItem(Category category, QTreeWidgetItem* parent = nullptr);

    Category ItemCategory() { return mCategory; }

private:

    Category mCategory;
};

#endif // BRUSHSETTINGITEM_H
