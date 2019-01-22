/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2012-2018 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef BACKUPELEMENT_H
#define BACKUPELEMENT_H

#include <QObject>
#include <QUndoCommand>
#include <QRectF>
#include <QTransform>

#include "direction.h"
#include "movemode.h"
#include "pencildef.h"
#include "layer.h"

class Editor;
class BackupManager;
class PreferenceManager;
class BitmapImage;
class VectorImage;
class SoundClip;
class Camera;
class Layer;
class KeyFrame;

enum types { UNDEFINED,
             ADD_KEY_MODIF,
             REMOVE_KEY_MODIF
           };

class BackupElement : public QUndoCommand
{
public:
    explicit BackupElement(Editor* editor, QUndoCommand* parent = nullptr);
    virtual ~BackupElement();

    Editor* editor() { return mEditor; }

    virtual int type() { return UNDEFINED; }
    virtual void undo() { Q_ASSUME(true); } // should never end here
    virtual void redo() { Q_ASSUME(true); } // should never end here
private:
    Editor* mEditor = nullptr;
};

class AddBitmapElement : public BackupElement
{
public:
    AddBitmapElement(BitmapImage* backupBitmap, BitmapImage* bufferImage,
                     int backupLayerId,
                     int backupFrameIndex,
                     QString description,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);

    int oldLayerIndex = 0;
    int newLayerIndex = 0;

    int oldFrameIndex = 0;
    int newFrameIndex = 0;
    int previousFrameIndex = 0;

    int otherFrameIndex = 0;

    int oldLayerId = 0;
    int newLayerId = 0;

    int emptyFrameSettingVal = -1;

    BitmapImage* oldBitmap = nullptr;
    BitmapImage* newBitmap = nullptr;

    BitmapImage* oldBufferImage = nullptr;
    Layer* layer = nullptr;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;

    void redoTransform();
    void undoTransform();
};

class AddVectorElement : public BackupElement
{
public:
    AddVectorElement(VectorImage* backupVector,
                     int backupFrameIndex,
                     int backupLayerId,
                     QString description,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);

    int newLayerIndex = 0;
    int oldFrameIndex = 0;
    int newFrameIndex = 0;

    int newLayerId = 0;
    int oldLayerId = 0;
    int emptyFrameSettingVal = -1;

    VectorImage* oldVector = nullptr;
    VectorImage* newVector = nullptr;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;

};

class AddKeyFrameElement : public BackupElement
{
public:
    enum { Id = 5 };
    AddKeyFrameElement(int backupFrameIndex,
                       int backupLayerId,
                       int backupKeySpacing,
                       bool backupKeyExisted,
                       QString description,
                       Editor* editor,
                       QUndoCommand* parent = nullptr);


    int newLayerIndex = 0;
    int newFrameIndex = 0;
    int oldFrameIndex = 0;
    int oldLayerId = 0;
    int newLayerId = 0;

    std::map<int, KeyFrame*>oldKeyFrames;
    std::map<int, KeyFrame*>newKeyFrames;

    int oldKeySpacing = 0;
    bool oldKeyExisted = false;

    KeyFrame* newKey = nullptr;
    int emptyFrameSettingVal = -1;

    bool isFirstRedo = true;

    int type() override { return ADD_KEY_MODIF; }
    void undo() override;
    void redo() override;
    int id() const override { return Id; }
    bool mergeWith(const QUndoCommand *other) override;
};

class RemoveKeyFrameElement : public BackupElement
{
public:
    enum { Id = 4 };
    RemoveKeyFrameElement(KeyFrame* backupBitmap,
                          int backupLayerId,
                          Editor* editor,
                          QUndoCommand* parent = nullptr);

    int oldLayerIndex = 0;
    int oldFrameIndex = 0;
    int oldLayerId = 0;

    KeyFrame* oldKey = nullptr;

    BitmapImage* oldBitmap = nullptr;
    VectorImage* oldVector = nullptr;
    SoundClip* oldClip = nullptr;
    Camera* oldCamera = nullptr;

    bool isFirstRedo = true;

    int type() override { return REMOVE_KEY_MODIF; }
    void undo() override;
    void redo() override;
    int id() const override { return Id; }

};

class SelectionElement : public BackupElement
{
public:

    enum { Id = 1 };

    SelectionElement(SelectionType backupSelectionType,
                     QRectF backupTempSelection,
                     QRectF backupSelection,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);

    QRectF oldSelection = QRectF();
    QRectF newSelection = QRectF();

    QRectF oldTempSelection = QRectF();
    QRectF newTempSelection = QRectF();

    SelectionType selectionType;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *other) override;
    int id() const override { return Id; }

    void redoDeselection();
    void redoSelection();
    void undoDeselection();
    void undoSelection();
    void apply(int layerId,
               int frameIndex,
               BitmapImage* bitmap,
               VectorImage* vector);
};

class TransformElement : public BackupElement

{
public:

    enum { Id = 2 };
    TransformElement(KeyFrame* backupKeyFrame,
                     int backupLayerId,
                     int backupFramePos, QRectF backupSelection,
                     QRectF backupTempSelection, QTransform backupTransform,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);


