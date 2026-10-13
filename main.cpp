
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <memory>
#include <map>
#include <string>
#include <utility>

using namespace std;

// ==========================================
// 1. DATA MODELS
// ==========================================

enum class Suit {
    Hearts, Diamonds, Clubs, Spades
};

enum class HandRank {
    HighCard,
    Pair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush
};

struct Card {
    int rank; // 2-14, dengan 11=J, 12=Q, 13=K, 14=A
    Suit suit;
};

struct Hand {
    array<Card, 5> cards;
};

struct TurnInput {
    Hand hand;
    HandRank rank;
};

struct ScoreResult {
    int chips;
    int multiplier;
    int total;
};

// ==========================================
// 2. HELPER FUNCTIONS
// ==========================================

string rankName(int rank) {
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    if (rank == 14) return "A";
    return to_string(rank);
}

string suitName(Suit suit) {
    switch (suit) {
    case Suit::Hearts:   return "Hearts";
    case Suit::Diamonds: return "Diamonds";
    case Suit::Clubs:    return "Clubs";
    case Suit::Spades:   return "Spades";
    }
    return "";
}

string handRankName(HandRank rank) {
    switch (rank) {
    case HandRank::HighCard:       return "High Card";
    case HandRank::Pair:           return "Pair";
    case HandRank::TwoPair:        return "Two Pair";
    case HandRank::ThreeOfAKind:   return "Three of a Kind";
    case HandRank::Straight:       return "Straight";
    case HandRank::Flush:          return "Flush";
    case HandRank::FullHouse:      return "Full House";
    case HandRank::FourOfAKind:    return "Four of a Kind";
    case HandRank::StraightFlush:  return "Straight Flush";
    case HandRank::RoyalFlush:     return "Royal Flush";
    }
    return "Unknown";
}

void printHand(const Hand& hand) {
    for (const Card& card : hand.cards) {
        cout << "[" << rankName(card.rank)
             << " of " << suitName(card.suit) << "] ";
    }
    cout << '\n';
}

// ==========================================
// 3. POKER HAND EVALUATOR
// ==========================================

class PokerHandEvaluator {
public:
    HandRank evaluate(const Hand& hand) {
        map<int, int> counts;
        vector<int> ranks;
        bool flush = true;

        Suit firstSuit = hand.cards[0].suit;

        for (const Card& card : hand.cards) {
            counts[card.rank]++;
            ranks.push_back(card.rank);

            if (card.suit != firstSuit) {
                flush = false;
            }
        }

        sort(ranks.begin(), ranks.end());

        // Straight, termasuk A-2-3-4-5
        bool straight = true;

        for (int i = 1; i < 5; ++i) {
            if (ranks[i] != ranks[i - 1] + 1) {
                straight = false;
                break;
            }
        }

        bool wheelStraight =
            ranks == vector<int>{2, 3, 4, 5, 14};

        straight = straight || wheelStraight;

        bool royal =
            ranks == vector<int>{10, 11, 12, 13, 14};

        int pairs = 0;
        bool three = false;
        bool four = false;

        for (const auto& entry : counts) {
            if (entry.second == 2) pairs++;
            if (entry.second == 3) three = true;
            if (entry.second == 4) four = true;
        }

        // Evaluasi dari kombinasi terkuat ke terlemah
        if (flush && straight && royal)
            return HandRank::RoyalFlush;

        if (flush && straight)
            return HandRank::StraightFlush;

        if (four)
            return HandRank::FourOfAKind;

        if (three && pairs == 1)
            return HandRank::FullHouse;

        if (flush)
            return HandRank::Flush;

        if (straight)
            return HandRank::Straight;

        if (three)
            return HandRank::ThreeOfAKind;

        if (pairs == 2)
            return HandRank::TwoPair;

        if (pairs == 1)
            return HandRank::Pair;

        return HandRank::HighCard;
    }
};

// ==========================================
// 4. INTERFACES
// ==========================================

class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generateInput(int round) = 0;
};

class IScoringRule {
public:
    virtual ~IScoringRule() = default;

    virtual ScoreResult computeScore(
        const TurnInput& input,
        int bonusMultiplier
    ) = 0;
};

class IRewardRule {
public:
    virtual ~IRewardRule() = default;

    virtual int computeReward(int score, int round) = 0;
};

// ==========================================
// 5. RANDOM CARD GENERATOR
// ==========================================

class RandomCardGenerator : public IInputGenerator {
private:
    vector<Card> deck;
    size_t nextCard = 0;
    mt19937 rng{ random_device{}() };
    PokerHandEvaluator evaluator;

public:
    RandomCardGenerator() {
        // Buat deck standar 52 kartu
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 2; rank <= 14; ++rank) {
                deck.push_back({
                    rank,
                    static_cast<Suit>(suit)
                });
            }
        }

        // Shuffle satu kali di awal sesi
        shuffle(deck.begin(), deck.end(), rng);
    }

    TurnInput generateInput(int round) override {
        if (nextCard + 5 > deck.size()) {
            throw runtime_error("Not enough cards remaining!");
        }

        Hand hand;

        for (int i = 0; i < 5; ++i) {
            hand.cards[i] = deck[nextCard++];
        }

        HandRank rank = evaluator.evaluate(hand);

        return TurnInput{ hand, rank };
    }
};

