/**
 * @file cxx/machine/machine/KMeansMachine.h
 * @date Tue May 10 11:35:58 2011 +0200
 * @author Francois Moulin <Francois.Moulin@idiap.ch>
 *
 * Copyright (C) 2011-2012 Idiap Research Institute, Martigny, Switzerland
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BOB_MACHINE_KMEANSMACHINE_H
#define BOB_MACHINE_KMEANSMACHINE_H

#include <blitz/array.h>
#include <cfloat>

#include "io/Arrayset.h"
#include "io/HDF5File.h"
#include "machine/Machine.h"

namespace bob { namespace machine {

/**
 * @brief This class implements a k-means classifier.
 * @details See Section 9.1 of Bishop, "Pattern recognition and machine learning", 2006
 */
class KMeansMachine: public Machine<blitz::Array<double,1>, double> {
  public: 
    /**
     * Default constructor. Builds an otherwise invalid 0 x 0 k-means 
     * machine. This is equivalent to construct a LinearMachine with two 
     * size_t parameters set to 0, as in LinearMachine(0, 0).
     */
    KMeansMachine();

    /** 
     * Constructor
     * @param[in] n_means  The number of means
     * @param[in] n_inputs The feature dimensionality
     */
    KMeansMachine(const size_t n_means, const size_t n_inputs); 

    /**
     * Builds a new machine with the given means. Each row of the means
     * matrix should represent a mean.
     */
    KMeansMachine(const blitz::Array<double,2>& means);

    /**
     * Copies another machine (copy constructor)
     */
    KMeansMachine(const KMeansMachine& other);

    /**
     * Starts a new KMeansMachine from an existing Configuration object.
     */
    KMeansMachine(bob::io::HDF5File& config);

    /**
     * Destructor
     */
    virtual ~KMeansMachine();

    /**
     * Assigns from a different machine
     */
    KMeansMachine& operator=(const KMeansMachine& other);

    /**
     * Loads data from an existing configuration object. Resets the current
     * state.
     */
    void load(bob::io::HDF5File& config);

    /**
     * Saves an existing machine to a Configuration object.
     */
    void save(bob::io::HDF5File& config) const;

    /**
     * Output the minimum distance between the input and one of the means
     * (overrides Machine::forward)
     */
    void forward(const blitz::Array<double,1>& input, double& output) const;
    
    /**
     * Output the minimum distance between the input and one of the means
     * (overrides Machine::forward_)
     * @warning Inputs are NOT checked
     */
    void forward_(const blitz::Array<double,1>& input, double& output) const;


    /**
     * Set the means
     */
    void setMeans(const blitz::Array<double,2>& means);
    
    /**
     * Set the i'th mean
     */
    void setMean(const size_t i, const blitz::Array<double,1>& mean);
    
    /**
     * Get a mean
     * @param[i]   i    The index of the mean
     * @param[out] mean The mean, a 1D array, with a length equal to the number of feature dimensions.
     */
    void getMean(const size_t i, blitz::Array<double,1>& mean) const;
    
    /**
     * Get the means
     * @param[out] means A 2D array, with as many rows as means, and as many columns as feature dimensions.
     */
    const blitz::Array<double,2>& getMeans() const
    { return m_means; }

     /**
     * Get the means in order to be updated
     * @warning Only trainers should use this function for efficiency reasons
     * @param[out] means A 2D array, with as many rows as means, and as many columns as feature dimensions.
     */
    blitz::Array<double,2>& updateMeans()
    { return m_means; }
   
    /**
     * Return the Euclidean distance of the sample, x, 
     * to the i'th mean
     * @param x The data sample (feature vector)
     * @param i The index of the mean
     */
    double getDistanceFromMean(const blitz::Array<double,1>& x, 
      const size_t i) const;
    
    /**
     * Calculate the index of the mean that is closest
     * (in terms of Euclidean distance) to the data sample, x
     * @param x The data sample (feature vector)
     * @param closest_mean (output) The index of the mean closest to the sample
     * @param distance (output) The distance of the sample from the closest mean
     */
    void getClosestMean(const blitz::Array<double,1>& x, 
      size_t &closest_mean, double &min_distance) const;
    
    /**
     * Output the minimum distance between the input and one of the means
     */
    double getMinDistance(const blitz::Array<double,1>& input) const;

    /**
     * For each mean, find the subset of the samples
     * that is closest to that mean, and calculate
     * 1) the variance of that subset (the cluster variance)
     * 2) the proportion of the samples represented by that subset (the cluster weight)
     * @param[in]  sampler   The sampler
     * @param[out] variances The cluster variances (one row per cluster), 
     *                       with as many columns as feature dimensions.
     * @param[out] weights   A vector of weights, one per cluster
     */
    void getVariancesAndWeightsForEachCluster(const bob::io::Arrayset &ar,
      blitz::Array<double,2>& variances, blitz::Array<double,1>& weights) const;
       
    /**
     * Resize the means
     */
    void resize(const size_t n_means, const size_t n_inputs);

    /**
     * Return the number of means
     */
    size_t getNMeans() const { return m_n_means; }
    
    /**
     * Return the number of inputs
     */
    size_t getNInputs() const { return m_n_inputs; }
    

  private:
     /**
     * The number of means
     */
    size_t m_n_means;
    
    /**
     * The number of inputs
     */
    size_t m_n_inputs;
    
    /**
     * The means (each row is a mean)
     */
    blitz::Array<double,2> m_means;

    /**
     * cache to avoid re-allocation
     */
    mutable blitz::Array<double,2> m_cache_means;
};

}}
#endif
