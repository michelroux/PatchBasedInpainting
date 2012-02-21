#ifndef AverageDifferenceAcceptanceVisitor_HPP
#define AverageDifferenceAcceptanceVisitor_HPP

#include <boost/graph/graph_traits.hpp>

// Parent class
#include "Visitors/AcceptanceVisitors/AcceptanceVisitorParent.h"

// Custom
#include "ImageProcessing/Mask.h"
#include "Helpers/OutputHelpers.h"
#include "Helpers/ITKHelpers.h"
#include "Helpers/BoostHelpers.h"

// ITK
#include "itkImage.h"
#include "itkImageRegion.h"

/**
  
 */
template <typename TGraph, typename TImage>
struct AverageDifferenceAcceptanceVisitor : public AcceptanceVisitorParent<TGraph>
{
  TImage* Image;
  Mask* MaskImage;

  const unsigned int HalfWidth;

  float DifferenceThreshold;

  typedef typename boost::graph_traits<TGraph>::vertex_descriptor VertexDescriptorType;

  DebugVisitor(TImage* const image, Mask* const mask, const unsigned int halfWidth, const float differenceThreshold = 100) :
  Image(image), MaskImage(mask), HalfWidth(halfWidth), DifferenceThreshold(differenceThreshold)
  {

  }

  bool AcceptMatch(VertexDescriptorType target, VertexDescriptorType source) const
  {
    itk::Index<2> targetPixel = ITKHelpers::CreateIndex(target);
    itk::ImageRegion<2> targetRegion = ITKHelpers::GetRegionInRadiusAroundPixel(targetPixel, HalfWidth);

    itk::Index<2> sourcePixel = ITKHelpers::CreateIndex(source);
    itk::ImageRegion<2> sourceRegion = ITKHelpers::GetRegionInRadiusAroundPixel(sourcePixel, HalfWidth);

    // Compute the average of the valid pixels in the target region
    std::vector<itk::Index<2> > validPixelsTargetRegion = MaskImage->GetValidPixelsInRegion(targetRegion);
    typename TImage::PixelType averageTargetRegionSourcePixel = ITKHelpers::AverageOfPixelsAtIndices(Image, validPixelsTargetRegion);

    // Compute the average of the pixels in the source region corresponding to hole pixels in the target region.
    std::vector<itk::Offset<2> > holeOffsets = MaskImage->GetHoleOffsetsInRegion(targetRegion);
    std::vector<itk::Index<2> > sourcePatchValidPixels = ITKHelpers::OffsetsToIndices(holeOffsets, sourceRegion.GetIndex());
    typename TImage::PixelType averageSourceRegionTargetPixel = ITKHelpers::AverageOfPixelsAtIndices(Image, sourcePatchValidPixels);

    // Compute the difference
    float energy = (averageTargetRegionSourcePixel - averageSourceRegionTargetPixel).GetNorm();
    std::cout << "Energy: " << energy << std::endl;

    
    if(energy < DifferenceThreshold)
      {
      std::cout << "Match accepted." << std::endl;
      return true;
      }
    else
      {
      std::cout << "Match rejected." << std::endl;
      return false;
      }
  };

};

#endif