// ==========================================
// 6. SCORING RULE
// ==========================================

class StandardScoringRule : public IScoringRule {
public:
    ScoreResult computeScore(
        const TurnInput& input,
        int bonusMultiplier
    ) override {
        int chips = 0;
        int multiplier = 0;

        // Contoh nilai dasar ala Balatro.
        // Nilai ini bisa kamu ubah sesuai desain game.

        switch (input.rank) {
        case HandRank::HighCard:
            chips = 5;   multiplier = 1;  break;
        case HandRank::Pair:
            chips = 10;  multiplier = 2;  break;
        case HandRank::TwoPair:
            chips = 20;  multiplier = 2;  break;
        case HandRank::ThreeOfAKind:
            chips = 30;  multiplier = 3;  break;
        case HandRank::Straight:
            chips = 30;  multiplier = 4;  break;
        case HandRank::Flush:
            chips = 35;  multiplier = 4;  break;
        case HandRank::FullHouse:
            chips = 40;  multiplier = 4;  break;
        case HandRank::FourOfAKind:
            chips = 60;  multiplier = 7;  break;
        case HandRank::StraightFlush:
            chips = 100; multiplier = 8;  break;
        case HandRank::RoyalFlush:
            chips = 100; multiplier = 8;  break;
        }

        multiplier += bonusMultiplier;

        return ScoreResult{
            chips,
            multiplier,
            chips * multiplier
        };
    }
};

// ==========================================
// 7. REWARD RULE
// ==========================================

class BonusRewardRule : public IRewardRule {
public:
    int computeReward(int score, int round) override {
        // Reward sederhana berdasarkan skor.
        int reward = score / 100;

        // Bonus akhir ronde genap
        if (round % 2 == 0) {
            reward += 2;
        }

        return reward;
    }
};

// ==========================================
// 8. SHOP SYSTEM
// ==========================================

class ShopSystem {
public:
    void executePhase(int& money, int& bonusMultiplier) {
        cout << "\n[SHOP] Bonus Multiplier +2 | Cost: $2\n";
        cout << "[SHOP] Your money: $" << money << '\n';
        cout << "Buy upgrade? (y/n): ";

        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            if (money >= 2) {
                money -= 2;
                bonusMultiplier += 2;

                cout << "[SHOP] Upgrade purchased!\n";
                cout << "[SHOP] Bonus multiplier: +"
                     << bonusMultiplier << '\n';
            }
            else {
                cout << "[SHOP] Not enough money!\n";
            }
        }
        else {
            cout << "[SHOP] Skipped.\n";
        }
    }
};

// ==========================================
// 9. CORE LOOP: RUN SESSION
// ==========================================

class RunSession {
private:
    shared_ptr<IInputGenerator> inputGen;
    shared_ptr<IScoringRule> scoreRule;
    shared_ptr<IRewardRule> rewardRule;

    ShopSystem shopSystem;

    int money = 0;
    int bonusMultiplier = 0;

    const int totalRounds = 3;

public:
    RunSession(
        shared_ptr<IInputGenerator> ig,
        shared_ptr<IScoringRule> sr,
        shared_ptr<IRewardRule> rr
    )
        : inputGen(move(ig)),
          scoreRule(move(sr)),
          rewardRule(move(rr)) {
    }

    void run() {
        cout << "================================\n";
        cout << "       POKER ROGUELIKE RUN\n";
        cout << "================================\n\n";

        for (int round = 1; round <= totalRounds; ++round) {
            cout << "========== ROUND "
                 << round << " ==========\n";

            // Phase 1: Generate random hand
            TurnInput input = inputGen->generateInput(round);

            cout << "\n[PLAY] Your cards:\n";
            printHand(input.hand);

            // Phase 2: Evaluate poker hand
            cout << "[HAND] "
                 << handRankName(input.rank) << '\n';

            // Phase 3: Calculate score
            ScoreResult result =
                scoreRule->computeScore(input, bonusMultiplier);

            cout << "[SCORE] Chips: " << result.chips
                 << " | Multiplier: " << result.multiplier
                 << " | Total: " << result.total << '\n';

            // Phase 4: Give reward
            int gain =
                rewardRule->computeReward(result.total, round);

            money += gain;

            cout << "[REWARD] Gained: $" << gain
                 << " | Money: $" << money << '\n';

            // Phase 5: Shop
            shopSystem.executePhase(money, bonusMultiplier);

            cout << "\n";
        }

        cout << "================================\n";
        cout << "             RUN END\n";
        cout << "================================\n";
        cout << "Final money: $" << money << '\n';
        cout << "Final bonus multiplier: +"
             << bonusMultiplier << '\n';
    }
};

// ==========================================
// 10. MAIN: COMPOSITION ROOT
// ==========================================

int main() {
    auto inputGenerator =
        make_shared<RandomCardGenerator>();

    auto scoringRule =
        make_shared<StandardScoringRule>();

    auto rewardRule =
        make_shared<BonusRewardRule>();

    RunSession session(
        inputGenerator,
        scoringRule,
        rewardRule
    );

    session.run();

    return 0;
}
