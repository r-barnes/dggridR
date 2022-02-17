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
// DgRunningStats.h: keep running statistics for a stream of numbers 
//
// Adapted from John D. Cook's implementation of an Knuth's algorithm.
// http://www.johndcook.com/blog/standard_deviation
//
// Version 7.0 - Kevin Sahr, 12/14/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRUNNINGSTATS_H
#define DGRUNNINGSTATS_H

////////////////////////////////////////////////////////////////////////////////
class DgRunningStats {

    public:
        DgRunningStats(void) : m_n(0) {}

        void clear()
        {
            m_n = 0;
        }

        void push(long double x)
        {
            m_n++;

            // See Knuth TAOCP vol 2, 3rd edition, page 232
            if (m_n == 1)
            {
                m_oldM = m_newM = x;
                m_oldS = 0.0;
                min_ = x;
                max_ = x;
            }
            else
            {
                m_newM = m_oldM + (x - m_oldM)/m_n;
                m_newS = m_oldS + (x - m_oldM)*(x - m_newM);

                if (x < min_)
                   min_ = x;

                if (x > max_)
                   max_ = x;
    
                // set up for next iteration
                m_oldM = m_newM; 
                m_oldS = m_newS;
            }
        }

        unsigned long long int numDataValues(void) const
        {
            return m_n;
        }

        long double min(void) const { return min_; }
        long double max(void) const { return max_; }

        long double mean(void) const
        {
            return (m_n > 0) ? m_newM : 0.0;
        }

        long double variance(void) const
        {
            return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 );
        }

        long double standardDeviation(void) const
        {
            return sqrt( variance() );
        }

    private:
        unsigned long long int m_n;
        long double m_oldM, m_newM, m_oldS, m_newS;
        long double min_, max_;
    };


////////////////////////////////////////////////////////////////////////////////

#endif
