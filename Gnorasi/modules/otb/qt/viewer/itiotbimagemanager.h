#ifndef ITIOTBIMAGEMANAGER_H
#define ITIOTBIMAGEMANAGER_H

#include <QObject>

#include "voreen/core/ports/port.h"


namespace itiviewer{

/*!
 *
 *  This class is created in order to take over the functionality
 *  previously handled by the ImageView class declared in the
 *  otbImageView header file contained in the OTB library.
 *
 *  This is a singleton class in order the unique instance of this class to be
 *  used as an interface - console item , accessible from everywhere in
 *  the project.
 *
 *  This class has a voreen Port as member variable, this port holds data related to the image
 *  The concrete ItiOtbImageViewer class knows which kind of port is used a raster or vector port
 *
 */
class ItiOtbImageManager
{
public:
    //!
    static ItiOtbImageManager* instance();

    //! This function should be called at the destructor of the top level class
    static void deleteInstance();

    /** Set/Get the image to render */
    voreen::Port* imagePort() const { return m_pPort; }
    void setImagePort(voreen::Port* p) { m_pPort = p; }

    /** Set/Get the DEM directory */
    void setDemDirectory(const QString &d) { m_DEMDirectory = d; }
    QString demDirectory() const { return m_DEMDirectory; }

    /** Set/Get the DEM directory */
    void setGeoidFile(const QString &fp) { m_GeoidFile = fp; }
    QString geoidFile() const { return m_GeoidFile; }

private:
    //! ctor
    ItiOtbImageManager();

    //!dtor
    ~ItiOtbImageManager();

    //! this is the unique instance
    static ItiOtbImageManager* m_pInstance;

    /*! Pointer to the image , the port holds image related data.*/
    voreen::Port *m_pPort;

    /*! Path to the DEMDirectory (used if a VectorData is rendered */
    QString m_DEMDirectory;

    /*! GeoidFile filename (used if a VectorData is rendered */
    QString m_GeoidFile;
};

} // end of namespace itiviewer

#endif // ITIOTBIMAGEMANAGER_H
