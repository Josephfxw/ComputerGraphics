// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Contrast.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Contrast::applyFilter(ImagePtr I1, ImagePtr I2)
{
    // INSERT YOUR CODE HERE
    if (I1.isNull()) return 0;
    
    
    // apply filter
    double b, c;	// brightness, contrast parameters
    b = m_sliderB->value();
    c = m_sliderC->value();
    
    // error checking
    if(b < -256 || b > 256) return 0;
    if(c < -100 || c > 100) return 0;
   
    
    contrast(I1, b, c, I2);
    
    return 1;
    
    
    
    

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Contrast");
    
    // INSERT YOUR CODE HERE
    // init widgets
    // create label[i]
    QLabel *labelC = new QLabel;
    labelC->setText(QString("Contrast"));
    QLabel *labelB = new QLabel;
    labelB->setText(QString("brightness"));
    
    // create slider
    m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderC->setTickPosition(QSlider::TicksBelow);
    m_sliderC->setTickInterval(25);
    m_sliderC->setMinimum(-100);
    m_sliderC->setMaximum(100);
    m_sliderC->setValue(0);
    
    m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderB->setTickPosition(QSlider::TicksBelow);
    m_sliderB->setTickInterval(25);
    m_sliderB->setMinimum(-256);
    m_sliderB->setMaximum(MXGRAY);
    m_sliderB->setValue(0);
    
    // create spinbox
    m_spinBoxC = new QSpinBox(m_ctrlGrp);
    m_spinBoxC->setMinimum(-100);
    m_spinBoxC->setMaximum(100);
    m_spinBoxC->setValue(0);
    
    m_spinBoxB = new QSpinBox(m_ctrlGrp);
    m_spinBoxB->setMinimum(-256);
    m_spinBoxB->setMaximum(MXGRAY);
    m_spinBoxB->setValue(0);
    
    // init signal/slot connections for Threshold
    connect(m_sliderC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
    connect(m_spinBoxC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
    
    connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness(int)));
    connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness(int)));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(labelC, 0, 0);
    layout->addWidget(m_sliderC, 0, 1);
    layout->addWidget(m_spinBoxC, 0, 2);
    
    layout->addWidget(labelB, 1, 0);
    layout->addWidget(m_sliderB, 1, 1);
    layout->addWidget(m_spinBoxB, 1, 2);
    
    // assign layout to group box
    m_ctrlGrp->setLayout(layout);
    
    
    return(m_ctrlGrp);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeContrast:
//
// Slot to process change in contrast caused by moving the slider.
//
void
Contrast::changeContrast(int contrast)
{
    m_sliderC->blockSignals(true);
    m_sliderC->setValue(contrast);
    m_sliderC->blockSignals(false);
    m_spinBoxC->blockSignals(true);
    m_spinBoxC->setValue(contrast);
    m_spinBoxC->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::changeBrightness:
//
// Slot to process change in brightness caused by moving the slider.
//
void
Contrast::changeBrightness(int brightness)
{
    m_sliderB->blockSignals(true);
    m_sliderB->setValue(brightness);
    m_sliderB->blockSignals(false);
    m_spinBoxB->blockSignals(true);
    m_spinBoxB->setValue(brightness);
    m_spinBoxB->blockSignals(false);
    
    // apply filter to source image; save result in destination image
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// contrast:
//
// INSERT YOUR CODE HERE.
//
void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    //double cont;
    
    if (contrast >= 0) {
        contrast = contrast / 25.0 + 1.0;
    }
    else {
        contrast = contrast / 133.0 + 1.0;
    }
    
    // compute lut[]
    int reference = 128;
    int i, lut[MXGRAY];
    for (i = 0; i < MXGRAY; ++i) lut[i] = CLIP((i - reference)*contrast + reference + brightness, 0, 225);
    
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for (endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {
    m_sliderC->setValue(0);
    m_sliderB->setValue(0);
    m_spinBoxC->setValue(0);
    m_spinBoxB->setValue(0);
    
}