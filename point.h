#ifndef _POINT_H_
#define _POINT_H_

/*!
    CPoint is the class that defines the position of 
	an element within a plane representation.
    \author Javier Bermejo García
*/
class CPoint {
    float X;       //!< Position X of the point.
    float Y;       //!< Position Y of the point.
    
private:
    /*!
        Calculates the distance between two X coordinates.
        \param x the coordinate with which one wants to calculate the distance.
        \return the distance.
    */
    float DistanceX(float x);
    
    /*!
        Calculates the distance between two Y coordinates.
        \param y the coordinate with which one wants to calculate the distance.
        \return the distance.
    */
    float DistanceY(float y);
    
public:
    /*!
        CPoint constructor.
    */
    CPoint(void);
	
	/*!
        CPoint constructor.
		\param x coordinate X.
        \param y coordinate Y.
    */
    CPoint(float x, float y);
    
    /*!
        Set the coordinate X.
        \param x coordinate X.
    */
    void SetX(float x);
    
    /*!
        Set the coordinate Y.
        \param y coordinate Y.
    */
    void SetY(float y);
    
    /*!
        Set the point coordinates.
        \param Point the point to set.
    */
    void SetPoint(CPoint Point);
    
    /*!
        Set the point coordinates.
        \param x coordinate X.
        \param y coordinate Y.
    */
    void SetPoint(float x, float y);
    
    /*!
        Get the coordinate X.
        \return the coordinate X.
    */
    float GetX(void);
    
    /*!
        Get the coordinate Y.
        \return the coordinate Y.
    */
    float GetY(void);
    
    /*!
        Check if two points are the same.
        \param Point point to compare with.
        \return true if success, false otherwise.
    */
    bool EqualPoints(CPoint Point);
    
    /*!
        Calculates the distance between two points.
        \param point to compare with.
        \return the distance.
    */
    float Distance(CPoint Point);
};

#endif
