#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgParamList.h: DgParamList class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPARAMLIST_H
#define DGPARAMLIST_H

#include "DgBase.h"
#include "DgString.h"
#include "DgUtil.h"

#include <cfloat>
#include <climits>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
class DgAssoc {

   public:

      DgAssoc (const string& nameIn)
         : isApplicable_(false), isValid_(false), isDefault_(true),
           isUserSet_(false), isUsed_(false)
         { name_ = toLower(nameIn); }

      virtual ~DgAssoc (void);

      virtual string valToStr (void) const = 0;

      const string& name (void) const { return name_; }

      string asString (void) const
           { return name() + " " + (isValid() ? valToStr() : string("INVALID"))
                    + " (" +
                       (!isApplicable() ? string("N/A") :
                         (isDefault() ? string("default") : string("user set")))
                    + ")"; }

      string validationErrMsg (void) const { return validationErrMsg_; }

      void setValidationErrMsg (const string& valErrMsgIn)
               { validationErrMsg_ = valErrMsgIn; }

      bool isApplicable (void) const { return isApplicable_; }

      bool setIsApplicable (bool isApplicableIn) { return (isApplicable_ = isApplicableIn); }

      bool isValid (void) const { return isValid_; }

      bool setIsValid (bool isValidIn) { return (isValid_ = isValidIn); }

      bool isDefault (void) const { return isDefault_; }

      bool setIsDefault (bool isDefaultIn) { return (isDefault_ = isDefaultIn); }

      bool isUserSet (void) const { return isUserSet_; }

      bool setIsUserSet (bool isUserSetIn) { return (isUserSet_ = isUserSetIn); }

      bool isUsed (void) const { return isUsed_; }

      bool setIsUsed (bool isUsedIn) { return (isUsed_ = isUsedIn); }

      virtual void setValStr (const string& valStr) = 0;

      DgAssoc& operator= (const DgAssoc& obj)
        {
           if (&obj != this)
           {
              name_ = obj.name();
              validationErrMsg_ = obj.validationErrMsg();
              isValid_ = obj.isValid();
           }

           return *this;
        }

      virtual bool validate (void) { return (isValid_ = true); }

   protected:

      string name_;
      string validationErrMsg_;

      bool isApplicable_;
      bool isValid_;
      bool isDefault_;
      bool isUserSet_;
      bool isUsed_;
};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgAssoc& assoc)
{
   stream << assoc.asString() << endl;

   return stream;

} // inline ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
class DgParamList {

   public:

      DgParamList (void) { }

     ~DgParamList (void);

      vector<DgAssoc*> parameters;

      void clearList (void);

      void loadParams (const string& fileName);

      void setParam (const string& nameIn, const string& strValIn);

      void setPresetParam (const string& nameIn, const string& strValIn);

      void insertParam (DgAssoc* param); // does not make a copy

      DgAssoc* getParam (const string& nameIn,
                         bool setToIsApplicable = true) const;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgParamList& plist)
{
   for (unsigned int i = 0; i < plist.parameters.size(); i++)
   {
      if (plist.parameters[i]->isUsed())
         stream << *(plist.parameters[i]);
   }

   return stream;

} // inline ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
template<class T> class DgParameter : public DgAssoc {

   public:

      DgParameter<T> (const string& nameIn) : DgAssoc (nameIn) { }

      DgParameter<T> (const string& nameIn, const T& valIn, bool validIn = true)
        : DgAssoc (nameIn), value_ (valIn) { DgAssoc::setIsValid(validIn); }

      const T& value (void) const { return value_; }

      virtual void setValue (const T& value) { value_ = value; validate(); }

      virtual void setValStr (const string& valStr)
        {
           setValue(strToVal(valStr));
        }

      virtual string valToStr (void) const = 0;
      virtual T strToVal (const string& strVal) const = 0;

      DgParameter<T>& operator= (const DgParameter<T>& obj)
        {
           if (&obj != this)
           {
              DgAssoc::operator=(obj);
              value_ = obj.value();
           }

           return *this;
        }

      DgParameter<T>& operator= (const DgAssoc& obj)
      {
         const DgParameter<T>* objT = dynamic_cast<const DgParameter<T>*>(&obj);
         if (!objT)
         {
            report(
            string("DgParameter::operator=() conflicting types for parameter "
                     + name(), DgBase::Fatal));
         }

         if (objT != this)
         {
            DgAssoc::operator=(*objT);
            value_ = objT->value();
         }

         return *this;
      }

   protected:

      T value_;
};