    QRectF oldSelectionRect;
    QRectF newSelectionRect;

    QRectF oldSelectionRectTemp;
    QRectF newSelectionRectTemp;

    QTransform oldTransform;
    QTransform newTransform;

    BitmapImage* oldBitmap = nullptr;
    BitmapImage* newBitmap = nullptr;

    VectorImage* oldVector = nullptr;
    VectorImage* newVector = nullptr;

    int oldLayerId = 0;
    int newLayerId = 0;

    int oldFrameIndex = 0;
    int newFrameIndex = 0;

    bool isFirstRedo = true;
    void undo() override;
    void redo() override;
    void apply(QRectF tempRect,
               BitmapImage* bitmapImage,
               VectorImage* vectorImage,
               QRectF selectionRect,
               QTransform transform,
               int frameIndex,
               int layerId);

    bool mergeWith(const QUndoCommand *other) override;
    int id() const override { return Id; }
};

class QProgressDialog;
class ImportBitmapElement : public BackupElement
{

public:
    enum { Id = 7 };

    ImportBitmapElement(std::map<int, KeyFrame*, std::greater<int>>backupCanvasKeyFrames,
                        std::map<int, KeyFrame*, std::less<int> > backupImportedKeyFrames,
                        int backupLayerId,
                        Editor* editor,
                        QUndoCommand* parent = nullptr);

    std::map<int, KeyFrame*, std::greater<int>> oldKeyFrames;
    std::map<int, KeyFrame*,std::less<int>> importedKeyFrames;

    int oldLayerId = 0;
    int newLayerId = 0;

    bool isFirstRedo = true;

    QProgressDialog* progress = nullptr;

    void undo() override;
    void redo() override;
    int id() const override { return Id; }
};

class CameraMotionElement : public BackupElement
{
public:

    enum { Id = 3 };
    CameraMotionElement(QPointF backupTranslation,
                        float backupRotation,
                        float backupScale,
                        Editor* editor,
                        QUndoCommand* parent = nullptr);


    QPointF oldTranslation = QPointF(0,0);
    float  oldRotation = 0.0f;
    float oldScale = 0.0f;

    QPointF newTranslation = QPointF(0,0);
    float  newRotation = 0.0f;
    float newScale = 0.0f;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *other) override;
    int id() const override { return Id; }
};

class AddLayerElement : public BackupElement
{
public:
    AddLayerElement(Layer* backupLayer,
                    Editor* editor,
                    QUndoCommand* parent = nullptr);

    Layer* oldLayer;
    Layer* newLayer;

    QString newLayerName;

    Layer::LAYER_TYPE newLayerType = Layer::UNDEFINED;

    int newLayerId = 0;
    int oldLayerId = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class DeleteLayerElement : public BackupElement
{
public:
    DeleteLayerElement(QString backupLayerName,
                       Layer::LAYER_TYPE backupType,
                       std::map<int, KeyFrame*, std::greater<int> >,
                       int backupFrameIndex,
                       int backupLayerIndex,
                       int backupLayerId,
                       Editor* editor,
                       QUndoCommand* parent = nullptr);

    std::map<int, KeyFrame*, std::greater<int> >oldLayerKeys;

    QString oldLayerName;

    Layer::LAYER_TYPE oldLayerType;

    int oldLayerIndex = 0;
    int oldFrameIndex = 0;
    int oldLayerId = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class RenameLayerElement : public BackupElement
{
public:
    RenameLayerElement(QString backupLayerName,
                       int backupLayerId,
                       Editor* editor,
                       QUndoCommand* parent = nullptr);

    QString oldLayerName;
    QString newLayerName;

    int oldLayerIndex = 0;
    int newLayerIndex = 0;

    int oldLayerId = 0;
    int newLayerId = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class CameraPropertiesElement : public BackupElement
{
public:
    CameraPropertiesElement(QString backupLayerName,
                            QRect backupViewRect,
                            int backupLayerId,
                            Editor* editor,
                            QUndoCommand* parent = nullptr);

    QString oldLayerName;
    QString newLayerName;

    int oldLayerIndex = 0;
    int newLayerIndex = 0;

    QRect oldViewRect;
    QRect newViewRect;

    int oldLayerId = 0;
    int newLayerId = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class DragFrameElement : public BackupElement
{
public:
    DragFrameElement(int backupLayerIndex,
                     int backupFrameOffset,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);

    int layerId = 0;

    int frameOffset = 0;
    int endFrame = 0;
    int startFrame = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class FlipViewElement : public BackupElement
{
public:
    FlipViewElement(bool backupFlipEnabled,
                    DIRECTION backupFlipDirection,
                    Editor* editor,
                    QUndoCommand* parent = nullptr);

    bool isFlipped = false;

    DIRECTION direction;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

class MoveLayerElement : public BackupElement
{

public:
    MoveLayerElement(int backupOldLayerIndex,
                     int backupNewLayerIndex,
                     Editor* editor,
                     QUndoCommand* parent = nullptr);

    int oldLayerIndex = 0;
    int newLayerIndex = 0;

    bool isFirstRedo = true;

    void undo() override;
    void redo() override;
};

#endif // BACKUPELEMENT_H
