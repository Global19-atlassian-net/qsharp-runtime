// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <cassert>
#include <complex>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "types.hpp"
#include "gates.hpp"

#include "external/fused.hpp"

namespace Microsoft
{
namespace Quantum
{
namespace SIMULATOR
{
    namespace detail
    {
        inline std::size_t get_register(const std::vector<unsigned>& qs, std::size_t basis_state)
        {
            std::size_t result = 0;
            for (unsigned i = 0; i < qs.size(); ++i)
                result |= ((basis_state >> qs[i]) & 1) << i;
            return result;

        }

        inline std::size_t set_register(const std::vector<unsigned>& qs, std::size_t qmask, std::size_t basis_state, std::size_t original = 0ull)
        {
            std::size_t result = original & ~qmask;
            for (unsigned i = 0; i < qs.size(); ++i)
                result |= ((basis_state >> i) & 1) << qs[i];
            return result;
        }
    }

    class GateWrapper {
    public:
        GateWrapper(std::vector<unsigned> controls, unsigned target, TinyMatrix<ComplexType, 2> mat) : controls_(controls), target_(target), mat_(mat) {}
        std::vector<unsigned> get_controls() { return controls_; }
        unsigned get_target() { return target_; }
        TinyMatrix<ComplexType, 2> get_mat() { return mat_; }
    private:
        std::vector<unsigned> controls_;
        unsigned target_;
        TinyMatrix<ComplexType, 2> mat_;
    };


    class Cluster {
    public:
        Cluster(std::vector<unsigned> qids, std::vector<GateWrapper> gates) : qids_(qids), gates_(gates) {}
        std::vector<unsigned> get_qids() { return qids_; }
        std::vector<GateWrapper> get_gates() { return gates_; }
        void setQids(std::vector<unsigned> qids) {
            qids_ = qids;
        }
        void append_gates(std::vector<GateWrapper> gates) {
            gates_.insert(gates_.end(), gates.begin(), gates.end());
        }
        std::pair<Cluster, std::vector<unsigned>> next_cluster(std::vector<Cluster>& prevClusters, unsigned maxWidth, unsigned maxDepth) {
            std::set<unsigned> tempCurQids(qids_.begin(), qids_.end());
            for (int i = 0; i < prevClusters.size(); i++) {
                if (i > maxDepth) {
                    break;
                }
                auto prevQids = prevClusters[i].get_qids();
                std::sort(prevQids.begin(), prevQids.end());
                std::set<unsigned> tempTotQids(qids_.begin(), qids_.end());
                tempTotQids.insert(prevQids.begin(), prevQids.end());
                std::vector<unsigned> totQids(tempTotQids.begin(), tempTotQids.end());
                if (totQids.size() <= maxWidth) {
                    auto cl = prevClusters[i];
                    prevClusters.erase(prevClusters.begin() + i);
                    return std::make_pair(cl, totQids);
                }

                std::vector<int> qidsIntersection;
                if (i == prevClusters.size() - 1) {
                    std::set_intersection(prevQids.begin(), prevQids.end(),
                        tempCurQids.begin(), tempCurQids.end(),
                        std::back_inserter(qidsIntersection));
                }
                else {
                    std::vector<unsigned> lookAhead = prevClusters[i + 1].get_qids();
                    std::set<int> lookAheadQids(lookAhead.begin(), lookAhead.end());
                    if (i == 0) {
                        std::set_intersection(prevQids.begin(), prevQids.end(),
                            tempCurQids.begin(), tempCurQids.end(),
                            std::back_inserter(qidsIntersection));
                    }
                    std::set_intersection(lookAheadQids.begin(), lookAheadQids.end(),
                        tempCurQids.begin(), tempCurQids.end(),
                        std::back_inserter(qidsIntersection));
                }
                tempCurQids.insert(prevQids.begin(), prevQids.end());
                if (qidsIntersection.size() > 0) {
                    break;
                }
            }
            Cluster defCl = Cluster({}, {});
            std::vector<unsigned> defVec = {};
            return std::make_pair(defCl, defVec);
        }
    private:
        std::vector<unsigned> qids_;
        std::vector<GateWrapper> gates_;
    };

/// A wave function class to store and manipulate the state of qubits

template <class T = ComplexType>
class Wavefunction
{
public:
    using value_type = T;
    using qubit_t = unsigned;
    using RngEngine = std::mt19937;

    constexpr qubit_t invalid_qubit() const { return std::numeric_limits<qubit_t>::max(); }

    /// allocate a wave function for zero qubits
    Wavefunction(unsigned /*ignore*/) : num_qubits_(0), wfn_(1, 1.), usage_(0)
    {
        rng_.seed(std::clock());
    }

