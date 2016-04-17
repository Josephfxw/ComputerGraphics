//
//  Blur.cpp
//  qip
//
//  Created by Joseph Fan on 4/16/16.
//
//



#include "MainWindow.h"
#include "Blur.hpp"

extern MainWindow *g_mainWindowP;
int Blurhistogram[MXGRAY];
int Blurmax =MXGRAY;
int Blurmin =0;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Blur::Blur(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Blur::applyFilter(ImagePtr I1, ImagePtr I2)
{
    
    
    
    // INSERT YOUR CODE HERE
    if (I1.isNull()) return 0;
    
    
    
        
        Blurmax = m_sliderB->value();
        
 
    
 
        
        Blurmin = m_sliderC->value();
  
    
    
    
    // error checking
    if(Blurmax < 0 || Blurmin > 255) return 0;
    if(Blurmin < 0 || Blurmax >255) return 0;
    if (Blurmin >= Blurmax)  Blurmax = Blurmin+1 ;
    
    
    
    blur(I1, Blurmin, Blurmax, I2);
    
    return 1;
    
    
    
    
    
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//


QGroupBox*
Blur::controlPanel()
{
    // init group box
    m_ctrlGrp = new QGroupBox("Blur");
    
    // INSERT YOUR CODE HERE
    // init widgets
    // create label[i]
    QLabel *labelC = new QLabel;
    labelC->setText(QString("Xsz"));
    QLabel *labelB = new QLabel;
    labelB->setText(QString("Ysz"));
    
    

    
    
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
   
    
    connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeMax(int)));
    connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeMax(int)));
    
    
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
Blur::changeMin(int contrast)
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
Blur::changeMax(int brightness)
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
template <class T>
void
blur1D_odd(ChannelPtr<T> src, int len, int stride, double ww, ChannelPtr<T> dst)
{
    
    ImagePtr I1 = IP_allocImage(MXRES, 1, FLOATCH_TYPE);
    
    ChannelPtr<T> srcp;
    ChannelPtr<T> buf = I1[0];
    
  
    if( src != dst && stride ==1)
        srcp = src;
    else {
        for(int i=0; i<len;++i,src += stride)buf[i] = *src;
        srcp = buf;
    }
    
    int ww2 = (int) (ww/2);
    int offset = (int) (-ww);
    
    double num = ww2;
    int lim = (int) num;
    double sum =0;
    int i =0;
    for (; i<lim;++i) sum += *srcp++;
    
    for (; num <ww; ++i){
        *dst = (T) (sum/num);
        dst += stride;
        sum += *srcp++;
        num += 1;
    }
    
    for(; i<len; ++i){
        sum -= srcp[offset];
        sum += *srcp++;
        *dst = (T)(sum/ww);
        dst += stride;
    }
    
   
    
    
    
    
    
    
}


template <class T>
void
IP_blur1D(ChannelPtr<T> src, int len, int stride, double ww, ChannelPtr<T> dst)
{
    //error checking
    if(ww>len){
        fprintf(stderr, "IP_blur1D: filter exceeds scanline (%f>%d)\n",ww,len);
        return;
    }
    
    //when filter width is less than a pixel
    if (ww<=1){
        if(src!=dst){
            for(int i=0; i<len;++i){
                *dst =*src;
                dst += stride;
                src += stride;
            }
        }
        return;
    }
    //check for odd window size: use simpler blur
    if (ww == (int) ww && (int) ww %2){
        blur1D_odd(src,len,stride,ww,dst);
        return;
    }
    ImagePtr I1 = IP_allocImage(MXRES, 1, FLOATCH_TYPE);
    
    ChannelPtr<T>buf = I1[0];
    double ww2 = (ww - 1.)/2;
    double wt1 = ww2 - (int)ww2;
    double wt2 = 1. - wt1;
    
    ChannelPtr<T> srcp;
    if( src != dst && stride ==1)
        srcp = src;
    else {
        for(int i=0; i<len;++i,src += stride)buf[i] = *src;
        srcp = buf;
    }
    
    double num = ww2 +1;
    
    /*
     
     This is the missing code that deals with the even number.
     
     
     
     */

    
    
    
    
    
}






void
Blur::blur(ImagePtr I1, double xsz, double ysz, ImagePtr I2)
{
    
    int w = I1->width();
    int h = I1->height();
    
    
    //error checking
    if (xsz >= MXBLUR -1 || ysz >= MXBLUR-1 || xsz>w || ysz >h){
        fprintf(stderr, "Blur: xsz, ysz = %f, %f w,h=%d, %d MXBLUR = %d\n", xsz,ysz,w,h,MXBLUR);
        IP_copyImage(I1, I2);
        return;
        
    }
    if (xsz <=1 && ysz<=1){
        if(I1 !=I2) IP_copyImage(I1, I2);
        return;
    }
    
    IP_copyImageHeader(I1, I2);
    
    int t;
    ChannelPtr<uchar> src, dst;
    ChannelPtr<float> fsrc, fdst;
    
    for (int ch =0; IP_getChannel(I1, ch, src, t); ch++){
        if (t ==UCHAR_TYPE){
            if(xsz >1.){
                dst =I2[ch];
                for(int y=0; y<h; y++){
                    IP_blur1D(src,w,1,xsz,dst);
                    src += w;
                    dst +=w;
                }
                src = I2[ch];
                    
            }
            if(ysz > 1.){
                dst = I2[ch];
                for(int x=0;x<w;x++){
                    IP_blur1D(src,h,w,ysz,dst);
                    
                    src +=1;
                    dst +=1;
                }
                
            }
            
        }else{
            IP_castChannel(I1, ch, I2, ch, FLOAT_TYPE);
            if(xsz >1.){
                fdst = fsrc = I2[ch];
                for(int y=0; y<h;y++){
                    IP_blur1D(fsrc,w,1,xsz,fdst);
                    fsrc +=w;
                    fdst +=w;
                }
                fsrc = I2[ch];
            }
            
            if(ysz >1.){
                fdst = I2[ch];
                for(int x=0;x<w;x++){
                    IP_blur1D(fsrc,h,w,ysz,fdst);
                    fsrc +=1;
                    fdst +=1;
                }
            }
        }
        
        
       
        
    }

    
    //int i,lut[MXGRAY];
   /* for (i = 0; i < MXGRAY; ++i) lut[i] = CLIP((i - minv)*MXGRAY / (maxv-minv), 0, MaxGray);
    
    
    int type;
    ChannelPtr<uchar> p1, p2, endd;
    for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        for (endd = p1 + total; p1<endd;) *p2++ = lut[*p1++];
    }
    
    */
    
}







// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Blur::reset() {
    m_sliderC->setValue(0);
    m_sliderB->setValue(0);
    m_spinBoxC->setValue(0);
    m_spinBoxB->setValue(0);
    
}