/*

Pencil - Traditional Animation Software
Copyright (C) 2012-2018 Matthew Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <QMessageBox>
#include <QProgressDialog>
#include "bitmapcoloring.h"
#include "ui_bitmapcoloringwidget.h"
#include "layermanager.h"
#include "toolmanager.h"
#include "app_util.h"


BitmapColoring::BitmapColoring(Editor* editor, QWidget *parent) :
    BaseDockWidget(parent)
{
    QWidget* innerWidget = new QWidget;
    setWindowTitle(tr("Bitmap Coloring"));

    ui = new Ui::BitmapColoringWidget;
    ui->setupUi(innerWidget);
    setWidget(innerWidget);

    mEditor = editor;
    mScribblearea = mEditor->getScribbleArea();
    if (mEditor->layers()->currentLayer()->type() == Layer::BITMAP)
        mLayerBitmap = static_cast<LayerBitmap*>(mEditor->layers()->currentLayer());
    mBitmapImage = mLayerBitmap->getBitmapImageAtFrame(mEditor->currentFrame());
    ui->btnSelectAreas->setIcon(QIcon(":/icons/select.png"));
    checkBlackBoxes();
    checkRedBoxes();
    checkGreenBoxes();
    checkBlueBoxes();

    connect(ui->cbMethodSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BitmapColoring::colorMethodChanged);
    connect(ui->cb2TraceBlack, &QCheckBox::stateChanged, this, &BitmapColoring::checkBlackBoxes);
    connect(ui->cb2TraceRed, &QCheckBox::stateChanged, this, &BitmapColoring::checkRedBoxes);
    connect(ui->cb2TraceGreen, &QCheckBox::stateChanged, this, &BitmapColoring::checkGreenBoxes);
    connect(ui->cb2TraceBlue, &QCheckBox::stateChanged, this, &BitmapColoring::checkBlueBoxes);
    connect(ui->cb3TraceAllKeyframes, &QCheckBox::stateChanged, this, &BitmapColoring::checkAllKeyframesBoxes);

    // Prepare
    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, &BitmapColoring::tabWidgetClicked);
    connect(ui->cb1Threshold, &QCheckBox::stateChanged, this, &BitmapColoring::updateBtnSelect);
    connect(ui->sb1Threshold, QOverload<int>::of(&QSpinBox::valueChanged), this, &BitmapColoring::setThreshold);
    connect(ui->btnSelectAreas, &QPushButton::clicked, this, &BitmapColoring::activateSelectTool);
    connect(ui->btnApplyTrace, &QPushButton::clicked, this, &BitmapColoring::traceLines);
    // Thin
    connect(ui->sbSpotAreas, QOverload<int>::of(&QSpinBox::valueChanged), this, &BitmapColoring::setSpotArea);
    connect(ui->btnApplyThin, &QPushButton::clicked, this, &BitmapColoring::thinLines);
    // Finish
    connect(ui->btnApplyBlend, &QPushButton::clicked, this, &BitmapColoring::blendLines);

    updateUI();
}

BitmapColoring::~BitmapColoring()
{
    delete ui;
}

void BitmapColoring::initUI()
{
    updateUI();
}

void BitmapColoring::updateUI()
{
    if (mEditor->layers()->currentLayer()->type() == Layer::BITMAP)
        mLayerBitmap = static_cast<LayerBitmap*>(mEditor->layers()->currentLayer());
    if (mLayerBitmap == nullptr) { return; }

    if (mEditor->layers()->currentLayer()->type() == Layer::BITMAP)
    {
        if (mLayerBitmap->getHasColorLayer())
        {
            ui->tab1->setEnabled(true);
            ui->tab2->setEnabled(false);
            ui->tab3->setEnabled(false);
        }
        else if (mLayerBitmap->getIsColorLayer())
        {
            ui->tab1->setEnabled(false);
            ui->tab2->setEnabled(true);
            ui->tab3->setEnabled(true);
        }
        else
        {
            ui->tab1->setEnabled(true);
            ui->tab2->setEnabled(true);
            ui->tab3->setEnabled(true);
        }
    }
    else
    {   // If it is not a Bitmap Layer - disable
        ui->tab1->setEnabled(false);
        ui->tab2->setEnabled(false);
        ui->tab3->setEnabled(false);
    }
    mScribblearea->updateCurrentFrame();
}

void BitmapColoring::visibilityChanged(bool visibility)
{
    if (visibility)
        updateUI();
}

void BitmapColoring::colorMethodChanged()
{
    if (ui->cbMethodSelector->currentIndex() == 0)
    {
        ui->cb2TraceBlack->setChecked(false);
        ui->cb2TraceBlack->setEnabled(false);
        ui->cb2ThinBlack->setChecked(false);
        ui->cb2ThinBlack->setEnabled(false);
        ui->cb2BlendBlack->setChecked(false);
        ui->cb2BlendBlack->setEnabled(false);
    }
    else
    {
        ui->cb2TraceBlack->setEnabled(true);
        ui->cb2ThinBlack->setEnabled(true);
        ui->cb2BlendBlack->setEnabled(true);
        ui->cb2TraceBlack->setChecked(true);
    }
}

void BitmapColoring::checkBlackBoxes()
{
    if (ui->cb2TraceBlack->isChecked())
    {
        ui->cb2ThinBlack->show();
        ui->cb2BlendBlack->show();
        ui->cb2ThinBlack->setChecked(ui->cb2TraceBlack->isChecked());
        ui->cb2BlendBlack->setChecked(ui->cb2TraceBlack->isChecked());
    }
    else
    {
        ui->cb2ThinBlack->hide();
        ui->cb2BlendBlack->hide();
    }
}

void BitmapColoring::checkRedBoxes()
{
    if (ui->cb2TraceRed->isChecked())
    {
        ui->cb2ThinRed->show();
        ui->cb2BlendRed->show();
        ui->cb2ThinRed->setChecked(ui->cb2TraceRed->isChecked());
        ui->cb2BlendRed->setChecked(ui->cb2TraceRed->isChecked());
    }
    else
    {
        ui->cb2ThinRed->hide();
        ui->cb2BlendRed->hide();
    }
}

void BitmapColoring::checkGreenBoxes()
{
    if (ui->cb2TraceGreen->isChecked())
    {
        ui->cb2ThinGreen->show();
        ui->cb2BlendGreen->show();
        ui->cb2ThinGreen->setChecked(ui->cb2TraceGreen->isChecked());
        ui->cb2BlendGreen->setChecked(ui->cb2TraceGreen->isChecked());
    }
    else
    {
        ui->cb2ThinGreen->hide();
        ui->cb2BlendGreen->hide();
    }
}

void BitmapColoring::checkBlueBoxes()
{
    if (ui->cb2TraceBlue->isChecked())
    {
        ui->cb2ThinBlue->show();
        ui->cb2BlendBlue->show();
        ui->cb2ThinBlue->setChecked(ui->cb2TraceBlue->isChecked());
        ui->cb2BlendBlue->setChecked(ui->cb2TraceBlue->isChecked());
    }
    else
    {
        ui->cb2ThinBlue->hide();
        ui->cb2BlendBlue->hide();
    }
}

void BitmapColoring::checkAllKeyframesBoxes()
{
    ui->cb3ThinAllKeyframes->setChecked(ui->cb3TraceAllKeyframes->isChecked());
    ui->cb3BlendAllKeyframes->setChecked(ui->cb3TraceAllKeyframes->isChecked());
}

void BitmapColoring::tabWidgetClicked(int index)
{
    switch (index)
    {
    case 0:
        updateTraceBoxes();
        break;
    case 1:
        updateThinBoxes();
        break;
    case 2:
        updateBlendBoxes();
        break;
    default:
        updateTraceBoxes();
    }
}

// public Trace funtions
void BitmapColoring::updateTraceBoxes()
{
    if (mLayerBitmap->getIsColorLayer())
    {
        ui->tab1->setEnabled(false);
    }
    else
    {
        ui->tab1->setEnabled(true);
        ui->gb2Trace->setEnabled(true);
        if (ui->cbMethodSelector->currentIndex() == 0)
            ui->cb2TraceBlack->setEnabled(false);
    }
}

void BitmapColoring::updateBtnSelect()
{
    if (ui->cb1Threshold->isChecked())
    {
        ui->btnSelectAreas->setEnabled(true);
    }
    else
    {
        mSelectAreas = false;
        ui->btnSelectAreas->setIcon(QIcon(":/icons/select.png"));
        ui->btnSelectAreas->setEnabled(false);
    }
}

void BitmapColoring::activateSelectTool()
{
    if (!mSelectAreas)
    {
        mSelectAreas = true;
        ui->btnSelectAreas->setIcon(QIcon(":/icons/select_ok.png"));
        ToolManager* tool = mEditor->tools();
        tool->setCurrentTool(SELECT);
    }
    else {
        mSelectAreas = false;
        ui->btnSelectAreas->setIcon(QIcon(":/icons/select.png"));
    }
}

void BitmapColoring::setThreshold(int threshold)
{
    mBitmapImage->setThreshold(threshold);
}

void BitmapColoring::traceLines()
{
    if (mLayerBitmap == nullptr) { return; }

    if (mSelectAreas && !mScribblearea->isSomethingSelected())
    {
        QMessageBox::information(this, tr("No selection!"), tr("Please select area with select tool..."));
        return;
    }

    if (ui->cb3TraceAllKeyframes->isChecked())
    {
        QProgressDialog* mProgress = new QProgressDialog(tr("Tracing lines in bitmaps..."), tr("Abort"), 0, 100, this);
        mProgress->setWindowModality(Qt::WindowModal);
        mProgress->show();
        int keysToTrace = mLayerBitmap->keyFrameCount();
        mProgress->setMaximum(keysToTrace);
        mProgress->setValue(0);
        int keysTraced = 0;

        for (int i = mLayerBitmap->firstKeyFramePosition(); i <= mLayerBitmap->getMaxKeyFramePosition(); i++)
        {
            if (mLayerBitmap->keyExists(i))
            {
                mProgress->setValue(keysTraced++);
                QApplication::processEvents();
                mEditor->scrubTo(i);
                trace();
                if (mProgress->wasCanceled())
                {
                    break;
                }
            }
        }
        mProgress->close();
    }
    else if (mLayerBitmap->keyExists(mEditor->currentFrame()))
    {
        trace();
    }
    mScribblearea->deselectAll();
    ui->cb1Threshold->setChecked(false);
    updateBtnSelect();
}

// public Thin functions
void BitmapColoring::updateThinBoxes()
{
    if (mLayerBitmap->getHasColorLayer())
    {
        ui->tab2->setEnabled(false);
    }
    else
    {
        ui->tab2->setEnabled(true);
        ui->gb2Thin->setEnabled(true);
        if (ui->cbMethodSelector->currentIndex() == 0)
            ui->cb2ThinBlack->setEnabled(false);
    }
}

void BitmapColoring::setSpotArea(int size)
{
    mBitmapImage->setSpotArea(size);
}

void BitmapColoring::thinLines()
{
    if (mLayerBitmap == nullptr) { return; }

    if (!ui->cb3ThinAllKeyframes->isChecked())
    {
        if (mLayerBitmap->keyExists(mEditor->currentFrame()))
        {
            thin();
        }
    }
    else
    {
        QProgressDialog* mProgress = new QProgressDialog(tr("Thinning lines in bitmaps..."), tr("Abort"), 0, 100, this);
        mProgress->setWindowModality(Qt::WindowModal);
        mProgress->show();
        mProgress->setMaximum(mLayerBitmap->keyFrameCount());
        mProgress->setValue(0);
        int keysThinned = 0;

        for (int i = mLayerBitmap->firstKeyFramePosition(); i <= mLayerBitmap->getMaxKeyFramePosition(); i++)
        {
            if (mLayerBitmap->keyExists(i))
            {
                mEditor->scrubTo(i);
                thin();
                mProgress->setValue(keysThinned++);
                QApplication::processEvents();
            }
            if (mProgress->wasCanceled())
            {
                break;
            }
        }
        mProgress->close();
    }
    ui->cbSpotAreas->setChecked(false);
}

// public Blend functions
void BitmapColoring::updateBlendBoxes()
{
    if (mLayerBitmap->getHasColorLayer())
    {
        ui->tab3->setEnabled(false);
    }
    else
    {
        ui->tab3->setEnabled(true);
        if (ui->cbMethodSelector->currentIndex() == 0)
            ui->cb2BlendBlack->setEnabled(false);
    }
}

void BitmapColoring::blendLines()
{
    if (mLayerBitmap == nullptr) { return; }

    QString orgName = mLayerBitmap->name();
    orgName.chop(2);
    LayerBitmap* artLayer = static_cast<LayerBitmap*>(mEditor->layers()->findLayerByName(orgName));

    if (!ui->cb3BlendAllKeyframes->isChecked())
    {
        blend(artLayer);
    }
    else
    {
        QProgressDialog progress(tr("Blending lines in bitmaps..."), tr("Abort"), 0, 100, this);
        hideQuestionMark(progress);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        int keysToBlend = mLayerBitmap->keyFrameCount();
        progress.setMaximum(keysToBlend);
        int keysBlended = 0;

        for (int i = mLayerBitmap->firstKeyFramePosition(); i <= mLayerBitmap->getMaxKeyFramePosition(); i++)
        {
            if (mLayerBitmap->keyExists(i))
            {
                mEditor->scrubTo(i);
                blend(artLayer);
                progress.setValue(keysBlended++);
                QApplication::processEvents();
                if (progress.wasCanceled())
                {
                    break;
                }
            }
        }
    }
}

// protected functions
void BitmapColoring::traceScansToTransparent()
{
    if (ui->cb1Threshold->isChecked())
    {
        mEditor->copy();
        mEditor->layers()->currentLayer()->removeKeyFrame(mEditor->currentFrame());
        mEditor->layers()->currentLayer()->addNewKeyFrameAt(mEditor->currentFrame());
        mEditor->paste();
    }
    mBitmapImage = mLayerBitmap->getBitmapImageAtFrame(mEditor->currentFrame());
    mBitmapImage = mBitmapImage->scanToTransparent(mBitmapImage,
                                                   true,
                                                   ui->cb2TraceRed->isChecked(),
                                                   ui->cb2TraceGreen->isChecked(),
                                                   ui->cb2TraceBlue->isChecked());
}

void BitmapColoring::prepareLines()
{
    if (mLayerBitmap == nullptr) { return; }
    if (!mLayerBitmap->keyExists(mEditor->currentFrame())) { return; }

    LayerBitmap* colorLayer = nullptr;
    if (ui->cbMethodSelector->currentIndex() == 0)
    {           // if coloring is on same layer...
        colorLayer = mLayerBitmap;
    }
    else
    {           // if coloring is on separate layer...
        if (!mLayerBitmap->getHasColorLayer())
        {
            colorLayer = mEditor->layers()->createBitmapLayer(mLayerBitmap->name() + "_C");
            mLayerBitmap->setHasColorLayer(true);
            colorLayer->setIsColorLayer(true);
        }
        else {
            colorLayer = static_cast<LayerBitmap*>(mEditor->layers()->findLayerByName(mLayerBitmap->name() + "_C"));
        }
    }

    if (ui->cbMethodSelector->currentIndex() == 1)
    {
        colorLayer->setVisible(false);
        mLayerBitmap->copyFrame(mLayerBitmap, colorLayer, mEditor->currentFrame());
    }
    colorLayer->getBitmapImageAtFrame(mEditor->currentFrame())->traceLine(colorLayer->getBitmapImageAtFrame(mEditor->currentFrame()),
                                                                          ui->cb2TraceBlack->isChecked(),
                                                                          ui->cb2TraceRed->isChecked(),
                                                                          ui->cb2TraceGreen->isChecked(),
                                                                          ui->cb2TraceBlue->isChecked());
}

void BitmapColoring::trace()
{
    if (ui->cb1Threshold->isChecked())
        traceScansToTransparent();
    prepareLines();
}

void BitmapColoring::thin()
{
    mBitmapImage = mLayerBitmap->getBitmapImageAtFrame(mEditor->currentFrame());
    if (ui->cbSpotAreas->isChecked())
    {
        mBitmapImage->fillSpotAreas(mBitmapImage);
    }
    mBitmapImage->toThinLine(mBitmapImage,
                             ui->cb2ThinBlack->isChecked(),
                             ui->cb2ThinRed->isChecked(),
                             ui->cb2ThinGreen->isChecked(),
                             ui->cb2ThinBlue->isChecked());
    mEditor->backup(tr("Thin lines"));
    updateUI();
}

void BitmapColoring::blend(LayerBitmap *artLayer)
{
    mLayerBitmap->getBitmapImageAtFrame(mEditor->currentFrame())->blendLines(mLayerBitmap->getBitmapImageAtFrame(mEditor->currentFrame()),
                                                       ui->cb2BlendBlack->isChecked(),
                                                       ui->cb2BlendRed->isChecked(),
                                                       ui->cb2BlendGreen->isChecked(),
                                                       ui->cb2BlendBlue->isChecked());
    mEditor->backup(mEditor->layers()->currentLayerIndex(), mEditor->currentFrame(), tr("Blend lines"));
    if (ui->cbMethodSelector->currentIndex() == 1 && artLayer != nullptr)
    {
        artLayer->getBitmapImageAtFrame(mEditor->currentFrame())->traceLine(artLayer->getBitmapImageAtFrame(mEditor->currentFrame()),
                                                                            false,
                                                                            false,
                                                                            false,
                                                                            false);
        mEditor->backup(mEditor->layers()->currentLayerIndex(), mEditor->currentFrame(), tr("Blend lines"));
    }
    updateUI();
}