////////////////////////////////////////////////////////////////////////////////
template<class T> bool getParamValue (const DgParamList& plist,
                                      const string& name, T& var,
                                      bool dieOnFail = true)
{
   DgAssoc* assoc = plist.getParam(name);
   if (!assoc)
   {
      if (dieOnFail)
      {
         report(string("getParamValue() missing required parameter ") + name,
                DgBase::Fatal);
      }
      else
      {
         return false;
      }
   }

   const DgParameter<T>* assocT = dynamic_cast<const DgParameter<T>*>(assoc);
   if (!assocT)
   {
      if (dieOnFail)
      {
         report(string("getParamValue() type mismatch on parameter ") + name,
                DgBase::Fatal);
      }
      else
      {
         return false;
      }
   }

   // if we're here everything is peachy

   assoc->setIsUsed(true);
   var = assocT->value();

   return true;

} // template<class T> bool getParamValue

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgStringParam : public DgParameter<string> {

   public:

      DgStringParam (const string& nameIn) : DgParameter<string> (nameIn) { }

      DgStringParam (const string& nameIn, const string& valIn,
                     bool validIn = true)
        : DgParameter<string> (nameIn, valIn, validIn) { }

      virtual string valToStr (void) const { return value_; }
      virtual string strToVal (const string& strVal) const { return strVal; }

};

////////////////////////////////////////////////////////////////////////////////
class DgBoolParam : public DgParameter<bool> {

   public:

      DgBoolParam (const string& nameIn) : DgParameter<bool> (nameIn) { }

      DgBoolParam (const string& nameIn, bool valIn, bool validIn = true)
        : DgParameter<bool> (nameIn, valIn, validIn) { }

      virtual string valToStr (void) const
                        { return string(value() ? "true" : "false"); }

      virtual bool strToVal (const string& strVal) const
          {
             DgBoolParam* me = const_cast<DgBoolParam*>(this);
             string lower = toLower(strVal);
             me->setIsValid(true);

             if (lower == string("true")) return true;
             else if (lower == string("false")) return false;

             // if we're here we have a bad value

             me->setIsValid(false);

             me->setValidationErrMsg(string("Value '") + strVal +
               string("' is not one of the allowed values 'true' or 'false'"));

             return false;
          }

      virtual bool validate (void) { return isValid(); }
};

////////////////////////////////////////////////////////////////////////////////
template<class T>
class DgBoundedParam : public DgParameter<T> {

   public:

      DgBoundedParam (const string& nameIn) : DgParameter<T> (nameIn) { }

      DgBoundedParam (const string& nameIn, const T& valIn, const T& minIn,
                      const T& maxIn, bool validIn = true)
       : DgParameter<T> (nameIn, valIn, validIn), min_ (minIn), max_ (maxIn) { }

      DgBoundedParam (const string& nameIn, const T& minIn, const T& maxIn)
          : DgParameter<T> (nameIn), min_ (minIn), max_ (maxIn) { }

      virtual bool validate (void)
                {
                   DgAssoc::setIsValid((this->value() >= min()) && (this->value() <= max()));
                   return this->isValid();
                }

      DgBoundedParam& operator= (const DgBoundedParam& obj)
               {
                  if (&obj != this)
                  {
                     DgParameter<T>::operator=(obj);
                     min_ = obj.min();
                     max_ = obj.max();
                  }

                  return *this;

               } // DgBoundedParam::operator=

      // get methods

      const T& min (void) const { return min_; }
      const T& max (void) const { return max_; }

      // set methods

      void setMin (const T& minIn) { min_ = minIn; }
      void setMax (const T& maxIn) { max_ = maxIn; }

   private:

      T min_;
      T max_;

}; // class DgBoundedParam

////////////////////////////////////////////////////////////////////////////////
class DgIntParam : public DgBoundedParam<int> {

   public:

     DgIntParam (const string& nameIn, int minIn = INT_MIN,
                 int maxIn = INT_MAX)
          : DgBoundedParam<int> (nameIn, minIn, maxIn) { }

      DgIntParam (const string& nameIn, const int& valIn,
                  const int& minIn = INT_MIN, const int& maxIn = INT_MAX,
                  bool validIn = true)
        : DgBoundedParam<int> (nameIn, valIn, minIn, maxIn, validIn)
                {
                  if (!validate())
                  {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                  }
                }

      virtual string valToStr (void) const { return dgg::util::to_string(value_); }
      virtual int strToVal (const string& strVal) const
                {
			return dgg::util::from_string<int>(strVal);
                }

