#pragma once
#include <QWidget>
#include <QProcess>

class QComboBox;
class QPushButton;
class QLabel;
class QGraphicsView;
class QGraphicsScene;
class QSlider;
class QLineEdit;
class Path;
class PurePursuitPath;




class MainWindow : public QWidget
{
    Q_OBJECT



public:
    QSlider* zoomSlider_{};
    // constructor
    explicit MainWindow(QWidget *parent = nullptr);
    // destructor
    ~MainWindow() override;

private slots:
    void sendToRobot();
    void updateZoom() const;
    void addSpline();
    void animate();

protected:
    void resizeEvent(QResizeEvent *event) override;
    // void mousePressEvent(QMouseEvent *event) override;

private:
    QPushButton* button_{};
    QProcess process_{};
    QComboBox* comPort_{};
    QLabel* portLabel_{};
    QSlider* panSlider_{};
    QLabel* zoomLabel_{};
    QLabel* panLabel_{};
    QPushButton* plusButton_{};
    QPushButton* animateButton_{};


public:
    QGraphicsView* graphicsView_{};
    QGraphicsScene* graphicsScene_{};
    PurePursuitPath* purePursuitPath_{};

    Path* path_{};
    void drawAxis() const;

    void animatePurePursuit();

    void animateRamsete();
};