    void reset()
    {
        fused_.reset();
        rng_.seed(std::clock());
        num_qubits_ = 0;
        wfn_.resize(1);
        wfn_[0] = 1.;
        qubitmap_.resize(0);
    }

    ~Wavefunction()
    {
        flush();
    }

    unsigned qubit(unsigned q) const
    {
        assert(qubitmap_[q] != invalid_qubit());
        return qubitmap_[q];
    }

    unsigned qubit(Gates::OneQubitGate const& g) const
    {
        return qubit(g.qubit());
    }

    void flush() const
    {
        unsigned fusedSpan = dbgFusedSpan;
        auto clusters = make_clusters(fusedSpan, gatelist_);
        gatelist_.clear();
        if (clusters.size() == 0) {
            fused_.flush(wfn_);
        }
        else {
            for (int i = 0; i < clusters.size(); i++) {
                Cluster cl = clusters.at(i);
                for (GateWrapper gate : cl.get_gates()) {
                    std::vector<unsigned> cs = gate.get_controls();
                    if (cs.size() == 0) {
                        fused_.apply(wfn_, gate.get_mat(), gate.get_target());
                    }
                    else {
                        fused_.apply_controlled(wfn_, gate.get_mat(), cs, gate.get_target());
                    }
                }
                fused_.flush(wfn_);
            }

        }
    }

    unsigned findNextPos(unsigned startIdx, std::unordered_set<qubit_t> setForSearch) const
    {
        while (setForSearch.find(startIdx) != setForSearch.end())
        {
            startIdx++;
        }
        return startIdx;
    }
    /// allocate a qubit and grow the wave function
    unsigned allocate()
    {
        assert(usage_ != 2);
        usage_ = 1;
        flush();
        wfn_.resize(2 * wfn_.size());
        auto it = std::find(qubitmap_.begin(), qubitmap_.end(), invalid_qubit());
        if (it != qubitmap_.end())
        {
            unsigned num = static_cast<unsigned>(it - qubitmap_.begin());
            qubitmap_[num] = num_qubits_++;
            return num;
        }
        else {
            qubitmap_.push_back(num_qubits_++);
            return static_cast<unsigned>(qubitmap_.size() - 1);
        }
    }

    /// allocate a qubit and grow the wave function
    void allocateQubit(unsigned id)
    {
        assert(usage_ != 1);
        usage_ = 2;
        flush();
        wfn_.resize(2 * wfn_.size());
        if (id < qubitmap_.size()) {
            qubitmap_[id] = num_qubits_++;
        }
        else {
            assert(id == qubitmap_.size());
            qubitmap_.push_back(num_qubits_++);
        }
        assert((wfn_.size() >> num_qubits_) == 1);
    }

    /// release the specified qubit
    /// \pre the qubit has to be in a classical state in the computational basis
    void release(qubit_t q)
    {
        unsigned p = qubit(q); //returns qubitmap_[q]
        flush();
        kernels::collapse(wfn_, p, getvalue(q), true);
        for (int i = 0; i < qubitmap_.size(); ++i)
            if (qubitmap_[i] > p && qubitmap_[i] != invalid_qubit())
                qubitmap_[i]--;
        qubitmap_[q] = invalid_qubit();
        --num_qubits_;
    }

    /// the number of used qubits
    qubit_t num_qubits() const
    {
        return num_qubits_;
    }

    /// probability of measuring a 1
    double probability(qubit_t q) const
    {
        flush();
        return kernels::probability(wfn_, qubit(q));
    }

    /// probability of jointly measuring a 1
    double jointprobability(std::vector<qubit_t> const& qs) const
    {
        flush();
        std::vector<qubit_t> ps = qubits(qs);
        return kernels::jointprobability(wfn_, ps);
    }

    /// probability of jointly measuring a 1
    double jointprobability(std::vector<Gates::Basis> const& bs, std::vector<qubit_t> const& qs) const
    {
        flush();
        std::vector<qubit_t> ps = qubits(qs);
        return kernels::jointprobability(wfn_, bs, ps);
    }

    /// measure a qubit
    bool measure(qubit_t q)
    {
        flush();
        std::uniform_real_distribution<double> uniform(0., 1.);
        bool result = (uniform(rng_) < probability(q));
        kernels::collapse(wfn_, qubit(q), result);
        kernels::normalize(wfn_);
        return result;
    }

