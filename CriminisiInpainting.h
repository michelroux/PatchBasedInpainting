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

#ifndef CriminisiInpainting_h
#define CriminisiInpainting_h

// Custom
#include "Helpers.h"
#include "Patch.h"
#include "Types.h"

// ITK
#include "itkCovariantVector.h"
#include "itkImage.h"
  
class CriminisiInpainting
{

public:

  enum DifferenceTypeEnum {DIFFERENCE_ALL, DIFFERENCE_ALL255, DIFFERENCE_DEPTH};
  
  void SetDifferenceType(const int);
  
  // Constructor
  CriminisiInpainting();

  void Iterate();

  // The real work is done here.
  void Inpaint();
  
  // Specify the image to inpaint.
  void SetImage(FloatVectorImageType::Pointer image);
  
  // Specify the region to inpaint.
  void SetMask(Mask::Pointer mask);
  
  // Specify the size of the patches to copy.
  void SetPatchRadius(const unsigned int);

  // Specify if you want to see debugging outputs.
  void SetDebugImages(const bool);
  
  void SetDebugMessages(const bool);
  
  // Compute the confidence values for pixels that were just inpainted.
  void UpdateConfidences(const itk::ImageRegion<2>& targetRegion, const float value);
  
  // Get the output of the inpainting.
  FloatVectorImageType::Pointer GetResult();
  
  // Get the current confidence image
  FloatScalarImageType::Pointer GetConfidenceImage();

  // Get the current confidence image
  FloatScalarImageType::Pointer GetPriorityImage();
  
  // Get the current boundary image
  UnsignedCharScalarImageType::Pointer GetBoundaryImage();

  // Get the current boundary image
  FloatVector2ImageType::Pointer GetBoundaryNormalsImage();
  
  // Get the current isophote image
  FloatVector2ImageType::Pointer GetIsophoteImage();

  // Get the current data image
  FloatScalarImageType::Pointer GetDataImage();
  
  // Get the current mask image
  Mask::Pointer GetMaskImage();
  
  void SetUseConfidence(const bool);
  void SetUseData(const bool);

  void Initialize();

  // Determine whether or not the inpainting is completed by seeing if there are any pixels in the mask that still need to be filled.
  bool HasMoreToInpaint();
  
  bool GetUsedTargetPatch(const unsigned int id, Patch& patch);
  bool GetUsedSourcePatch(const unsigned int id, Patch& patch);
  
  unsigned int GetIteration();
  
private:

  // This is the original Criminisi idea of filling the highest priority first
  void FindBestPatchForHighestPriority(Patch& sourcePatch, Patch& targetPatch);
  
  // This is a new idea to try to fill several patches and return the best pair
  void FindBestPatchLookAhead(Patch& sourcePatch, Patch& targetPatch);
  
  // This is the suggested value in Criminisi's paper, but it does not change anything at all, as we find argmax of the priorities, and alpha is a simple scaling factor of the priorities.
  static const float Alpha = 255;
  
  // Image to inpaint. This should not be modified throughout the algorithm.
  FloatVectorImageType::Pointer OriginalImage;
  
  // The CIELab conversion of the input RGB image
  FloatVectorImageType::Pointer CIELabImage;
  
  // The intermediate steps and eventually the result of the inpainting.
  FloatVectorImageType::Pointer CurrentOutputImage;
  
  // This image will be used for all patch to patch comparisons. It should point at either OriginalImage or CIELabImage.
  FloatVectorImageType::Pointer CompareImage;
  
  // The mask specifying the region to inpaint. This does not change throughout the algorithm - it is the original mask.
  Mask::Pointer OriginalMask;
  
  // This mask is updated as patches are copied.
  Mask::Pointer CurrentMask;
  
  // Keep track of the confidence of each pixel
  FloatScalarImageType::Pointer ConfidenceImage;

  // Keep track of the data term of each pixel
  FloatScalarImageType::Pointer DataImage;
  
  // The patch radius.
  itk::Size<2> PatchRadius;

  // Store the computed isophotes.
  FloatVector2ImageType::Pointer IsophoteImage;
  
