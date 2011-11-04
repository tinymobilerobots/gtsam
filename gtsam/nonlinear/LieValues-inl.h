/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file LieValues.cpp
 * @author Richard Roberts
 */

#pragma once

#include <utility>
#include <iostream>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>

#include <gtsam/linear/VectorValues.h>
#include <gtsam/base/LieVector.h>
#include <gtsam/base/Lie-inl.h>
#include <gtsam/nonlinear/Ordering.h>

#include <gtsam/nonlinear/LieValues.h>

#define INSTANTIATE_LIE_VALUES(J) template class LieValues<J>;

using namespace std;

namespace gtsam {

/* ************************************************************************* */
  template<class J>
  void LieValues<J>::print(const string &s) const {
       cout << "LieValues " << s << ", size " << values_.size() << "\n";
       BOOST_FOREACH(const KeyValuePair& v, values_) {
         gtsam::print(v.second, (string)(v.first));
       }
     }

  /* ************************************************************************* */
  template<class J>
  bool LieValues<J>::equals(const LieValues<J>& expected, double tol) const {
    if (values_.size() != expected.values_.size()) return false;
    BOOST_FOREACH(const KeyValuePair& v, values_) {
    	if (!expected.exists(v.first)) return false;
      if(!gtsam::equal(v.second, expected[v.first], tol))
        return false;
    }
    return true;
  }

  /* ************************************************************************* */
  template<class J>
  const typename J::Value& LieValues<J>::at(const J& j) const {
    const_iterator it = values_.find(j);
    if (it == values_.end()) throw std::invalid_argument("LieValues::at() invalid j: " + (string)j);
    else return it->second;
  }

  /* ************************************************************************* */
  template<class J>
  size_t LieValues<J>::dim() const {
  	size_t n = 0;
  	BOOST_FOREACH(const KeyValuePair& value, values_)
  		n += value.second.dim();
  	return n;
  }

  /* ************************************************************************* */
  template<class J>
  VectorValues LieValues<J>::zero(const Ordering& ordering) const {
  	return VectorValues::Zero(this->dims(ordering));
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::insert(const J& name, const typename J::Value& val) {
    values_.insert(make_pair(name, val));
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::insert(const LieValues<J>& cfg) {
	  BOOST_FOREACH(const KeyValuePair& v, cfg.values_)
		 insert(v.first, v.second);
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::update(const LieValues<J>& cfg) {
	  BOOST_FOREACH(const KeyValuePair& v, values_) {
	  	boost::optional<typename J::Value> t = cfg.exists_(v.first);
	  	if (t) values_[v.first] = *t;
	  }
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::update(const J& j, const typename J::Value& val) {
	  	values_[j] = val;
  }

  /* ************************************************************************* */
  template<class J>
  std::list<J> LieValues<J>::keys() const {
	  std::list<J> ret;
	  BOOST_FOREACH(const KeyValuePair& v, values_)
		  ret.push_back(v.first);
	  return ret;
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::erase(const J& j) {
    size_t dim; // unused
    erase(j, dim);
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::erase(const J& j, size_t& dim) {
    iterator it = values_.find(j);
    if (it == values_.end()) throw std::invalid_argument("LieValues::erase() invalid j: " + (string)j);
    dim = it->second.dim();
    values_.erase(it);
  }

  /* ************************************************************************* */
  // todo: insert for every element is inefficient
  template<class J>
  LieValues<J> LieValues<J>::retract(const VectorValues& delta, const Ordering& ordering) const {
		LieValues<J> newValues;
		typedef pair<J,typename J::Value> KeyValue;
		BOOST_FOREACH(const KeyValue& value, this->values_) {
			const J& j = value.first;
			const typename J::Value& pj = value.second;
			Index index;
			if(ordering.tryAt(j,index)) {
			  newValues.insert(j, pj.retract(delta[index]));
			} else
			  newValues.insert(j, pj);
		}
		return newValues;
	}

  /* ************************************************************************* */
  template<class J>
  std::vector<size_t> LieValues<J>::dims(const Ordering& ordering) const {
    _ValuesDimensionCollector dimCollector(ordering);
    this->apply(dimCollector);
    return dimCollector.dimensions;
  }

  /* ************************************************************************* */
  template<class J>
  Ordering::shared_ptr LieValues<J>::orderingArbitrary(Index firstVar) const {
    // Generate an initial key ordering in iterator order
    _ValuesKeyOrderer keyOrderer(firstVar);
    this->apply(keyOrderer);
    return keyOrderer.ordering;
  }

//  /* ************************************************************************* */
//  // todo: insert for every element is inefficient
//  template<class J>
//  LieValues<J> LieValues<J>::retract(const Vector& delta) const {
//    if(delta.size() != dim()) {
//    	cout << "LieValues::dim (" << dim() << ") <> delta.size (" << delta.size() << ")" << endl;
//      throw invalid_argument("Delta vector length does not match config dimensionality.");
//    }
//    LieValues<J> newValues;
//    int delta_offset = 0;
//		typedef pair<J,typename J::Value> KeyValue;
//		BOOST_FOREACH(const KeyValue& value, this->values_) {
//			const J& j = value.first;
//			const typename J::Value& pj = value.second;
//      int cur_dim = pj.dim();
//      newValues.insert(j,pj.retract(sub(delta, delta_offset, delta_offset+cur_dim)));
//      delta_offset += cur_dim;
//    }
//    return newValues;
//  }

  /* ************************************************************************* */
  // todo: insert for every element is inefficient
  // todo: currently only logmaps elements in both configs
  template<class J>
  inline VectorValues LieValues<J>::unretract(const LieValues<J>& cp, const Ordering& ordering) const {
  	VectorValues delta(this->dims(ordering));
  	unretract(cp, ordering, delta);
  	return delta;
  }

  /* ************************************************************************* */
  template<class J>
  void LieValues<J>::unretract(const LieValues<J>& cp, const Ordering& ordering, VectorValues& delta) const {
    typedef pair<J,typename J::Value> KeyValue;
    BOOST_FOREACH(const KeyValue& value, cp) {
      assert(this->exists(value.first));
      delta[ordering[value.first]] = this->at(value.first).unretract(value.second);
    }
  }

}


