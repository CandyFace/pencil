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
#ifndef LayerBitmapSurface_H
#define LayerBitmapSurface_H

#include "layer.h"

class BitmapSurface;
class QDir;

class LayerBitmapSurface : public Layer
{
    Q_OBJECT

public:
    LayerBitmapSurface(Object* object);
    ~LayerBitmapSurface();

    QDomElement createDomElement(QDomDocument& doc) override;
    void loadDomElement(QDomElement element, QString dataDirPath, ProgressCallback progressStep) override;
    Status presave(const QString& sDataFolder) override;

    BitmapSurface* getBitmapImageAtFrame(int frameNumber);
    BitmapSurface* getLastBitmapImageAtFrame(int frameNumber, int increment = 0);

protected:
    Status saveKeyFrameFile(KeyFrame*, QString strPath) override;
    KeyFrame* createKeyFrame(int position, Object*) override;

private:
    void loadImageAtFrame(QString strFilePath, QPoint topLeft, int frameNumber);
    QString filePath(KeyFrame* key, const QDir& dataFolder) const;
    QString fileName(KeyFrame* key) const;
    bool needSaveFrame(KeyFrame* key, const QString& strSavePath);
};

#endif