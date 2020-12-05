#ifndef MPMAPPINGSETTINGSWIDGET_H
#define MPMAPPINGSETTINGSWIDGET_H

#include <QToolButton>
#include <QDialog>
#include <QLabel>
#include <QPointer>

#include "brushsetting.h"
#include "mpmappingwidget.h"

class Editor;
class QGridLayout;
class ComboBox;
class QHBoxLayout;
class MPInputButton;


class MPInputButton : public QToolButton
{
    Q_OBJECT

public:
    MPInputButton(MPInputButton* inputButton);
    MPInputButton(BrushInputType inputType, QWidget* parent = nullptr);

    void pressed();

signals:
    void didPress(BrushInputType inputType);

private:
    BrushInputType mInputType;
};


class MPMappingOptionsWidget : public QDialog
{
    Q_OBJECT
public:
    struct MPMappingOption
    {
        MPInputButton* mappingButton;
        MPInputButton* removeActionButton;
        QLabel* settingDescLabel;
        BrushInputType inputType;

        MPMappingOption(MPInputButton* newMappingButton, MPInputButton* newRemoveActionButton, QLabel* newSettingDescLabel) {
            mappingButton = newMappingButton;
            removeActionButton = newRemoveActionButton;
            settingDescLabel = newSettingDescLabel;
        }

        void deleteAll()
        {
            delete mappingButton;
            delete removeActionButton;
            delete settingDescLabel;
        }
    };

    MPMappingOptionsWidget(QString optionName, BrushSettingType settingType, QWidget* parent = nullptr);

    void setCore(Editor* editor) { mEditor = editor; }
    void showInputMapper(BrushInputType inputType);
    void initUI();

    void updateMappingForInput(QVector<QPointF> points, BrushInputType inputType);

    void updateMappingWidgetForInput(BrushInputType inputType);

signals:
    void notifyMappingWidgetNeedsUpdate(BrushInputType inputType);
    void mappingForInputUpdated(QVector<QPointF> points, BrushInputType inputType);
    void removedInputOption(BrushInputType inputType);

private:
    MPMappingOption createMappingOption(BrushInputType input);
    void removeAction(BrushInputType input);
    void addOptionField(int index, QString name, int value);
    void setupUI();

    QGridLayout* mGridLayout = nullptr;
    QHBoxLayout* mHBoxLayout = nullptr;
    ComboBox* mMappingOptionsComboBox = nullptr;

    QPointer<MPMappingWidget> mMappingWidget;

    BrushSettingType mSettingType;
    Editor* mEditor = nullptr;

    QWidget* mParent = nullptr;

    QList<MPMappingOption> mOptions;

    QList<bool> mRemovedInputs;
};

#endif // MPMAPPINGSETTINGSWIDGET_H
