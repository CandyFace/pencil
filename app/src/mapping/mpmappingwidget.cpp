#include "mpmappingwidget.h"
#include <QDebug>

MPMappingWidget::MPMappingWidget(QString name, QString description, qreal min, qreal max, BrushInputType inputType, QVector<QPointF> points, int maxPoints, QWidget* parent)
    : MappingConfiguratorWidget(name, description, min, max, points, maxPoints, parent), mInputType(inputType)
{
    connect(this, &MappingConfiguratorWidget::mappingUpdated, this, &MPMappingWidget::updateMapping);
    connect(this, &MPMappingWidget::updateMappingWidget, this, &MappingConfiguratorWidget::updateInputsInMappingWidget);
}

MPMappingWidget::MPMappingWidget(QWidget* parent)
    : MappingConfiguratorWidget(parent)
{
}

void MPMappingWidget::updateMapping(QVector<QPointF> points)
{
    qDebug() << "mpmapwidget - updateMapping called";
    emit mappingForInputUpdated(points, mInputType);
}