      virtual bool validate (void)
                {
                   DgBoundedParam<int>::validate();
                   if (!isValid())
                   {
                      setValidationErrMsg(string("value out of range ") +
                                  dgg::util::to_string(min()) + " to " +
                                  dgg::util::to_string(max()));
                   }
                   return isValid();
                }
};

////////////////////////////////////////////////////////////////////////////////
class DgLIntParam : public DgBoundedParam<long long int> {

   public:

     DgLIntParam (const string& nameIn,
                  long long int minIn = LLONG_MIN,
                  long long int maxIn = LLONG_MAX)
          : DgBoundedParam<long long int> (nameIn, minIn, maxIn) { }

      DgLIntParam (const string& nameIn, const long long int& valIn,
                  const long long int& minIn = LLONG_MIN,
		  const long long int& maxIn = LLONG_MAX,
                  bool validIn = true)
        : DgBoundedParam<long long int> (nameIn, valIn, minIn, maxIn, validIn)
                {
                  if (!validate())
                  {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                  }
                }

      virtual string valToStr (void) const { return dgg::util::to_string(value_); }
      virtual long long int strToVal (const string& strVal) const
                {
			return dgg::util::from_string<long long int>(strVal);
                }

      virtual bool validate (void)
                {
                   DgBoundedParam<long long int>::validate();
                   if (!isValid())
                   {
                      setValidationErrMsg(string("value out of range ") +
                                  dgg::util::to_string(min()) + " to " + dgg::util::to_string(max()));
                   }
                   return isValid();
                }
};

////////////////////////////////////////////////////////////////////////////////
class DgULIntParam : public DgBoundedParam<unsigned long int> {

   public:

     DgULIntParam (const string& nameIn, unsigned long int minIn = 0UL,
                 unsigned long int maxIn = ULONG_MAX)
          : DgBoundedParam<unsigned long int> (nameIn, minIn, maxIn) { }

      DgULIntParam (const string& nameIn, const unsigned long int& valIn,
                  const unsigned long int& minIn = 0UL,
                  const unsigned long int& maxIn = ULONG_MAX,
                  bool validIn = true)
        : DgBoundedParam<unsigned long int>
                          (nameIn, valIn, minIn, maxIn, validIn)
                {
                  if (!validate())
                  {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                  }
                }

      virtual string valToStr (void) const { return dgg::util::to_string(value_); }
      virtual unsigned long int strToVal (const string& strVal) const
                      { return dgg::util::from_string<unsigned long int>(strVal); }

      virtual bool validate (void)
                {
                   DgBoundedParam<unsigned long int>::validate();
                   if (!isValid())
                   {
                      setValidationErrMsg(string("value out of range ") +
                                  dgg::util::to_string(min()) + " to " + dgg::util::to_string(max()));
                   }
                   return isValid();
                }
};

////////////////////////////////////////////////////////////////////////////////
class DgUint64Param : public DgBoundedParam<unsigned long long int> {

   public:

      DgUint64Param (const string& nameIn, unsigned long long int minIn = 0ULL,
                   unsigned long long int maxIn = ULLONG_MAX)
          : DgBoundedParam<unsigned long long int> (nameIn, minIn, maxIn) { }

      DgUint64Param (const string& nameIn, const unsigned long long int& valIn,
                    const unsigned long long int& minIn = 0ULL,
                    const unsigned long long int& maxIn = ULLONG_MAX,
                    bool validIn = true)
        : DgBoundedParam<unsigned long long int> (nameIn, valIn, minIn, maxIn, validIn)
                {
                  if (!validate())
                  {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                  }
                }

      virtual string valToStr (void) const { return dgg::util::to_string(value_); }
      virtual unsigned long long int strToVal (const string& strVal) const
                      { return dgg::util::from_string<unsigned long long int>(strVal); }

      virtual bool validate (void)
                {
                   DgBoundedParam<unsigned long long int>::validate();
                   if (!isValid())
                   {
                      setValidationErrMsg(string("value out of range ") +
                              dgg::util::to_string(min()) + " to " + dgg::util::to_string(max()));
                   }
                   return isValid();
                }
};

////////////////////////////////////////////////////////////////////////////////
class DgDoubleParam : public DgBoundedParam<long double> {

   public:

      DgDoubleParam (const string& nameIn, long double minIn = LDBL_MIN,
                     long double maxIn = LDBL_MAX)
          : DgBoundedParam<long double> (nameIn, minIn, maxIn) { }