  // Keep track of the edge of the region to inpaint.
  UnsignedCharScalarImageType::Pointer BoundaryImage;
  
  // Store the computed boundary normals.
  FloatVector2ImageType::Pointer BoundaryNormals;

  // Keep track of the priority of each pixel.
  FloatScalarImageType::Pointer PriorityImage;

  void ComputeAllDataTerms();
  
  // Functions
  void InitializeMask();
  void InitializeConfidence();
  void InitializeData();
  void InitializePriority();
  void InitializeImage();

  // Debugging
  void DebugWriteAllImages();
  void DebugWriteAllImages(const itk::Index<2>& pixelToFill, const itk::Index<2>& bestMatchPixel, const unsigned int iteration);
  void DebugWritePatch(const itk::Index<2>& pixel, const std::string& filePrefix, const unsigned int iteration);
  void DebugWritePatch(const itk::ImageRegion<2>& region, const std::string& filename);

  void DebugWritePatch(const itk::Index<2>& pixel, const std::string& filename);
  void DebugWritePixelToFill(const itk::Index<2>& pixelToFill, const unsigned int iteration);
  void DebugWritePatchToFillLocation(const itk::Index<2>& pixelToFill, const unsigned int iteration);

  itk::CovariantVector<float, 2> GetAverageIsophote(const itk::Index<2>& queryPixel);
  bool IsValidPatch(const itk::Index<2>& queryPixel, const unsigned int radius);
  
  itk::ImageRegion<2> CropToValidRegion(const itk::ImageRegion<2>& patch);

  unsigned int GetNumberOfPixelsInPatch();

  itk::Size<2> GetPatchSize();

  // Find the boundary of a region.
  void FindBoundary();
  
  // Compute the isophotes.
  void ComputeIsophotes();

  // Compute the normals of a region boundary.
  void ComputeBoundaryNormals();

  // Enlarge the mask so that isophotes are not computed over the mask/image boundary
  void ExpandMask();

  // Criminisi specific functions
  // Compute the priorities at all boundary pixels.
  void ComputeAllPriorities();
  
  // Compute the priority of a specific pixel.
  float ComputePriority(const itk::Index<2>& queryPixel);
  
  // Compute the Confidence at a pixel.
  float ComputeConfidenceTerm(const itk::Index<2>& queryPixel);
  
  // Compute the Data at a pixel.
  float ComputeDataTerm(const itk::Index<2>& queryPixel);

  // Return the highest priority pixel. Return the value of that priority by reference.
  itk::Index<2> FindHighestValueOnBoundary(FloatScalarImageType::Pointer priorityImage, float& maxPriority);

  // Update the mask so that the pixels in the region that was filled are marked as filled.
  void UpdateMask(const itk::ImageRegion<2> region);

  // Locate all patches that are completely inside of the image and completely inside of the source region
  void ComputeSourcePatches();
  
  // Store the list of source patches computed with ComputeSourcePatches()
  std::vector<Patch> SourcePatches;
  
 // This member tracks the current iteration. This is only necessary to help construct useful filenames for debugging outputs from anywhere in the class.
  unsigned int Iteration;
  
  // This variable determines which Difference subclass is instantiated.
  int DifferenceType;
  
  //// Debugging ////
  // Should we output images at every iteration?
  bool DebugImages;
  
  // Should we output verbose information about what is happenening at every iteration?
  bool DebugMessages;
  
  // Output a message if DebugMessages is set to true.
  void DebugMessage(const std::string&);
  
  // Output a message and a value if DebugMessages is set to true.
  template <typename T>
  void DebugMessage(const std::string& message, T value);

  // This is set when the image is loaded so that the region of all of the images can be addressed without referencing any specific image.
  itk::ImageRegion<2> FullImageRegion;
  
  // These are tracked for visualization purposes only.
  std::vector<Patch> UsedTargetPatches;
  std::vector<Patch> UsedSourcePatches;
  
  float HistogramDifferenceND(const Patch& patch1, const Patch& patch2);
  float HistogramDifference1D(const Patch& patch1, const Patch& patch2);
  
  unsigned int HistogramBinsPerDimension;
};

#include "CriminisiInpainting.hxx"

#endif