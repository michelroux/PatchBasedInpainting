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

#ifndef PatchHelpers_H
#define PatchHelpers_H

// Qt
#include <QImage>

// Submodules
#include <Mask/Mask.h>

namespace PatchHelpers
{

////////////// Functions //////////////
bool CheckSurroundingRegionsOfAllHolePixels(const Mask* const mask, const unsigned int patchRadius);

////////////// Function templates //////////////
template <typename TImage>
QImage GetQImageCombinedPatch(const TImage* const image, const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion, const Mask* const mask);

template <class TImage>
void CopyRegion(const TImage* const sourceImage, TImage* const targetImage, const itk::Index<2>& sourcePosition,
                const itk::Index<2>& targetPosition, const unsigned int radius);

template <class TImage>
void CopyPatchIntoImage(const TImage* const patch, TImage* const image, const Mask* const mask,
                        const itk::Index<2>& position);

template <class TImage>
void CopyPatchIntoImage(const TImage* patch, TImage* const image, const itk::Index<2>& centerPixel);

template <class TPriorityQueue>
void WritePriorityQueue(TPriorityQueue q, const std::string& fileName);

template <typename TNodeQueue, typename TBoundaryStatusMap, typename TPriorityMap>
void DumpQueue(TNodeQueue nodeQueue, const TBoundaryStatusMap boundaryStatusMap, const TPriorityMap priorityMap);

/** Write an image of the priorities at the pixels that are valid in the propertyMap */
template <typename TNodeQueue, typename TPropertyMap>
void WriteValidQueueNodesPrioritiesImage(TNodeQueue nodeQueue, const TPropertyMap propertyMap,
                                         const itk::ImageRegion<2>& fullRegion, const std::string& fileName);

/** Write an image of the priorities at the pixels that are valid in the propertyMap */
template <typename TNodeQueue, typename TPropertyMap>
void WriteValidQueueNodesLocationsImage(TNodeQueue nodeQueue, const TPropertyMap propertyMap,
                                       const itk::ImageRegion<2>& fullRegion, const std::string& fileName);

/** Write an image of the patches in the container described by the iterators. The image has two columns -
  * the first column is the patches, and the second column is the patchIds. This is so we can visually identify
  * which patch number each patch is (these number are sequential). */
template <typename TIterator, typename TImage, typename TPropertyMap>
void WriteTopPatches(TImage* const image, TPropertyMap propertyMap, const TIterator first,
                     const TIterator last, const std::string& prefix, const unsigned int iteration);

/** Write an image of the patches in the container described by the iterators. The patches are laid out in a grid -
  * left to right, top to bottom. */
template <typename TIterator, typename TImage, typename TPropertyMap>
void WriteTopPatchesGrid(const TImage* const image, TPropertyMap propertyMap, const TIterator first,
                         const TIterator last, const std::string& prefix, const unsigned int iteration,
                         unsigned int gridWidth, unsigned int gridHeight);

template <typename TPatchContainer>
void WriteTopPatchesGrid(const TPatchContainer& patches,
                         unsigned int gridWidth, unsigned int gridHeight,
                         const std::string& outputFileName);

/** Read an image of top patches. */
std::vector<itk::Image<itk::CovariantVector<unsigned char, 3>, 2>::Pointer>
ReadTopPatchesGrid(const std::string& fileName, const unsigned int patchSize,
                   const unsigned int gridWidth, const unsigned int gridHeight);
}

#include "PatchHelpers.hpp"

#endif
