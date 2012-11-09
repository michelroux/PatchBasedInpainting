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

#ifndef PatchIndicatorVisitor_HPP
#define PatchIndicatorVisitor_HPP

#include <boost/graph/graph_traits.hpp>

// Custom
#include "Visitors/InpaintingVisitorParent.h"
#include "Mask/Mask.h"
#include "ITKHelpers/ITKHelpers.h"
#include "BoostHelpers/BoostHelpers.h"

// ITK
#include "itkImage.h"
#include "itkImageRegion.h"

/**
  * This visitor writes out information and images at each iteration.
 */
template <typename TGraph, typename TImage>
struct PatchIndicatorVisitor : public InpaintingVisitorParent<TGraph>
{
  const TImage* Image;
  const Mask* MaskImage;

  const unsigned int PatchHalfWidth;
  unsigned int NumberOfFinishedVertices = 0;

  typedef typename boost::graph_traits<TGraph>::vertex_descriptor VertexDescriptorType;
  
  PatchIndicatorVisitor(const TImage* const image, const Mask* const mask, const unsigned int patchHalfWidth,
                        const std::string& visitorName = "PatchIndicatorVisitor") :
  InpaintingVisitorParent<TGraph>(visitorName),
  Image(image), MaskImage(mask), PatchHalfWidth(patchHalfWidth)
  {

  }

  void FinishVertex(VertexDescriptorType targetNode, VertexDescriptorType sourceNode) override
  {
    itk::ImageRegion<2> fullRegion = this->Image->GetLargestPossibleRegion;

    itk::ImageRegion<2> fullTargetRegion =
        ITKHelpers::GetRegionInRadiusAroundPixel(targetNode, this->PatchHalfWidth);
    itk::ImageRegion<2> fullSourceRegion =
        ITKHelpers::GetRegionInRadiusAroundPixel(sourceNode, this->PatchHalfWidth);

    // Ensure that the source patch will match the target patch after it is cropped (this must be done before cropping the target region)
    itk::ImageRegion<2> sourceRegion = ITKHelpers::CropRegionAtPosition(fullSourceRegion,
                                                                        fullRegion, fullTargetRegion);

    // Ensure that the target patch is inside the image.
    itk::ImageRegion<2> targetRegion = fullTargetRegion;
    targetRegion.Crop(fullRegion);

    typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2> RGBImageType;
    RGBImageType::Pointer rgbImage = RGBImageType::New();
    rgbImage->SetRegions(this->Image->GetLargestPossibleRegion());
    rgbImage->Allocate();

    ITKHelpers::VectorImageToRGBImage(this->Image, rgbImage.GetPointer());

    RGBImageType::PixelType sourceColor; // green
    sourceColor[0] = 0;
    sourceColor[1] = 255;
    sourceColor[2] = 0;

    RGBImageType::PixelType targetColor; // red
    targetColor[0] = 255;
    targetColor[1] = 0;
    targetColor[2] = 0;

    RGBImageType::PixelType holeColor; // black
    holeColor[0] = 0;
    holeColor[1] = 0;
    holeColor[2] = 0;

    ITKHelpers::OutlineRegion(rgbImage.GetPointer(), sourceRegion, sourceColor);
    ITKHelpers::OutlineRegion(rgbImage.GetPointer(), targetRegion, targetColor);

    MaskOperations::WriteMaskedRegionPNG(rgbImage.GetPointer, this->MaskImage, rgbImage->GetLargestPossibleRegion(),
                                         Helpers::GetSequentialFileName("PatchIndicator",
                                                                        this->NumberOfFinishedVertices, "png"),
                                         holeColor);


    this->NumberOfFinishedVertices++;
  }

}; // PatchIndicatorVisitor

#endif
