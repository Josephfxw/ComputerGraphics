//
//  HistogramStretching.hpp
//  qip
//
//  Created by Joseph Fan on 3/28/16.
//
//

#ifndef HistogramStretching_hpp
#define HistogramStretching_hpp



#include "ImageFilter.h"

class HistogramStretching : public ImageFilter {
    Q_OBJECT
    
public:
    
    HistogramStretching	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void histogramstretching(ImagePtr I1, int min, int max, ImagePtr I2);
    
    protected slots:
    void changeMin(int );
    void changeMax(int );
    void changeAutoMax ( int);
     void changeAutoMin ( int);
    
private:
    // brightness/contrast controls
    QSlider		*m_sliderB ;	// brightness slider
    QSlider		*m_sliderC ;	// contrast   slider
    QSpinBox	*m_spinBoxB;	// brightness spin box
    QSpinBox	*m_spinBoxC;	// contrast   spin box
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// groupbox for panel
    QCheckBox   *m_checkBoxMin;
    QCheckBox   *m_checkBoxMax;
    
};



#endif /* HistogramStretching_hpp */




