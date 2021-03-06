/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

#ifndef ITIOTBVECTORIMAGEVIEWER_H
#define ITIOTBVECTORIMAGEVIEWER_H

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>

#include "../itiotbimageviewer.h"

#include "../../vector_globaldefs.h"

#include "../../models/itiotbVectorImageModel.h"


namespace itiviewer{

class ItiOtbVectorQGLWidgetScrollable;
class ItiOtbVectorQGLWidgetFullView;
class ItiOtbVectorQGLWidgetZoomable;
class ItiViewerObservableRegion;
class ItiViewerPixelInfoWidget;

/** \brief The ItiOtbVectorImageViewer class
 *
 *  This class implements a standard visualization tool to be
 *  plugged at the end of a pipeline.
 *  This viewer handles Vector images coming from ImagePorts which are specialized data IO classes handling OTB Images.
 *  This viewer's GUI designing and the general usability has been effected by the Monteverdi viewer version 1
 *  For more information take a look at the Orfeo ToolBox website \link http://www.orfeo-toolbox.org/otb/ \endlink
 *
 */
class ItiOtbVectorImageViewer : public ItiOtbImageViewer
{
    Q_OBJECT
public:
    explicit ItiOtbVectorImageViewer(QWidget *parent = 0);

    /*!
     *  \brief dtor!, ATM saving display GUI settings is called on the splitted mode version of the viewer.
     */
    ~ItiOtbVectorImageViewer();

    /*!
     * \brief disassembleWidgets.
     *
     *  Disassembles all children widgets, each widget has its own window
     */
    void disassembleWidgets();

    /*!
     * \brief assembleWidgets
     *
     *  Assembles all children widgets into one sinble parent widget.
     */
    void assembleWidgets();

    /*!
     * \brief draw
     *
     *  This is a accumulated function, each childrens gl widget's draw funtion is called seperately.
     */
    void draw();


    /** Compute the linear buffer index according to the 2D region and
     * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
     * The resulting buffer will be flipped over the X axis.
     * \param index 2D index
     * \param region 2D region
     */
    static inline unsigned int ComputeXAxisFlippedBufferIndex(const VectorIndexType& index, const VectorRegionType& region)
    {
      return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
              index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
    }

