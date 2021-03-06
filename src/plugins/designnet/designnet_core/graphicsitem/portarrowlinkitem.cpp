#include "portarrowlinkitem.h"
#include "portgraphicsitem.h"
#include "GraphicsUI/arrowlinkcontrolitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QMoveEvent>
#include <QGraphicsScene>
#include <QDebug>
namespace DesignNet{

PortArrowLinkItem::PortArrowLinkItem(PortGraphicsItem *sourcePort,
                                     QGraphicsItem *parent)
    : ArrowLinkItem(parent),
      m_sourcePort(sourcePort),
      m_targetPort(0),
      m_bBeginDrag(false)
{
    m_controlPoint_1->setParentItem(m_sourcePort);
}

PortArrowLinkItem::~PortArrowLinkItem()
{

}

void PortArrowLinkItem::setSourcePort(PortGraphicsItem *sourcePort)
{
    m_sourcePort = sourcePort;
    if(sourcePort)
    {
        setStartPoint(sourcePort->scenePos());
        m_controlPoint_1->setParentItem(m_sourcePort);
    }
}

PortGraphicsItem *PortArrowLinkItem::getSourcePort() const
{
    return m_sourcePort;
}

void PortArrowLinkItem::setTargetPort(PortGraphicsItem *targetPort)
{
    m_targetPort = targetPort;
	m_controlPoint_2->setParentItem(m_targetPort);
    setEndPoint(targetPort->scenePos());
    if(m_sourcePort && m_targetPort)
    {
        emit arrowConnected(m_sourcePort, m_targetPort);
		m_sourcePort->installSceneEventFilter(this);
		m_targetPort->installSceneEventFilter(this);
    }
}

PortGraphicsItem *PortArrowLinkItem::getTargetPort() const
{
    return m_targetPort;
}

QPointF PortArrowLinkItem::getStartPoint() const
{
    return mapFromItem(m_sourcePort,
                       m_sourcePort->boundingRect().right(), 0);
}

QPointF PortArrowLinkItem::getEndPoint() const
{
    if(m_targetPort)
        return mapFromItem(m_targetPort,
                           m_targetPort->boundingRect().left(), 0);

    return ArrowLinkItem::getEndPoint();
}

QPainterPath PortArrowLinkItem::shape() const
{
    QPointF c1 = getControlItemPosFirst();
    QPointF c2 = getControlItemPosSecond();
    QPainterPath path(getStartPoint());
    path.cubicTo(c1, c2, getEndPoint());
    path.cubicTo(c2, c1, getStartPoint());
    QPainterPathStroker pathStroker;
    pathStroker.setWidth(5);
    return pathStroker.createStroke(path);
}

void PortArrowLinkItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{    
    ArrowLinkItem::mousePressEvent(event);
}

void PortArrowLinkItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    PortGraphicsItem *sourcePort = getSourcePort();
    if(!sourcePort)
        return ;

    this->setEndPoint(event->pos());
    update();
    m_bBeginDrag = true;
	event->ignore();
	ArrowLinkItem::mouseMoveEvent(event);
}

int PortArrowLinkItem::type() const
{
    return UserType + PortArrowLinkItemType;
}

bool PortArrowLinkItem::sceneEventFilter( QGraphicsItem *watched, QEvent *event )
{
	QMoveEvent * mevent = dynamic_cast<QMoveEvent *>(event);
	if(mevent)
		updateGeometory();
	
	return false;
}

void PortArrowLinkItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	ArrowLinkItem::paint(painter, option, widget);
}

}
