#pragma once
#include <QGraphicsItem>
#include <QGraphicsScene>

#ifndef GRAPHIC_COMPOSITE_DEFINITION
#define GRAPHIC_COMPOSITE_DEFINITION
class GraphicComponent;
template <typename T = GraphicComponent>
class GraphicComposite;
#endif


class GraphicComponent : public QObject, public QGraphicsItem
{
public:
	GraphicComponent(QGraphicsScene &scene, GraphicComponent *parent) : QObject(parent), QGraphicsItem(), parent(parent), index(-1) { scene.addItem((QGraphicsItem*)this); };
	~GraphicComponent() = default;
	int getIndex() const { return index; }
	GraphicComponent* getParent() const { return parent; }
	virtual GraphicComposite<>* getComposite() const { return nullptr; }
	int index;
	GraphicComponent *parent;
};
