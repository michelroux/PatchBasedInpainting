#ifndef TopPatchesTableModel_H
#define TopPatchesTableModel_H

// Qt
#include <QAbstractTableModel>
#include <QItemSelection>

// STL
#include <vector>

// Custom
#include "CandidatePairs.h"

class TopPatchesTableModel : public QAbstractTableModel
{
public:
  TopPatchesTableModel(std::vector<std::vector<CandidatePairs> >& allCandidatePairs);
  
  int rowCount(const QModelIndex& parent) const;
  int columnCount(const QModelIndex& parent) const;
  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  
  Qt::ItemFlags flags(const QModelIndex& index) const;
  void SetImage(FloatVectorImageType::Pointer image);
  
  void SetIterationToDisplay(const unsigned int);
  void SetForwardLookToDisplay(const unsigned int);
  
  void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
  
  void Refresh();
  
  void SetPatchDisplaySize(const unsigned int value);
protected:
  
  std::vector<std::vector<CandidatePairs> >& AllCandidatePairs;
  
  FloatVectorImageType::Pointer Image;
  unsigned int IterationToDisplay;
  unsigned int ForwardLookToDisplay;
  
  unsigned int PatchDisplaySize;
};

#endif
