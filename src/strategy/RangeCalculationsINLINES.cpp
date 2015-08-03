#include "util/select.hpp"

namespace rangecalc {
  void init_uniform(PreflopRange& range) {
    int hi = 0;
    for (int ci=0; ci<52; ++ci) {
      for (int cj=ci+1; cj<52; ++cj) {
        ps::CardSet cset;
        cset.insert(ps::Card(ci));
        cset.insert(ps::Card(cj));
        range.setHolding(hi, Holding(cset));
        range.getValue(hi).weight[0] = 1.0;
        range.getValue(hi).weight[1] = 1.0;
        ++hi;
      }
    }
  }
  
  void computeEquities_naive(TurnComputationMap& map, const TurnEvalMap& turn_evals,
      const Board& board)
  {
    for (int p=0; p<2; ++p) {
      for (size_t i=0; i<TurnRange::sSize; ++i) {
        Holding holding_i = map.getHolding(i);
        ComputationUnit& unit_i = map.getValue(i);
        
        float numerator = 0.0;
        float denominator = 0.0;
        for (int r=0; r<52; ++r) {
          ps::Card river = ps::Card(r);
          if ((board.getCards()|holding_i.getCardSet()).contains(river)) continue;
          ps::PokerEvaluation eval_i = turn_evals.getValue(i).evals[r];

          for (size_t j=0; j<TurnRange::sSize; ++j) {
            Holding holding_j = map.getHolding(j);
            const ComputationUnit& unit_j = map.getValue(j);
            ps::PokerEvaluation eval_j = turn_evals.getValue(j).evals[r];
            bool intersects = holding_j.intersects(holding_i.getCardSet() | board.getCards()) ||
              holding_j.contains(river);
            
            float weight = unit_j.weight[1-p];

            numerator += (intersects?0:weight) * 
              branchless::select(eval_i>eval_j, 1.0, branchless::select(eval_i==eval_j, 0.5, 0.0));
            
            denominator += (intersects?0:weight);
          }
        }
        unit_i.equity[p] = numerator / denominator;
      }
    }
  }

  void computeTurnEquityMatrix(TurnEquityMatrix& matrix, const Board& board, const TurnEvalMap& evals)
  {
    /*
     * TODO(dshin) matrix.I is symmetric along diagonal, matrix.M is pseudo-symmetric along diagonal.
     *
     * So just compute half of the matrix, and then do a pseudo-reflection across the diagonal.
     */
    for (size_t i=0; i<TurnRange::sSize; ++i) {
      Holding holding_i = evals.getHolding(i);
      for (size_t j=0; j<TurnRange::sSize; ++j) {
        Holding holding_j = evals.getHolding(j);
        bool intersects = holding_j.intersects(holding_i);
        if (intersects) {
          matrix.I(i,j) = 0.0;
          matrix.M(i,j) = 0.0;
          continue;
        }

        int counts[3] = {};  // loss/tie/win
        for (int r=0; r<52; ++r) {
          ps::Card river = ps::Card(r);
          ps::CardSet used_cards = holding_i.getCardSet() | holding_j.getCardSet() | board.getCards();
          if (used_cards.contains(river)) continue;
          
          ps::PokerEvaluation eval_i = evals.getValue(i).evals[r];
          ps::PokerEvaluation eval_j = evals.getValue(j).evals[r];

          counts[cmp(eval_i, eval_j)] += 1;
        }
        
        matrix.M(i,j) = ((float)counts[1]+2*counts[2])/(2*(counts[0]+counts[1]+counts[2]));
        matrix.I(i,j) = 1.0;
        
        /*
        fprintf(stdout, "%s %s L:%d T:%d W:%d\n", holding_i.str().c_str(), holding_j.str().c_str(),
            counts[0], counts[1], counts[2]);
            */
      }
    }
  }
  
  void computeEquities_naive(RiverComputationMap& map) {
    for (int p=0; p<2; ++p) {
      for (size_t i=0; i<RiverRange::sSize; ++i) {
        Holding holding_i = map.getHolding(i);
        RiverComputationUnit& unit_i = map.getValue(i);
        ps::PokerEvaluation eval_i = unit_i.eval;

        float numerator = 0.0;
        float denominator = 0.0;
        for (size_t j=0; j<RiverRange::sSize; ++j) {
          Holding holding_j = map.getHolding(j);
          const RiverComputationUnit& unit_j = map.getValue(j);
          ps::PokerEvaluation eval_j = unit_j.eval;
          bool intersects = holding_i.intersects(holding_j);
          
          float weight = unit_j.weight[1-p];

          numerator += (intersects?0:weight) * 
            branchless::select(eval_i>eval_j, 1.0, branchless::select(eval_i==eval_j, 0.5, 0.0));
          
          denominator += (intersects?0:weight);
        }

        unit_i.equity[p] = numerator / denominator;
      }
    }
  }

  void computeEquities_smart(RiverComputationMap& map)
  {
    map.sort(RankCompare);

    // need per-card weights to do inclusion-exclusion combinatorics
    float per_card_weights[52][2] = {};
    float cumulative_weight[2] = {};

    size_t N = RiverRange::sSize;

    size_t i=0;
    while (i<N) {
      RiverComputationUnit& unit_i = map.getValue(i);
      ps::PokerEvaluation eval_i = unit_i.eval;
      
      // need per-card weights to do inclusion-exclusion combinatorics
      float per_card_subweights[52][2] = {};
      float cumulative_subweight[2] = {};
     
      // find tying hands, need to find them separately because of 0.5 multiplier
      size_t j=i;
      for (; j<N && map.getValue(j).eval==eval_i; ++j) {
        const RiverComputationUnit& unit_j = map.getValue(j);
        Holding holding_j = map.getHolding(j);
        int code1 = holding_j.getCard1().code();
        int code2 = holding_j.getCard2().code();
        for (int p=0; p<2; ++p) {
          float weight_j = unit_j.weight[p];
          cumulative_subweight[p] += weight_j;
          per_card_subweights[code1][p] += weight_j;
          per_card_subweights[code2][p] += weight_j;
        }
      }

      for (size_t k=i; k<j; ++k) {
        RiverComputationUnit& unit_k = map.getValue(k);
        Holding holding_k = map.getHolding(k);
        int code1 = holding_k.getCard1().code();
        int code2 = holding_k.getCard2().code();
        for (int p=0; p<2; ++p) {
          float weight_k = unit_k.weight[p];

          // inclusion-exclusion combinatorics:
          float win_weight = cumulative_weight[p] - per_card_weights[code1][p] -
            per_card_weights[code2][p];
          float tie_weight = weight_k + cumulative_subweight[p] - per_card_subweights[code1][p] -
            per_card_subweights[code2][p];

          // this is just numerator, will divide by denominator later
          unit_k.equity[1-p] = win_weight + 0.5*tie_weight;
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
      RiverComputationUnit& unit = map.getValue(i);
      Holding holding = map.getHolding(i);
      int code1 = holding.getCard1().code();
      int code2 = holding.getCard2().code();
      for (int p=0; p<2; ++p) {
        float weight = unit.weight[1-p];
        float numerator = unit.equity[p];
        
        // inclusion-exclusion combinatorics:
        float denominator = weight + cumulative_weight[1-p] - per_card_weights[code1][1-p] -
          per_card_weights[code2][1-p];

        unit.equity[p] = numerator / denominator;
      }
    }
  }

  /*
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
  */
}

