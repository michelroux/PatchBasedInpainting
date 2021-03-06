/*=========================================================================
 *
 *  Copyright David Doria 2012 daviddoria@gmail.com
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

#ifndef RGBSSD_hpp
#define RGBSSD_hpp

// STL
#include <stdexcept>

// Submodules
#include <Helpers/Helpers.h>
#include <Helpers/ContainerInterface.h>
#include <ITKHelpers/ITKHelpers.h>
#include <ITKHelpers/ITKContainerInterface.h>

/**
  * This class is designed to compute the difference between RGB image pixels.
  */
template <typename PixelType>
struct RGBSSD
{

  float operator()(const PixelType& a, const PixelType& b) const
  {
    assert(length(a) == length(b));
//    if(length(a) != this->Weights.size())
//    {
//      std::stringstream ss;
//      ss << "length(a) != Weights.size(). a is " << length(a) << " and weights is " << Weights.size();
//      throw std::runtime_error(ss.str());
//    }
    
    float pixelDifference = 0.0f;

    for(unsigned int component = 0; component < 3; ++component)
    {
      float componentDifference = (Helpers::index(a,component) - Helpers::index(b,component)) *
                                  (Helpers::index(a,component) - Helpers::index(b,component));
      pixelDifference += componentDifference;
    }

    return pixelDifference;
  }
};

/**
  * This specialization handles pixels of type CovariantVector<T,N>.
  */
template <>
template <typename T, unsigned int N>
class RGBSSD<itk::CovariantVector<T, N> >
{
public:
  typedef itk::CovariantVector<T, N> PixelType;


  // This version uses a built-in ITK function. This function uses casts to double internally,
  // so it is not ideal for integer types
  // with values that you know will not cause overflow (unnecessarily slow)
//  float operator()(const PixelType& a, const PixelType& b) const
//  {
////    std::cout << "SumSquaredPixelDifference <itk::CovariantVector<T, N> >::operator()" << std::endl;
//    return (a-b).GetSquaredNorm();
//  }

  // Manual
  float operator()(const PixelType& a, const PixelType& b) const
  {
//    std::cout << "SumSquaredPixelDifference <itk::CovariantVector<T, N> >::operator()" << std::endl;
//    T sum = std::numeric_limits<T>::zero;
    T sum = static_cast<T>(0);

    for(unsigned int component = 0; component < 3; component++)
    {
      sum += (a[component] - b[component]) * (a[component] - b[component]);
    }
    return sum;
  }
};

#endif
