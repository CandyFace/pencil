#include "imagecompositor.h"

#include <QPainter>

ImageCompositor::ImageCompositor(QRect compositorRect, QPoint origin, QTransform compsitorView)
{
    mCanvasTransform = compsitorView;
    mOutputImage = QImage(compositorRect.size(), QImage::Format_ARGB32_Premultiplied);
    mOutputOrigin = origin;

    mOutputImage.fill(Qt::transparent);
}

void ImageCompositor::addImage(QImage& image)
{
    QPainter mainCompositor(&mOutputImage);
    mainCompositor.setTransform(mCanvasTransform);
    mainCompositor.translate(mOutputOrigin);
    mainCompositor.drawImage(QPoint(), image);

    mComposedImages.append(image);
}

void ImageCompositor::addEffect(CompositeEffect effect, QColor effectColor)
{
    QPainter effectPainter(&mOutputImage);

    if (effect == CompositeEffect::Colorize) {
        addColorizeEffect(effectPainter, effectColor);
    }
}

void ImageCompositor::addEffect(CompositeEffect effect, QTransform effectTransform, QRect selection)
{
    QPainter effectPainter(&mOutputImage);

    if (effect == CompositeEffect::Transformation) {
        addTransformationEffect(effectPainter, effectTransform, selection);
    }
}

void ImageCompositor::addColorizeEffect(QPainter& effectPainter, QBrush brush)
{
    effectPainter.setBrush(brush);
    effectPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    effectPainter.drawRect(effectPainter.viewport());
}

void ImageCompositor::addTransformationEffect(QPainter& effectPainter, QTransform effectTransform, QRect selection)
{
    Q_ASSERT_X(!mComposedImages.isEmpty(),
               "ImageCompositor::addTransformationEffect",
               "Make sure you have added an image before adding an effect");

    // Make sure there is something selected
    if (selection.width() == 0 && selection.height() == 0)
        return;

    QImage tranformedImage = QImage(selection.size(), QImage::Format_ARGB32_Premultiplied);
    tranformedImage.fill(Qt::transparent);

    QPainter imagePainter(&tranformedImage);
    imagePainter.translate(-selection.topLeft());
    imagePainter.drawImage(mOutputOrigin, mComposedImages.last());
    imagePainter.end();

    effectPainter.save();

    effectPainter.setTransform(mCanvasTransform);

    // Clear the painted area to make it look like the content has been erased
    effectPainter.save();
    effectPainter.setCompositionMode(QPainter::CompositionMode_Clear);
    effectPainter.fillRect(selection, QColor(255,255,255,255));
    effectPainter.restore();

    // Multiply the selection and view matrix to get proper rotation and scale values
    // Now the image origin will be topleft
    effectPainter.setTransform(effectTransform*mCanvasTransform);

    // Draw the selection image separately and on top
    effectPainter.drawImage(selection, tranformedImage);
    effectPainter.restore();
}

QImage ImageCompositor::output() const {
    return mOutputImage;
}