      DgDoubleParam (const string& nameIn, const long double& valIn,
                     const long double& minIn = LDBL_MIN,
                     const long double& maxIn = LDBL_MAX, bool validIn = true)
          : DgBoundedParam<long double> (nameIn, valIn, minIn, maxIn, validIn)
                {
                  if (!validate())
                  {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                  }
                }

      virtual string valToStr (void) const { return dgg::util::to_string(value_); }
      virtual long double strToVal (const string& strVal) const
                      { return dgg::util::from_string<long double>(strVal); }

      virtual bool validate (void)
                {
                   DgBoundedParam<long double>::validate();
                   if (!isValid())
                   {
                      setValidationErrMsg(string("value out of range ") +
                                  dgg::util::to_string(min()) + " to " +
                                  dgg::util::to_string(max()));
                   }
                   return isValid();
                }
};

////////////////////////////////////////////////////////////////////////////////
template<class T>
class DgChoiceParam : public DgParameter<T> {

   public:

      DgChoiceParam (const string& nameIn, const vector<T*>* choicesIn = 0)
          : DgParameter<T> (nameIn) { if (choicesIn) addChoices(*choicesIn); }

      DgChoiceParam (const string& nameIn, const T& valIn,
                     const vector<T*>* choicesIn = 0, bool validIn = true)
        : DgParameter<T> (nameIn, valIn, validIn)
      {
          if (choicesIn)
           addChoices(*choicesIn);
      }

     ~DgChoiceParam (void)
           { clearChoices(); }

      const vector<T*>& choices (void) const { return choices_; }

      void addChoices (const vector<T*>& choicesIn) // makes copy
            {
               for (unsigned int i = 0; i < choicesIn.size(); i++)
               {
                  choices_.push_back(new string(*choicesIn[i]));
               }
            }

      void clearChoices (void)
            {
		dgg::util::release(choices_);
            }

      virtual string valToStr (void) const = 0;
      virtual T strToVal (const string& strVal) const = 0;

      virtual bool validate (void)
                {
                   for (unsigned int i = 0; i < choices_.size(); i++)
                   {
                      if (*choices_[i] == this->value())
		       return this->setIsValid(true);
                   }

                   this->setValidationErrMsg(string("value not allowed"));

                   return this->setIsValid(false);
                }

      DgChoiceParam& operator= (const DgChoiceParam& obj)
               {
                  if (&obj != this)
                  {
                     DgParameter<T>::operator=(obj);

                     clearChoices();

                     for (unsigned int i = 0; i < obj.choices().size(); i++)
                        choices_.push_back(new T(*obj.choices()[i]));
                  }

                  return *this;

               } // DgChoiceParam::operator=

   protected:

      vector<T*> choices_;

}; // class DgChoiceParam

////////////////////////////////////////////////////////////////////////////////
class DgStringChoiceParam : public DgChoiceParam<string> {

   public:

      DgStringChoiceParam (const string& nameIn,
                           const vector<string*>* choicesIn = 0)
          : DgChoiceParam<string> (nameIn, choicesIn)
                {
                   for (unsigned int i = 0; i < choices_.size(); i++)
                   {
                      *choices_[i] = toLower(*choices_[i]);
                   }
                }

      DgStringChoiceParam (const string& nameIn, const string& valIn,
                     const vector<string*>* choicesIn = 0, bool validIn = true)
          : DgChoiceParam<string> (nameIn, valIn, choicesIn, validIn)
                {
                   for (unsigned int i = 0; i < choices_.size(); i++)
                   {
                      *choices_[i] = toLower(*choices_[i]);
                   }

                   if (!validate())
                   {
                     report(
                        string("Invalid initialization data for parameter:\n")
                        + name() + " " + valToStr() + string("\n") +
                        validationErrMsg(), DgBase::Fatal);
                   }
                }

      virtual string valToStr (void) const { return value_; }
      virtual string strToVal (const string& strVal) const { return strVal; }

      virtual bool validate (void)
                {
                   string lower = toLower(value());
                   for (unsigned int i = 0; i < choices_.size(); i++)
                   {
                      if (*choices_[i] == lower)
		       return this->setIsValid(true);
                   }

                   string err(string("Value '") + value() +
                        string("' is not one of the allowed values:\n"));
                   for (unsigned int i = 0; i < choices_.size(); i++)
                   {
                      err = err + *choices_[i] + string("\n");
                   }

                   setValidationErrMsg(err);

                   return this->setIsValid(false);
                }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
