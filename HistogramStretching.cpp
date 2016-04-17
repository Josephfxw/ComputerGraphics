//
//  HistogramStretching.cpp
//  qip
//
//  Created by Joseph Fan on 3/28/16.
//
//

// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "HistogramStretching.hpp"

extern MainWindow *g_mainWindowP;
int histogram[MXGRAY];
int max =MXGRAY;
int min =0;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
HistogramStretching::HistogramStretching(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
HistogramStretching::applyFilter(ImagePtr I1, ImagePtr I2)
{
    
    
    
    // INSERT YOUR CODE HERE
    if (I1.isNull()) return 0;
    
    
    
    if (!m_checkBoxMax->isChecked()) {
        
        max = m_sliderB->value();
        
    }
    
    if (!m_checkBoxMin->isChecked()){
        
        min = m_sliderC->value();
    }
    
    
    
    // error checking
    if(max < 0 || min > 255) return 0;
    if(min < 0 || max >255) return 0;
    if (min >= max)  max = min+1 ;
    
    
    
    histogramstretching(I1, min, max, I2);
    
    return 1;
    
    
    
    
    
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//


QGroupBox*
HistogramStretching::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("HistoStrentching");
    
    // INSERT YOUR CODE HERE
    // init widgets
    // create label[i]
    QLabel *labelC = new QLabel;
    labelC->setText(QString("Min"));
    QLabel *labelB = new QLabel;
    labelB->setText(QString("Max"));
    
    
    m_checkBoxMin = new QCheckBox(tr("MinAuto"), m_ctrlGrp);
    m_checkBoxMax = new QCheckBox(tr("MaxAuto"), m_ctrlGrp);
    
    
    // create slider
    m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderC->setTickPosition(QSlider::TicksBelow);
    m_sliderC->setTickInterval(25);
    m_sliderC->setMinimum(0);
    m_sliderC->setMaximum(255);
    m_sliderC->setValue(0);
    
    m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
    m_sliderB->setTickPosition(QSlider::TicksBelow);
    m_sliderB->setTickInterval(25);
    m_sliderB->setMinimum(0);
    m_sliderB->setMaximum(255);
    m_sliderB->setValue(0);
    
    // create spinbox
    m_spinBoxC = new QSpinBox(m_ctrlGrp);
    m_spinBoxC->setMinimum(0);
    m_spinBoxC->setMaximum(255);
    m_spinBoxC->setValue(0);
    
    
    m_spinBoxB = new QSpinBox(m_ctrlGrp);
    m_spinBoxB->setMinimum(0);
    m_spinBoxB->setMaximum(255);
    m_spinBoxB->setValue(0);
    
    // init signal/slot connections for Threshold
    connect(m_sliderC, SIGNAL(valueChanged(int)), this, SLOT(changeMin(int)));
    connect(m_spinBoxC, SIGNAL(valueChanged(int)), this, SLOT(changeMin(int)));
    connect(m_checkBoxMin, SIGNAL(stateChanged(int)), this, SLOT(changeAutoMin(int )));
    
    connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeMax(int)));
    connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeMax(int)));
    connect(m_checkBoxMax, SIGNAL(stateChanged(int)), this, SLOT(changeAutoMax(int )));
    
    // assemble dialog
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(labelC, 0, 0);
    layout->addWidget(m_sliderC, 0, 1);
    layout->addWidget(m_spinBoxC, 0, 2);
    layout->addWidget(m_checkBoxMin, 0, 3,Qt::AlignLeft);
    
    layout->addWidget(labelB, 1, 0);
    layout->addWidget(m_sliderB, 1, 1);
    layout->addWidget(m_spinBoxB, 1, 2);
    layout->addWidget(m_checkBoxMax, 1, 3,Qt::AlignLeft);
    
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
HistogramStretching::changeMin(int contrast)
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
HistogramStretching::changeMax(int brightness)
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
HistogramStretching::histogramstretching(ImagePtr I1, int minv, int maxv, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    //double cont;
    
    
    int i,lut[MXGRAY];
    for (i = 0; i < MXGRAY; ++i) lut[i] = CLIP((i - minv)*MXGRAY / (maxv-minv), 0, MaxGray);
    
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for (endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
}


void
HistogramStretching::changeAutoMax( int){
    
    ImagePtr I1 = g_mainWindowP->imageSrc();
    ImagePtr I2 = g_mainWindowP->imageDst();
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for (endd = p1 + total; p1<endd;) histogram[*p1++]++;
    }
    
    
    for(int i = MaxGray;i>=0;i--){
        
        if(!histogram[i])continue;
        max = i;
        break;
        
    }
    
    
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
    
}



void
HistogramStretching::changeAutoMin( int){
    
    ImagePtr I1 = g_mainWindowP->imageSrc();
    ImagePtr I2 = g_mainWindowP->imageDst();
    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w * h;
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for (endd = p1 + total; p1<endd;) histogram[*p1++]++;
    }
    
    
    
    for(int i =0;i<=MaxGray;i++){
        
        if(!histogram[i])continue;
        min= i;
        break;
        
    }
    
    
    applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
    
    // display output
    g_mainWindowP->displayOut();
    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
HistogramStretching::reset() {
    m_sliderC->setValue(0);
    m_sliderB->setValue(0);
    m_spinBoxC->setValue(0);
    m_spinBoxB->setValue(0);
    
}