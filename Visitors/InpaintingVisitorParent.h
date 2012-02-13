#ifndef InpaintingVisitorParent_HPP
#define InpaintingVisitorParent_HPP

#include "Priority/Priority.h"

// Boost
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

/**
 * This is an abstract visitor that allows child visitors be stored as a vector<InpaintingVisitorParent*>.
 */
template <typename TGraph>
struct InpaintingVisitorParent
{
  typedef typename boost::graph_traits<TGraph>::vertex_descriptor VertexDescriptorType;

  virtual void initialize_vertex(VertexDescriptorType v, TGraph& g) const = 0;

  virtual void discover_vertex(VertexDescriptorType v, TGraph& g) const = 0;

  virtual void vertex_match_made(VertexDescriptorType target, VertexDescriptorType source, TGraph& g) const = 0;

  virtual void paint_vertex(VertexDescriptorType target, VertexDescriptorType source, TGraph& g) const = 0;

  virtual bool accept_painted_vertex(VertexDescriptorType v, TGraph& g) const = 0;

  virtual void finish_vertex(VertexDescriptorType v, TGraph& g) = 0;

}; // InpaintingVisitorParent

#endif