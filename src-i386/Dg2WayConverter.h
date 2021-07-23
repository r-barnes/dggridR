////////////////////////////////////////////////////////////////////////////////
//
// Dg2WayConverter.h: Dg2WayConverter class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DG2WAYCONVERTER_H
#define DG2WAYCONVERTER_H

class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class Dg2WayConverter {

   public:

      Dg2WayConverter (const DgConverterBase& forward,
                       const DgConverterBase& inverse);

      Dg2WayConverter (const Dg2WayConverter& con)
         : forward_ (&con.forward()), inverse_ (&con.inverse()) {}

      Dg2WayConverter& operator= (const Dg2WayConverter& con)
         { forward_ = &con.forward(); inverse_ = &con.inverse(); 
           return *this; }

      const DgConverterBase& forward (void) const { return *forward_; }
      const DgConverterBase& inverse (void) const { return *inverse_; }

   protected:

      const DgConverterBase* forward_;
      const DgConverterBase* inverse_;

};

////////////////////////////////////////////////////////////////////////////////
inline
Dg2WayConverter::Dg2WayConverter (const DgConverterBase& forward,
                                  const DgConverterBase& inverse)
{
   if (forward.fromFrame() != inverse.toFrame() ||
       forward.toFrame()   != inverse.fromFrame())
   {
      report("Dg2WayConverter::Dg2WayConverter() rf mismatch", DgBase::Fatal);
   }

   forward_ = &forward;
   inverse_ = &inverse;

} // Dg2WayConverter::Dg2WayConverter

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
