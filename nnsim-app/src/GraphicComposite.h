#pragma once
#include "GraphicComponent.h"


template <typename T>
class GraphicComposite : public GraphicComponent
{
public:
	GraphicComposite(QGraphicsScene &scene, GraphicComponent *parent) : GraphicComponent(scene, parent) {};
	~GraphicComposite() { for (T* comp : components) delete comp; }
	
	QVector<T*> components;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override
	{
		paintComposite(painter, item, widget);
		//for (T* component : components)
			//component->paint(painter, item, widget);
	}
	virtual void paintComposite(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) = 0;
	void addComponent(T* component) { components.push_back(component); component->index = components.size() - 1; }
	void removeComponent(T* component) { components.removeAt(component->getIndex()); }
	void removeAllComponents() { components.clear(); }
	void eraseAllComponents() { for (T* comp : components) delete comp; removeAllComponents(); }
	T* getChild(int index) { return components.at(index); }
	void hide() { QGraphicsItem::hide(); for (T* comp : components) comp->hide(); }
	void show() { QGraphicsItem::show(); for (T* comp : components) comp->show(); }
};

