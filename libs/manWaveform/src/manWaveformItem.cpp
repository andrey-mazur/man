#include <manWaveform/manWaveformItem.h>
#include <QtQuick/qsgsimplerectnode.h>
#include <QtQuick/qsgsimpletexturenode.h>
#include <QtQuick/qsgtextureprovider.h>
#include <private/qsgtexture_p.h>
#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLFramebufferObject>


manWaveformItem::manWaveformItem(QQuickItem * parent)
	: QQuickItem(parent)
	, _fbo(nullptr)
{
	setFlags(ItemHasContents);
}

QSGNode * manWaveformItem::updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * paintNodeData)
{
	/*
	QSGSimpleRectNode * node = static_cast<QSGSimpleRectNode *>(oldNode);
	if (!node)
	{
		node = new QSGSimpleRectNode(this->boundingRect(), QColor(Qt::blue));
	}
	*/
	/*
	QSGSimpleTextureNode * node = static_cast<QSGSimpleTextureNode *>(oldNode);
	if (!node)
	{
		node = new QSGSimpleTextureNode();
		QImage image("d:\\Development\\man\\apps\\host\\build\\Debug\\texture.jpg");
		QSGTexture * texture = QSGPlainTexture::fromImage(image);
		node->setTexture(texture);
	}

	node->setRect(this->boundingRect());
	*/
	QSGNode *node = oldNode;
	if (!node)
	{
		node = new QSGNode();
	}
	if (this->_fbo == NULL) {
		this->_fbo = new QOpenGLFramebufferObject(QSize(this->boundingRect().width(), this->boundingRect().height()));
		QSGTexture *m_texture = this->window()->createTextureFromId(this->_fbo->texture(), QSize(this->boundingRect().width(), this->boundingRect().height()), QQuickWindow::TextureHasAlphaChannel);
		QSGSimpleTextureNode *textureNode = new QSGSimpleTextureNode();
		textureNode->setTexture(m_texture);
		textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
		textureNode->setRect(this->boundingRect());
		textureNode->setFiltering(QSGTexture::Linear);
		node->appendChildNode(textureNode);
	}

	return node;
}
