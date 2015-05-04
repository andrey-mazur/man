#pragma once

#include <QtQuick/QQuickItem>


class QOpenGLFramebufferObject;
class manWaveformItem : public QQuickItem
{
	Q_OBJECT
public:
	explicit manWaveformItem(QQuickItem * parent = 0);

	QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
	QOpenGLFramebufferObject * _fbo;
};
