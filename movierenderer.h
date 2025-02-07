/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef MOVIERENDERER_H
#define MOVIERENDERER_H

#include <QObject>
#include <QSize>
#include <QString>
#include <QDir>
#include <QFuture>
#include <QTimer>

class QOpenGLContext;
class QOpenGLFramebufferObject;
class QOffscreenSurface;
class QQuickRenderControl;
class QQuickWindow;
class QQmlEngine;
class QQmlComponent;
class QQuickItem;
class AnimationDriver;
class MovieRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(int fileProgress READ fileProgress NOTIFY fileProgressChanged)
public:
    enum Status {
        NotRunning,
        Running
    };

    explicit MovieRenderer(QObject *parent = 0);

    void renderMovie(const QString &qmlFile, const QString &filename, const QString &outputDirectory, const QString &outputFormat, const QSize &size, qreal devicePixelRatio = 1.0, int durationMs = 1000, int fps = 24);

    ~MovieRenderer();

    int progress() const;

    bool event(QEvent *event) override;
    bool isRunning();

    int fileProgress() const;

public slots:
    void setFileProgress(int fileProgress);

signals:
    void progressChanged(int progress);
    void finished();

    void fileProgressChanged(int fileProgress);

private slots:
    void start();
    void cleanup();

    void createFbo();
    void destroyFbo();
    bool loadQML(const QString &qmlFile, const QSize &size);

    void renderNext();

    void setProgress(int progress);
    void futureFinished();

    void slotTimer();


private:
    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
    QOpenGLFramebufferObject *m_fbo;
    qreal m_dpr;
    QSize m_size;
    AnimationDriver *m_animationDriver;

    QTimer *m_timer;


    int m_progress;
    Status m_status;
    int m_duration;
    int m_fps;
    int m_frames;
    int m_currentFrame;
    QString m_outputName;
    QString m_outputFormat;
    QString m_outputDirectory;

    QVector<QFutureWatcher<void> *> m_futures;
    int m_futureCounter;
    int m_fileProgress;

};

#endif // MOVIERENDERER_H