    /** Compute the linear buffer index according to the 2D region and
    * its 2D index.This method is used when OTB_GL_USE_ACCEL is ON.
    * \param index 2D index
    * \param region 2D region
    */
    static inline unsigned int ComputeBufferIndex(const VectorIndexType& index, const VectorRegionType& region)
    {
      return (index[1] - region.GetIndex()[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
    }

    /*!
     * \brief forceUpdates
     */
    void forceUpdates();



public slots:


private slots:
    /*!
     * \brief onScrollableWidgetSizeChanged
     *  update regions to notify observers
     * \param size
     */
    void onScrollableWidgetSizeChanged(const QRect &size);

    /*!
     * \brief onZoomableWidgetSizeChanged
     *  update regions to notify observers
     * \param size
     */
    void onZoomableWidgetSizeChanged(const QRect &size, double z = 1.0);

    /*!
     * \brief onFocusRegionTranslated
     *  update regions to notify observers
     * \param rect
     */
    void onFocusRegionChanged(const QRect &rect);

    /*!
     * \brief updateMouseTrackingState
     */
    void enableMouseTrackingState();


signals:
    /*!
     * \brief modelChanged
     *  Notify by emitting a signal informing others that the model has been changed
     */
    void modelChanged();

private:

    /*!
     * \brief saveDisplaySettings
     *  This function is called when the viewer is on a splitted mode,
     *  all it does is to save the GUI state of the widgets on an INI settings file.
     */
    void saveDisplaySettings();

    /*!
     * \brief readDisplaySettings
     *
     *  This function is called when the viewer is on the splitted mode,
     *  all it does is to load and read the GUI settings size and pos and restore the widgets's state
     *  according to the value red from the INI settings file.
     */
    void readDisplaySettings();


    /*!
     * \brief setupModel
     *
     *  Inside this function the model is being setup. The original file is being loaded.
     *  This is a crucial function for the viewer to work
     */
    void setupModel();

    /*!
     * \brief resetObserverMechanism, Resets all observers and stuff, as regards the observe mechanism take a look at the respective classes
     *  and the setupObserverMechanism() function
     *
     */
    void resetObserverMechanism();

    /*!
     * \brief setupConnections , setup the signal and slot mechanism.
     *  The connections used to handle the interactions
     */
    void setupConnections();

    /*!
     * \brief setupObserverMechanism
     *
     *  Register all objects to the observable object.
     *  Reset the observer mechanism, un register all previously registered observe objects.
     *  The Observer mechanism is explained better on the respective classes.
     *  The Observer pattern has been used in order to handle the user interaction and the ability of the viewer.
     *  to preview some rectangle aread on the scrollable and the full view widgets on top of the OpenGL image painted.
     */
    void setupObserverMechanism();

    /*!
     * \brief setupLayout
     *
     *  Setup the layout , craete widgets objects ana setup the layouts.
     */
    void setupLayout();

    /*!
     * \brief m_pItiOtbVectorImageWidgetScroll
     *  The scrollable widgets accepts scroll events and has a helper rectangle which sets the focus region
     */
    ItiOtbVectorQGLWidgetScrollable *m_pItiOtbVectorImageWidgetScroll;

    /*!
     * \brief m_pItiOtbVectorImageWidgetFull , The full widget always shows the whole image extents
     */
    ItiOtbVectorQGLWidgetFullView* m_pItiOtbVectorImageWidgetFullView;

    /*!
     * \brief m_pItiOtbVectorImageWidgetZoom
     *  The zoombable widget shows the image in various scales depending on the image zoom properties
     */
    ItiOtbVectorQGLWidgetZoomable *m_pItiOtbVectorImageWidgetZoomable;

    /*!
     * \brief m_pvBoxLayoutLeft
     *  The left vertical layout, needed for assembiling and disassembliing purposes
     *  The left layout contain two view widgets the zoomable and the full widget
     *  and the metadata widget.
     */
    QVBoxLayout *m_pvBoxLayoutLeft;

    /*!
     * \brief m_pVboxLayoutRight
     *  The right vertical layout, needed for assembiling and disassembliing purposes
     */
    QVBoxLayout *m_pVboxLayoutRight;

    /*!
     * \brief m_pMainLayout
     *  The main layout, needed for assembiling and disassembliing purposes
     */
    QVBoxLayout *m_pMainLayout;

    /*!
     * \brief m_pSplitter
     *  The horizontal splitter , the view is splitted horizontally..
     */
    QSplitter *m_pSplitter;

    //! the following labels were taken from the monteverdi viewer
    //! a label showing a title for the scrollable resolution view
    QLabel *m_pLabelScrollableResolution;

    /*!
     * \brief m_pLabelNavigationView
     *  a label showing a title for the navigation view
     */
    QLabel *m_pLabelFullView;

    /*!
     * \brief m_pLabelZoomView
     *  a label showing a title for the zoomable view
     */
    QLabel *m_pLabelZoomView;

    /*!
     * \brief m_pMetadataWidget
     *  For the moment , a Container widget
     */
    ItiViewerPixelInfoWidget *m_pItiViewerPixelInfoWidget;

    /*!
     * \brief m_focusRegion
     *  The m_focusRegion region extents equal to the zoomable widget extents, so the zoomable widget
     *  visualizes the content defined by the focus region.
     *  This instance is an observable item
     */
    ItiViewerObservableRegion *m_pFocusRegion;

    /*!
     * \brief m_pVisibleRegion
     *  The visible region extents equal to the visible part of the image shown on the scrollable widget view
     *  This instance is an observable item
     */
    ItiViewerObservableRegion *m_pVisibleRegion;

    /*!
     * \brief m_labelCss
     */
    QString m_labelCss;


    /*!
     * \brief m_pphelperWidgetScroll , this is a container widget, holding ItiOtbVectorQGLWidgetScrollable object.
     */
    QWidget *m_pphelperWidgetScroll;

    /*!
     * \brief m_pphelperWidgetFullView, this is a container widget, holding ItiOtbVectorQGLWidgetFullView object.
     */
    QWidget *m_pphelperWidgetFullView;

    /*!
     * \brief m_pphelperWidgetZoomView, this is a container widget, holding ItiOtbVectorQGLWidgetZoomable object.
     */
    QWidget *m_pphelperWidgetZoomView;
    
};

} // end of namespace itiviewer

#endif // ITIOTBVECTORIMAGEVIEWER_H
