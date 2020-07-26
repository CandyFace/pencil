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

#include "brushtool.h"

#include <cmath>
#include <QSettings>
#include <QPixmap>
#include <QPainter>

#include "beziercurve.h"
#include "vectorimage.h"
#include "layervector.h"
#include "editor.h"
#include "colormanager.h"
#include "strokemanager.h"
#include "layermanager.h"
#include "viewmanager.h"
#include "selectionmanager.h"
#include "scribblearea.h"
#include "blitrect.h"
#include "pointerevent.h"
#include "brushsetting.h"


BrushTool::BrushTool(QObject* parent) : StrokeTool(parent)
{
}

ToolType BrushTool::type()
{
    return BRUSH;
}

void BrushTool::loadSettings()
{
    mPropertyEnabled[WIDTH] = true;
    mPropertyEnabled[FEATHER] = true;
    mPropertyEnabled[PRESSURE] = true;
    mPropertyEnabled[INVISIBILITY] = true;
    mPropertyEnabled[STABILIZATION] = true;

    mDefaultBrushSettings = { RadiusLog, PressureGain, Hardness };

    QSettings settings(PENCIL2D, PENCIL2D);

    properties.width = settings.value("brushWidth", 24.0).toDouble();
    properties.feather = settings.value("brushFeather", 48.0).toDouble();
    properties.pressure = settings.value("brushPressure", true).toBool();
    properties.invisibility = settings.value("brushInvisibility", false).toBool();
    properties.preserveAlpha = OFF;
    properties.stabilizerLevel = settings.value("brushLineStabilization", StabilizationLevel::STRONG).toInt();

    if (properties.width <= 0) { setWidth(15); }
    if (std::isnan(properties.feather)) { setFeather(15); }

    mQuickSizingProperties.insert(Qt::ShiftModifier, QuickPropertyType::WIDTH);
}

void BrushTool::resetToDefault()
{
    setWidth(24.0);
    setFeather(48.0);
    setStabilizerLevel(StabilizationLevel::STRONG);
}

void BrushTool::setWidth(const qreal width)
{
    // Set current property
    properties.width = width;
    mCurrentWidth = width;

    // Update settings
    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("brushWidth", width);
    settings.sync();
}

void BrushTool::setFeather(const qreal feather)
{
    // Set current property
    properties.feather = feather;

    // Update settings
    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("brushFeather", feather);
    settings.sync();
}

void BrushTool::setInvisibility(const bool invisibility)
{
    // force value
    properties.invisibility = invisibility;

    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("brushInvisibility", invisibility);
    settings.sync();
}

void BrushTool::setPressure(const bool pressure)
{
    // Set current property
    properties.pressure = pressure;

    // Update settings
    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("brushPressure", pressure);
    settings.sync();
}

void BrushTool::setStabilizerLevel(const int level)
{
    properties.stabilizerLevel = level;

    QSettings settings(PENCIL2D, PENCIL2D);
    settings.setValue("brushLineStabilization", level);
    settings.sync();
}

QCursor BrushTool::cursor()
{
    if (mEditor->preference()->isOn(SETTING::TOOL_CURSOR))
    {
        return QCursor(QPixmap(":icons/brush.png"), 0, 13);
    }
    return Qt::CrossCursor;
}

void BrushTool::pointerPressEvent(PointerEvent*)
{
    mScribbleArea->setAllDirty();
    mMouseDownPoint = getCurrentPoint();
    mLastBrushPoint = getCurrentPoint();

    startStroke();
    drawStroke();
}

void BrushTool::pointerMoveEvent(PointerEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        mCurrentPressure = strokeManager()->getPressure();
        drawStroke();
        if (properties.stabilizerLevel != strokeManager()->getStabilizerLevel())
            strokeManager()->setStabilizerLevel(properties.stabilizerLevel);
    }
}

void BrushTool::pointerReleaseEvent(PointerEvent*)
{
    endStroke();
}

// draw a single paint dab at the given location
void BrushTool::paintAt(QPointF point)
{
    //qDebug() << "Made a single dab at " << point;
    StrokeTool::drawStroke();
}

//void BrushTool::drawStroke()
//{
//    StrokeTool::drawStroke();
//    QList<QPointF> p = strokeManager()->interpolateStroke();

//    Layer* layer = mEditor->layers()->currentLayer();

//    if (layer->type() == Layer::BITMAP)
//    {
//        for (int i = 0; i < p.size(); i++)
//        {
//            p[i] = mEditor->view()->mapScreenToCanvas(p[i]);
//        }

//        qreal pressure = (properties.pressure) ? mCurrentPressure : 1.0;
//        qreal opacity = (properties.pressure) ? (mCurrentPressure * 0.5) : 1.0;
//        qreal brushWidth = properties.width * pressure;
//        mCurrentWidth = brushWidth;

//        qreal brushStep = (0.5 * brushWidth);
//        brushStep = qMax(1.0, brushStep);

//        BlitRect rect;

//        QPointF a = mLastBrushPoint;
//        QPointF b = getCurrentPoint();

//        qreal distance = 4 * QLineF(b, a).length();
//        int steps = qRound(distance / brushStep);

//        for (int i = 0; i < steps; i++)
//        {
//            QPointF point = mLastBrushPoint + (i + 1) * brushStep * (getCurrentPoint() - mLastBrushPoint) / distance;

//            rect.extend(point.toPoint());
//            mScribbleArea->drawBrush(point,
//                                     brushWidth,
//                                     properties.feather,
//                                     mEditor->color()->frontColor(),
//                                     opacity,
//                                     properties.useFeather,
//                                     properties.useAA);
//            if (i == (steps - 1))
//            {
//                mLastBrushPoint = getCurrentPoint();
//            }
//        }

//        int rad = qRound(brushWidth / 2 + 2);

//        mScribbleArea->paintBitmapBufferRect(rect);
//        mScribbleArea->refreshBitmap(rect, rad);

//        // Line visualizer
//        // for debugging
////        QPainterPath tempPath;

////        QPointF mappedMousePos = mEditor->view()->mapScreenToCanvas(strokeManager()->getMousePos());
////        tempPath.moveTo(getCurrentPoint());
////        tempPath.lineTo(mappedMousePos);

////        QPen pen( Qt::black,
////                   1,
////                   Qt::SolidLine,
////                   Qt::RoundCap,
////                   Qt::RoundJoin );
////        mScribbleArea->drawPolyline(tempPath, pen, true);

//    }
//    else if (layer->type() == Layer::VECTOR)
//    {
//        qreal pressure = (properties.pressure) ? mCurrentPressure : 1;
//        qreal brushWidth = properties.width * pressure;

//        int rad = qRound((brushWidth / 2 + 2) * mEditor->view()->scaling());

//        QPen pen(mEditor->color()->frontColor(),
//                 brushWidth * mEditor->view()->scaling(),
//                 Qt::SolidLine,
//                 Qt::RoundCap,
//                 Qt::RoundJoin);

//        if (p.size() == 4)
//        {
//            QPainterPath path(p[0]);
//            path.cubicTo(p[1], p[2], p[3]);

//            mScribbleArea->drawPath(path, pen, Qt::NoBrush, QPainter::CompositionMode_Source);
//            mScribbleArea->refreshVector(path.boundingRect().toRect(), rad);
//        }
//    }
//}