    bool jointmeasure(std::vector<qubit_t> const& qs)
    {
        flush();
        std::vector<qubit_t> ps = qubits(qs);
        std::uniform_real_distribution<double> uniform(0., 1.);
        bool result = (uniform(rng_) < jointprobability(qs));
        kernels::jointcollapse(wfn_, ps, result);
        kernels::normalize(wfn_);
        return result;
    }

    void apply_controlled_exp(std::vector<Gates::Basis> const& bs,
        double phi,
        std::vector<unsigned> const& cs,
        std::vector<unsigned> const& qs)
    {
        flush();
        kernels::apply_controlled_exp(wfn_, bs, phi, qubits(cs), qubits(qs));
    }

    /// checks if the qubit is in classical state
    bool isclassical(qubit_t q) const
    {
        flush();
        return kernels::isclassical(wfn_, qubit(q));
    }

    /// returns the classical value of a qubit (if classical)
    /// \pre the qubit has to be in a classical state in the computational basis
    bool getvalue(qubit_t q) const
    {
        flush();
        assert(isclassical(q));
        int res = kernels::getvalue(wfn_, qubit(q));
        if (res == 2)
            std::cout << *this;

        assert(res < 2);
        return res == 1;
    }

    /// the stored wave function as a vector
    WavefunctionStorage const& data() const
    {
        flush();
        return wfn_;
    }

    /// seed the random number engine for measurements
    void seed(unsigned s)
    {
        rng_.seed(s);
    }

    std::vector<Cluster> make_clusters(unsigned fuseSpan, std::vector<GateWrapper> gates) const {
        std::vector<Cluster> curClusters;

        if (gates.size() > 0) {
            unsigned maxDepth = 999;
            for (int i = 0; i < gates.size(); i++) {
                std::vector<unsigned> qids;
                std::vector<unsigned> controlQids = gates[i].get_controls();
                if (controlQids.size() > 0) {
                    qids = controlQids;
                }
                qids.push_back(gates[i].get_target());
                Cluster newCl = Cluster(qids, { gates[i] });
                curClusters.push_back(newCl);
            }
            for (int i = 1; i < fuseSpan + 1; i++) {
                auto prevClusters = curClusters;
                curClusters.clear();
                auto prevCluster = prevClusters[0];
                prevClusters.erase(prevClusters.begin());
                while (prevClusters.size() > 0) {
                    auto foundCompat = prevCluster.next_cluster(prevClusters, i, maxDepth);
                    Cluster clusterFound = foundCompat.first;
                    std::vector<unsigned> foundTotQids = foundCompat.second;
                    if (clusterFound.get_gates().size() == 0) {
                        curClusters.push_back(prevCluster);
                        prevCluster = prevClusters[0];
                        prevClusters.erase(prevClusters.begin());
                    }
                    else {
                        prevCluster.setQids(foundTotQids);
                        prevCluster.append_gates(clusterFound.get_gates());
                    }
                }
                curClusters.push_back(prevCluster);
            }
        }
        return curClusters;
    }

    void reorder_wavefunction(std::vector<unsigned> currLocs, std::vector<unsigned> newLocs) const
    {
        // swap qubits in wfn between qubitLoc and newPos

        for (std::size_t i = 0ull; i < wfn_.size(); i++)
        {
            std::size_t orig_i = i;
            std::size_t new_i = 0;
            for (unsigned j = 0; j < currLocs.size(); j++)
            {
                unsigned qubitLoc = currLocs[j];
                unsigned newPos = newLocs[j];
                if (newPos != qubitLoc)
                {
                    std::size_t bit1 = (i >> qubitLoc) & 1ull;
                    std::size_t bit2 = (i >> newPos) & 1ull;
                    std::size_t x = (bit1 ^ bit2);
                    x = (x << qubitLoc) | (x << newPos);
                    new_i = i ^ x;
                    i = new_i;
                }
            }
            i = orig_i;
            if (new_i > i)
            {
                std::iter_swap(wfn_.begin() + i, wfn_.begin() + new_i);
            }
            
        }

        for (unsigned j = 0; j < currLocs.size(); j++)
        {
            unsigned qubitLoc = currLocs[j];
            unsigned newPos = newLocs[j];
            // get id of qubit located at newPos and qubitLoc - getting index from the element
            auto newQubitLocItr = std::find(qubitmap_.begin(), qubitmap_.end(), newPos);
            assert(newQubitLocItr != qubitmap_.end());
            auto origQubitLocItr = std::find(qubitmap_.begin(), qubitmap_.end(), qubitLoc);
            assert(origQubitLocItr != qubitmap_.end());
            // swap elements in qubitmap located at iterators
            std::iter_swap(origQubitLocItr, newQubitLocItr);
        }
}
    /// generic application of a gate
    template <class Gate>
    void apply(Gate const& g)
    {
        //check flush condition
        std::vector<qubit_t> cs;
        GateWrapper gateApplied = GateWrapper(cs, qubit(g), g.matrix());
        gatelist_.push_back(gateApplied);
        if (gatelist_.size() > 50) {
            flush();
        }
        /*if (fused_.shouldFlush(wfn_, g.matrix(), cs, g.qubit())) {
            flush();
        }
        fused_.apply(wfn_, g.matrix(), g.qubit());*/
    }
    
