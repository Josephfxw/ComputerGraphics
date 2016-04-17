//
//  Blur.hpp
//  qip
//
//  Created by Joseph Fan on 4/16/16.
//
//

#ifndef Blur_hpp
#define Blur_hpp

#include "ImageFilter.h"



class Blur : public ImageFilter {
    Q_OBJECT
    
public:
    
    Blur	(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
    void		reset		();		// reset parameters
    
protected:
    void blur(ImagePtr I1, double xsz, double ysz, ImagePtr I2);
    
    protected slots:
    void changeMin(int );
    void changeMax(int );
    
    
private:
    // brightness/contrast controls
    QSlider		*m_sliderB ;	// brightness slider
    QSlider		*m_sliderC ;	// contrast   slider
    QSpinBox	*m_spinBoxB;	// brightness spin box
    QSpinBox	*m_spinBoxC;	// contrast   spin box
    
    // widgets and groupbox
    QGroupBox	*m_ctrlGrp;	// groupbox for panel

    
};


#endif /* Blur_hpp */
