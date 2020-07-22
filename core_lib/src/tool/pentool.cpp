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
#include "pentool.h"

#include <QPixmap>
#include <QSettings>

#include "vectorimage.h"
#include "layervector.h"
#include "colormanager.h"
#include "strokemanager.h"
#include "layermanager.h"
#include "viewmanager.h"
#include "selectionmanager.h"
#include "editor.h"
#include "scribblearea.h"
#include "blitrect.h"
#include "pointerevent.h"


PenTool::PenTool(QObject* parent) : StrokeTool(parent)
{
}

void PenTool::loadSettings()
{
    mPropertyEnabled[WIDTH] = true;
    mPropertyEnabled[PRESSURE] = true;
    mPropertyEnabled[VECTORMERGE] = true;
    mPropertyEnabled[ANTI_ALIASING] = true;
    mPropertyEnabled[STABILIZATION] = true;

    mDefaultBrushSettings = { RadiusLog, PressureGain, AntiAliasing };

    QSettings settings(PENCIL2D, PENCIL2D);

    properties.width = settings.value("penWidth", 12.0).toDouble();
    properties.pressure = settings.value("penPressure", true).toBool();
    properties.invisibility = OFF;
    properties.preserveAlpha = OFF;
    properties.stabilizerLevel = settings.value("penLineStabilization", StabilizationLevel::STRONG).toInt();

    mQuickSizingProperties.insert(Qt::ShiftModifier, WIDTH);
}

void PenTool::resetToDefault()
{
    setWidth(12.0);
    setUseFeather(false);
    setPressure(true);
    setStabilizerLevel(StabilizationLevel::STRONG);
}

void PenTool::setWidth(const qreal width)
{
    // Set current property
    properties.width = width;

    // Update settings
    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("penWidth", width);
    settings.sync();
}

void PenTool::setPressure(const bool pressure)
{
    // Set current property
    properties.pressure = pressure;

    // Update settings
    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("penPressure", pressure);
    settings.sync();
}

void PenTool::setStabilizerLevel(const int level)
{
    properties.stabilizerLevel = level;

    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("penLineStabilization", level);
    settings.sync();
}

QCursor PenTool::cursor()
{
    if (mEditor->preference()->isOn(SETTING::TOOL_CURSOR))
    {
        return QCursor(QPixmap(":icons/pen.png"), -5, 0);
    }
    return Qt::CrossCursor;
}

void PenTool::pointerPressEvent(PointerEvent *)
{
    mScribbleArea->setAllDirty();

    mMouseDownPoint = getCurrentPoint();
    mLastBrushPoint = getCurrentPoint();

    startStroke();
}

void PenTool::pointerMoveEvent(PointerEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        mCurrentPressure = strokeManager()->getPressure();
        drawStroke();
        if (properties.stabilizerLevel != strokeManager()->getStabilizerLevel())
            strokeManager()->setStabilizerLevel(properties.stabilizerLevel);
    }
}

void PenTool::pointerReleaseEvent(PointerEvent*)
{
//    mEditor->backup(typeName());
    endStroke();
}

// draw a single paint dab at the given location
void PenTool::paintAt(QPointF point)
{
    Q_UNUSED(point);
    // TODO: reimplement dabbing
    // mypaint doesn't seem to support it...
}

void PenTool::drawStroke()
{
    StrokeTool::drawStroke();
    QList<QPointF> p = strokeManager()->interpolateStroke();

    Layer* layer = mEditor->layers()->currentLayer();

    if (layer->type() == Layer::VECTOR)
    {
        qreal pressure = (properties.pressure) ? mCurrentPressure : 1.0;
        qreal brushWidth = properties.width * pressure;

        int rad = qRound((brushWidth / 2 + 2) * mEditor->view()->scaling());

        QPen pen(mEditor->color()->frontColor(),
                 brushWidth * mEditor->view()->scaling(),
                 Qt::SolidLine,
                 Qt::RoundCap,
                 Qt::RoundJoin);

        if (p.size() == 4)
        {
            QPainterPath path(p[0]);
            path.cubicTo(p[1], p[2], p[3]);
            mScribbleArea->drawPath(path, pen, Qt::NoBrush, QPainter::CompositionMode_Source);
            mScribbleArea->refreshVector(path.boundingRect().toRect(), rad);
        }
    }
}

void PenTool::paintBitmapStroke()
{
    mScribbleArea->paintBitmapBuffer();
}

void PenTool::paintVectorStroke(Layer* layer)
{
    if (mStrokePoints.empty())
        return;

    // Clear the temporary pixel path
    mScribbleArea->clearBitmapBuffer();
    qreal tol = mScribbleArea->getCurveSmoothing() / mEditor->view()->scaling();

    BezierCurve curve(mStrokePoints, mStrokePressures, tol);
    curve.setWidth(properties.width);
    curve.setFeather(properties.feather);
    curve.setFilled(false);
    curve.setInvisibility(properties.invisibility);
    curve.setVariableWidth(properties.pressure);
    curve.setColorNumber(mEditor->color()->frontColorNumber());

    auto pLayerVector = static_cast<LayerVector*>(layer);
    VectorImage* vectorImage = pLayerVector->getLastVectorImageAtFrame(mEditor->currentFrame(), 0);
    if (vectorImage == nullptr) { return; } // Can happen if the first frame is deleted while drawing
    vectorImage->addCurve(curve, mEditor->view()->scaling(), false);

    if (vectorImage->isAnyCurveSelected() || mEditor->select()->somethingSelected())
    {
        mEditor->deselectAll();
    }

    vectorImage->setSelected(vectorImage->getLastCurveNumber(), true);

    mScribbleArea->setModified(mEditor->layers()->currentLayerIndex(), mEditor->currentFrame());
    mScribbleArea->setAllDirty();
}
