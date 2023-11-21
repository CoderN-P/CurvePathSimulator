#include <QGraphicsView>
#include <QGraphicsScene>

void drawAxis(QGraphicsView* graphicsView_, QGraphicsScene* graphicsScene_){
    int graphicsViewWidth = graphicsView_->width();
    int graphicsViewHeight = graphicsView_->height();
    graphicsScene_->addLine((int)(graphicsViewWidth/2), 0, (int)(graphicsViewWidth/2), graphicsViewHeight);
    graphicsScene_->addLine(0, (int)(graphicsViewHeight/2), graphicsViewWidth, (int)(graphicsViewHeight/2));

}
