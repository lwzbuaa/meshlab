/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
#include "multiViewer_Container.h"
#include "glarea.h"

using namespace vcg; 

MultiViewer_Container::MultiViewer_Container(QWidget *parent)
: QWidget(parent)
{
	viewerList = QVector<Viewer*>();
	
	mainLayout = new QGridLayout;
	mainSplitter = new QSplitter();
	mainSplitter->setChildrenCollapsible(false);
	mainLayout->addWidget(mainSplitter);

	idCounter=0;

	layerDialog = new LayerDialog(this);
	layerDialog->setAllowedAreas (    Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	
	setLayout(mainLayout);
	
}

MultiViewer_Container::~MultiViewer_Container(){
	for each(Viewer* viewer in viewerList)
		delete viewer;
	delete mainLayout;
	delete mainSplitter;
	delete layerDialog;
}

int MultiViewer_Container::getNextViewerId(){
	return idCounter++;
}

void MultiViewer_Container::addView(Viewer* viewer, bool horiz){
	viewerList.append(viewer);
	int count = viewerCounter();

	if (count<=2){
		if(horiz)
			mainSplitter->setOrientation(Qt::Horizontal);
		else mainSplitter->setOrientation(Qt::Vertical);
		mainSplitter->addWidget((GLArea*)viewer);
	}
	else{
		GLArea* current = (GLArea*)currentView();
		QSplitter* parentSplitter = qobject_cast<QSplitter *>(current->parent());
		if(parentSplitter->count()==2){
			int index = parentSplitter->indexOf(current);
			QSplitter* newSplitter;
			if(horiz)
				newSplitter = new QSplitter(Qt::Horizontal);
			else newSplitter = new QSplitter(Qt::Vertical);
			parentSplitter->insertWidget(index, newSplitter);
			current->setParent(newSplitter);
			newSplitter->addWidget((GLArea*)viewer);

			newSplitter->setChildrenCollapsible(false);
		}
		else{
			if(horiz)
				parentSplitter->setOrientation(Qt::Horizontal);
			else parentSplitter->setOrientation(Qt::Vertical);
			parentSplitter->addWidget((GLArea*)viewer);
		}

	}


	//int row = count/2;
	//int column = count%2;
	//if(column==0){ //new splitter
	//	QSplitter* splitter = new QSplitter(Qt::Horizontal);
	//	hSplitters.append(splitter);
	//	splitter->addWidget((GLArea*)viewer);
	//	vSplitter->addWidget(splitter);
	//}
	//else 
	//	hSplitters.at(row)->addWidget((GLArea*)viewer);

	currentId = viewer->getId();
	//action for new viewer
    connect((GLArea*) viewer, SIGNAL(currentViewerChanged(int)), this, SLOT(updateCurrent(int)));

}

void MultiViewer_Container::removeView(int viewerId){
	for (int i=0; i< viewerList.count(); i++){
		Viewer* viewer = viewerList.at(i);
		if (viewer->getId() == viewerId){
			viewerList.remove(i);
			QSplitter* parentSplitter = qobject_cast<QSplitter *>(((GLArea*)viewer)->parent());
			delete viewer;
			if(parentSplitter->count()==0)
				delete parentSplitter;

			currentId = viewerList.at(0)->getId(); //per default il current � il primo viewer della lista (sempre presente)

			updateLayout();
		}
	}


		/*if(horiz)
					parentSplitter->setOrientation(Qt::Horizontal);
				else
					parentSplitter->setOrientation(Qt::Vertical);*/
}

void MultiViewer_Container::connectToLayerDialog(Viewer* viewer){
	connect(((GLArea*) viewer)->meshDoc, SIGNAL(currentMeshChanged(int)),layerDialog, SLOT(updateTable()));
}

void MultiViewer_Container::updateCurrent(int current){
	currentId=current;
}

void MultiViewer_Container::updateLayout(){
	for each(Viewer* viewer in viewerList)
		//splitter->removeWidget((GLArea*)viewer);
	for (int i=0; i< viewerList.count(); i++){
		int row = i/2;
		int column = i%2;
		//splitter->addWidget((GLArea*)viewerList.at(i));//, row, column); 
	}
}

Viewer* MultiViewer_Container::currentView(){
	for each ( Viewer* viewer in viewerList)
		if (viewer->getId() == currentId)
			return viewer;
}

int MultiViewer_Container::viewerCounter(){
	return viewerList.count();
}

void MultiViewer_Container::resizeEvent ( QResizeEvent * event ) {
	/*if(data->imageList.size()!=0){
		imageVis->adjustSize();
	}*/
}
/// update all the visual widgets at one time (glw, imageVis, tree, imageMag)
void MultiViewer_Container::updateAll(){
	/*glw->update();
	imageVis->update();
	imageMag->update();
	tree->updatePoints();*/
}
void MultiViewer_Container::keyPressEvent(QKeyEvent *keyEv){
	//keyEv->accept();
	//
	//// if the Delete key is pressed, then remove the selected point
	//if((data->selectedImagePoint.x()!=-1 || data->selectedMeshPoint!=-1) && keyEv->key() == Qt::Key_Delete){
	//	
	//	if(data->selectedImagePoint.x()!=-1){ 
	//		data->deleteImagePoint(data->selectedImagePoint);
	//		imageMag->update();
	//		glw->update();
	//	}
	//	else{ data->deleteMeshPoint(data->selectedMeshPoint);
	//	glw->update();
	//	}
	//	
	//}else  
	//	// if any of WASD keys are pressed, then move the selected image point
	//	if(data->selectedImagePoint.x()!=-1  && (keyEv->key() == Qt::Key_A || keyEv->key() == Qt::Key_S || keyEv->key() == Qt::Key_D || keyEv->key() == Qt::Key_W)){
	//		QPoint currentPoint = data->selectedImagePoint;
	//		
	//		switch(keyEv->key()){
	//			case Qt::Key_A:
	//				currentPoint.setX(currentPoint.x()-1);
	//				data->setSelectedImagePoint(currentPoint);
	//				break;
	//			case Qt::Key_D: 
	//				currentPoint.setX(currentPoint.x()+1);
	//				data->setSelectedImagePoint(currentPoint);
	//				break;
	//			case Qt::Key_W: 
	//				currentPoint.setY(currentPoint.y()-1);
	//				data->setSelectedImagePoint(currentPoint);
	//				break;
	//			case Qt::Key_S: 
	//				currentPoint.setY(currentPoint.y()+1);
	//				data->setSelectedImagePoint(currentPoint);
	//				break;
	//		}
	//	imageMag->update();
	//}
}