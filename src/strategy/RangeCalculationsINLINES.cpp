#include "util/select.hpp"

void RankedJointWeightedHolding::init(Holding holding, float weight1, float weight2) {
  _holding = holding;
  _weight[0] = weight1;
  _weight[1] = weight2;
}

Holding RankedJointWeightedHolding::getHolding() const { return _holding; }
void RankedJointWeightedHolding::setHolding(Holding holding) { _holding = holding; }

float RankedJointWeightedHolding::getWeight(int index) const { return _weight[index]; }
void RankedJointWeightedHolding::setWeight(int index, float weight) { _weight[index] = weight; }

float RankedJointWeightedHolding::getEquity(int index) const { return _equity[index]; }
void RankedJointWeightedHolding::setEquity(int index, float equity) { _equity[index] = equity; }

ps::PokerEvaluation RankedJointWeightedHolding::getEval() const { return _eval; }
void RankedJointWeightedHolding::setEval(ps::PokerEvaluation eval) { _eval = eval; }

namespace rangecalc {
  void computeEquities_naive(RankedJointHoldingDistribution& dist) {
    uint32_t N = dist.size();
    for (int p=0; p<2; ++p) {
      for (uint32_t i=0; i<N; ++i) {
        RankedJointWeightedHolding& holding_i = dist[i];
        ps::PokerEvaluation eval_i = holding_i.getEval();

        float numerator = 0.0;
        float denominator = 0.0;
        for (uint32_t j=0; j<N; ++j) {
          const RankedJointWeightedHolding& holding_j = dist[j];
          ps::PokerEvaluation eval_j = holding_j.getEval();
          bool intersects = holding_i.getHolding().intersects(holding_j.getHolding());
          
          float weight = holding_j.getWeight(1-p);

          numerator += (intersects?0:1) * weight * 
            branchless::select(eval_i>eval_j, 1.0, branchless::select(eval_i==eval_j, 0.5, 0.0));
          
          denominator += (intersects?0:1) * weight;
        }

        holding_i.setEquity(p, numerator / denominator);
      }
    }
  }

  void computeEquities_smart(RankedJointHoldingDistribution& dist) {
    dist.sort(RankCompare);

    uint32_t N = dist.size();

    // need per-card weights to do inclusion-exclusion combinatorics
    float per_card_weights[52][2] = {};
    float cumulative_weight[2] = {};

    uint32_t i=0;
    while (i<N) {
      const RankedJointWeightedHolding& unit_i = dist[i];
      ps::PokerEvaluation eval_i = unit_i.getEval();
      
      // need per-card weights to do inclusion-exclusion combinatorics
      float per_card_subweights[52][2] = {};
      float cumulative_subweight[2] = {};
     
      // find tying hands, need to find them separately because of 0.5 multiplier
      uint32_t j=i;
      for (; j<N && dist[j].getEval()==eval_i; ++j) {
        const RankedJointWeightedHolding& unit_j = dist[j];
        Holding holding_j = unit_j.getHolding();
        int code1 = holding_j.getCard1().code();
        int code2 = holding_j.getCard2().code();
        for (int p=0; p<2; ++p) {
          float weight_j = unit_j.getWeight(p);
          cumulative_subweight[p] += weight_j;
          per_card_subweights[code1][p] += weight_j;
          per_card_subweights[code2][p] += weight_j;
        }
      }

      for (uint32_t k=i; k<j; ++k) {
        RankedJointWeightedHolding& unit_k = dist[k];
        Holding holding_k = unit_k.getHolding();
        int code1 = holding_k.getCard1().code();
        int code2 = holding_k.getCard2().code();
        for (int p=0; p<2; ++p) {
          float weight_k = unit_k.getWeight(p);

          // inclusion-exclusion combinatorics:
          float win_weight = cumulative_weight[p] - per_card_weights[code1][p] -
            per_card_weights[code2][p];
          float tie_weight = weight_k + cumulative_subweight[p] - per_card_subweights[code1][p] -
            per_card_subweights[code2][p];

          // this is just numerator, will divide by denominator later
          unit_k.setEquity(1-p, win_weight + 0.5*tie_weight);
        }
      }

      i = j;
      
      cumulative_weight[0] += cumulative_subweight[0];
      cumulative_weight[1] += cumulative_subweight[1];
      // auto-vectorization ftw?
      for (int k=0; k<52; ++k) {
        per_card_weights[k][0] += per_card_subweights[k][0];
        per_card_weights[k][1] += per_card_subweights[k][1];
      }
    }

    for (i=0; i<N; ++i) {
      RankedJointWeightedHolding& unit = dist[i];
      Holding holding = unit.getHolding();
      int code1 = holding.getCard1().code();
      int code2 = holding.getCard2().code();
      for (int p=0; p<2; ++p) {
        float weight = unit.getWeight(p);
        float numerator = unit.getEquity(p);
        
        // inclusion-exclusion combinatorics:
        float denominator = weight + cumulative_weight[1-p] - per_card_weights[code1][1-p] -
          per_card_weights[code2][1-p];

        unit.setEquity(p, numerator / denominator);
      }
    }
  }

  void computeRiverEquityMatrix(RiverEquityMatrix& M, const HoldingIndexing& indexing,
      const HoldingMap<ps::PokerEvaluation>& evals)
  {
    int N = indexing.size();

    HoldingMap<Holding> holdings(N);
    for (int i=0; i<N; ++i) holdings[i] = indexing[i].toHolding();

    for (int i=0; i<N; ++i) {
      Holding holding_i = holdings[i];
      int j=0;
      for (int j64=0; j<N; ++j64) {
        uint64_t win_bits = 0LL;
        uint64_t tie_bits = 0LL;
        uint64_t denom_bits = 0LL;
        for (int jd=0; j<N && jd<64; ++jd) {
          Holding holding_j = holdings[j];
          bool intersects = holding_i.intersects(holding_j);
          bool wins = !intersects && (evals[i]>evals[j]);
          bool ties = !intersects && (evals[i]==evals[j]);

          win_bits |= (wins ? 1LL : 0LL) << jd;
          tie_bits |= (ties ? 1LL : 0LL) << jd;
          denom_bits |= ((!intersects) ? 1LL : 0LL) << jd;

          ++j;
        }

        M.set_win_block(i, j64, win_bits);
        M.set_tie_block(i, j64, tie_bits);
        M.set_denom_block(i, j64, denom_bits);
      }
    }
  }
}

