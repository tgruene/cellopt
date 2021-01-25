/* 
 * File:   myExceptions.h
 * Author: tg
 *
 * Created on November 8, 2013, 10:16 AM
 */

#ifndef MYEXCEPTIONS_H
#define	MYEXCEPTIONS_H

#include <sstream>
#include <string>

namespace myExcepts{
    /**
     * \class myException
     * \brief generic exception
     */
    class myException {
    protected:
        const std::string msg_;
    public:
        myException (const std::string text): msg_ (text) {}
        ~myException() throw() {}
        virtual const std::string what() const throw() =0;
    };
    
    /**
     * \class FileMissing
     * \brief Thrown when expected file could not be opened
     */
    class FileMissing: public myException {
    public:
        FileMissing (const std::string text): myException(text) {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: File " << msg_ 
                    << " cannot be opened\n";
            return msg.str();
        }
    };
    /**
     * \class Keyword
     * \brief Thrown when keyword not found in input file
     */
    class Keyword: public myException {
    public:
        Keyword (const std::string text): myException(text) {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Keyword " << msg_ 
                    << " not found\n";
            return msg.str();
        }
    };
    /**
     * \class FileHeader
     * \brief Exception to be thrown for incomplete file header
     */
    class FileHeader: public myException  {
    public:
        FileHeader (const std::string text): myException (text) {}
        ~FileHeader() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Unexpected or incomplete file header. " << msg_ 
                    << " not found\n";
            return msg.str();
        }
    };
    
    /**
     * \class Dimension
     * \brief Wrong dimension in initialisation of matrix, vector, etc.
     */
    class Dimension: public myException {
    public:
        Dimension (const std::string text): myException (text) {}
        ~Dimension() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Dimension error: " << msg_ 
                    << "\n";
            return msg.str();
        }
    };

    /**
     * \class ShelxFormat
     * \brief Error parsing SHELXL ins-file
     */
    class ShelxFormat: public myException {
    public:
        ShelxFormat (const std::string text): myException (text) {}
        ~ShelxFormat() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Unsupported Image format. File " << msg_ 
                    << "\n";
            return msg.str();
        }
    };

        /**
     * \class ShelxEoF
     * \brief End of file (END or HKLF instruction)
     */
    class ShelxEoF: public myException {
    public:
        ShelxEoF (const std::string text): myException (text) {}
        ~ShelxEoF() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Unsupported Image format. File " << msg_ 
                    << "\n";
            return msg.str();
        }
    };

    /**
     * \class ImageFormat
     * \brief Exception to be thrown for unsupported image formats.
     */
    class Format: public myException {
    public:
        Format (const std::string text): myException (text) {}
        ~Format() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error: Error in input format. " << msg_ 
                    << "\n";
            return msg.str();
        }
    };

    /**
     * \class Usage
     * \brief Thrown when program comes to stage where it should not be
     */
    class Usage: public myException {
    public:
        Usage (const std::string text): myException (text) {}
        ~Usage() throw () {}
        const std::string what() const throw () {
            std::ostringstream msg;
            msg << " *** Error in Program Flow: " << msg_ 
                    << "\n";
            return msg.str();
        }
    };
}

#endif	/* MYEXCEPTIONS_H */

