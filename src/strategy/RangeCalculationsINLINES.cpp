
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
          bool intersects = holding_i.intersects(holding_j);
          
          float weight = holding_j.getWeight(1-p);

          numerator += (intersects?0:1) * weight * 
            branchless::select(eval_i>eval_j, 1, branchless::select(eval_i==eval_j, 0.5, 0));
          
          denominator += (intersects?0:1) * weight;
        }

        holding_i.setEquity(p, numerator / denominator);
      }
    }
  }

  void computeEquities_smart(RankedJointHoldingDistribution& dist) {
    dist.sort(RankCompare);
    // TODO
  }
}