    /// generic application of a multiply controlled gate
    template <class Gate>
    void apply_controlled(std::vector<qubit_t> cs, Gate const& g)
    {
        std::vector<qubit_t> pcs = qubits(cs);
        GateWrapper gateApplied = GateWrapper(pcs, qubit(g), g.matrix());
        gatelist_.push_back(gateApplied);
        if (gatelist_.size() > 50) {
            flush();
        }
        //check flush condition
        /*if (fused_.shouldFlush(wfn_, g.matrix(), cs, g.qubit())) {
            flush();
        }
        fused_.apply_controlled(wfn_, g.matrix(), cs, g.qubit());*/
    }

    /// generic application of a controlled gate
    template <class Gate>
    void apply_controlled(qubit_t c, Gate const& g)
    {
        std::vector<qubit_t> cs(1, c);
        apply_controlled(cs, g);
    }

    /// unoptimized application of a doubly controlled gate
    template <class Gate>
    void apply_controlled(qubit_t c1, qubit_t c2, Gate const& g)
    {
        std::vector<qubit_t> cs;
        cs.push_back(c1);
        cs.push_back(c2);
        apply_controlled(cs, g);
    }

    template <class A>
    bool subsytemwavefunction(std::vector<unsigned> const& qs, std::vector<T, A>& qubitswfn, double tolerance)
    {
        flush(); // we have to flush before we can extract the state
        return kernels::subsytemwavefunction(wfn_, qubits(qs), qubitswfn, tolerance);
    }


    // apply permutation of basis states to the wave function
    void permute_basis(std::vector<unsigned> const& qs, std::size_t table_size,
        std::size_t const* permutation_table, bool adjoint = false)
    {
        assert(table_size == 1ull << qs.size());
        flush();
        auto real_qs = qubits(qs);
        auto num_states = wfn_.size();
        auto psi_new = WavefunctionStorage(num_states);
        auto qmask = kernels::make_mask(real_qs);

        auto permute = [&real_qs, qmask, table_size, permutation_table](std::size_t state) {
            auto qstate = detail::get_register(real_qs, state);
            assert(qstate < table_size);
            return detail::set_register(real_qs, qmask, permutation_table[qstate], state);
        };

        if (!adjoint)
        {
            for (size_t i = 0; i < num_states; ++i)
                psi_new[permute(i)] = wfn_[i];
        }
        else
        {
            for (size_t i = 0; i < num_states; ++i)
                psi_new[i] = wfn_[permute(i)];
        }

        std::swap(wfn_, psi_new);
    }

    RngEngine& rng()
    {
        return rng_;
    }


    std::vector<qubit_t> qubits(std::vector<qubit_t> const& qs) const
    {
        std::vector<qubit_t> ps;
        for (auto q : qs)
            ps.push_back(qubit(q));
        return ps;
    }

    // Returns the list of logical ids currently allocated.
    std::vector<qubit_t> logicalQubits() const
    {
        std::vector<qubit_t> qs;
        for (qubit_t i = 0; i < qubitmap_.size(); i++)
        {
            if (qubitmap_[i] != invalid_qubit()) qs.push_back(i);
        }
    
        return qs;
    }

  private:
    unsigned num_qubits_;             // for convenience
    mutable WavefunctionStorage wfn_; // storing the wave function
    mutable std::vector<qubit_t> qubitmap_;   // mapping of logical to physical qubits
	int usage_;
    mutable std::vector<GateWrapper> gatelist_;

    // randomness support
    RngEngine rng_;
    Fused fused_;
};


/// print information about the wave function
template <class T>
std::ostream& operator<<(std::ostream& out, Wavefunction<T> const& wfn)
{
    wfn.flush();
    out << "Wave function for " << wfn.num_qubits() << " with " << wfn.data().size() << " elements "
        << " using " << sizeof(T) * wfn.data().size() << " bytes" << std::endl;
    if (wfn.num_qubits() <= 6)
        std::copy(wfn.data().begin(), wfn.data().end(), std::ostream_iterator<T>(out, "\n"));
    return out;
}
}
}
}
