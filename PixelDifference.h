/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef PixelDifference_H
#define PixelDifference_H

#include <algorithm> // min, max

#include "itkVariableLengthVector.h"

// This class uses the built in operator-() to compute the difference.
// With certain input types, this could cause overflow problems (e.g
// subtracting 10u from 5u is undefined (unsigned char can't have negative
// values. See PixelDifferenceScalar for a difference that accounts for this.
template<typename TPixel>
struct PixelDifference
{
//   static float Difference(const TPixel& a, const TPixel& b)
//   {
//     return static_cast<float>(std::max(a,b) - std::min(a,b));
//   }

  static float Difference(const typename itk::VariableLengthVector<typename TPixel::ValueType>& a,
                          const typename itk::VariableLengthVector<typename TPixel::ValueType>& b)
  {
    float difference = 0;

    float diff = 0;
    for(unsigned int i = 0; i < a.GetSize(); ++i)
      {
      diff = fabs(a[i] - b[i]);
      difference += diff;
      }
    return difference;
  }

  template<typename U=TPixel>
  static float Difference(const typename std::enable_if<!std::is_same<U, typename itk::VariableLengthVector<typename TPixel::ValueType> >::value, TPixel >::type& a,
                          const typename std::enable_if<!std::is_same<U, typename itk::VariableLengthVector<typename TPixel::ValueType> >::value, TPixel >::type& b)
//   static float Difference(const typename std::enable_if<!std::is_same<U, itk::VariableLengthVector<TPixel> >::value, itk::VariableLengthVector<TPixel> >::type& a,
//                           const typename std::enable_if<!std::is_same<U, itk::VariableLengthVector<TPixel> >::value, itk::VariableLengthVector<TPixel> >::type& b)
  {
    return static_cast<float>(std::max(a,b) - std::min(a,b));
  }
};


#endif
