#include <iostream>
#include <memory>
#include <random>

// ==========================================
// 1. DATA MODELS & INTERFACES (Abstractions)
// ==========================================

struct TurnInput {
    int value;
};

// Interface untuk Input Generator
class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generateInput(int round) = 0;
};

// Interface untuk Scoring Rule
class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeScore(const TurnInput& input) = 0;
};

// Interface untuk Reward Rule
class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int computeReward(int baseScore, int round) = 0;
};

// ==========================================
// 2. SHOP SYSTEM
// ==========================================

class ShopSystem {
public:
    void executePhase(int& currentMoney) {
        // Logika sederhana/hardcoded sesuai spesifikasi
        std::cout << "[SHOP] offered: Bonus(+2) cost 2\n";
        std::cout << "[SHOP] skipped\n";
    }
};

// ==========================================
// 3. INVARIANT CORE LOOP: RunSession
// ==========================================
// RunSession HANYA mengatur alur eksekusi (phase order).
// Tidak memuat logika kalkulasi skor, reward, maupun input.
class RunSession {
private:
    std::shared_ptr<IInputGenerator> inputGen;
    std::shared_ptr<IScoringRule> scoreRule;
    std::shared_ptr<IRewardRule> rewardRule;
    ShopSystem shopSystem;
    int money{ 0 };
    const int totalRounds{ 3 };

public:
    RunSession(std::shared_ptr<IInputGenerator> ig,
        std::shared_ptr<IScoringRule> sr,
        std::shared_ptr<IRewardRule> rr)
        : inputGen(std::move(ig)), scoreRule(std::move(sr)), rewardRule(std::move(rr)) {
    }

    void run() {
        std::cout << "=== RUN START ===\n\n";

        for (int round = 1; round <= totalRounds; ++round) {
            std::cout << "Round " << round << "\n";

            // Phase 1: Generate input
            TurnInput input = inputGen->generateInput(round);
            std::cout << "[PLAY] input generated: " << input.value << "\n";

            // Phase 2: Compute base score
            int baseScore = scoreRule->computeScore(input);
            std::cout << "[SCORE] base score: " << baseScore << "\n";

            // Phase 3: Compute reward
            int gain = rewardRule->computeReward(baseScore, round);

            // Phase 4: Update money
            money += gain;
            std::cout << "[REWARD] gain: " << gain << " | money: " << money << "\n";

            // Phase 5: Shop phase
            shopSystem.executePhase(money);

            // Phase 6: Advance round
            std::cout << "\n";
        }

        std::cout << "=== RUN END ===\n";
        std::cout << "Final money: " << money << "\n";
    }
};

// ==========================================
// 4. CONCRETE IMPLEMENTATIONS (Mutable Parts)
// ==========================================

// Base Generator (sebelum modifikasi)
class FixedInputGenerator : public IInputGenerator {
public:
    TurnInput generateInput(int round) override {
        return TurnInput{ 3 }; // Nilai tetap 3
    }
};

// Modification 1: Random Input Generator
class RandomInputGenerator : public IInputGenerator {
private:
    std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist{ 1, 10 };

public:
    TurnInput generateInput(int round) override {
        return TurnInput{ dist(rng) };
    }
};

// Base Scoring Rule: Skor sama dengan nilai input
class StandardScoringRule : public IScoringRule {
public:
    int computeScore(const TurnInput& input) override {
        return input.value;
    }
};

// Modification 2: Reward Rule Baru
// Reward bukan lagi sama persis dengan baseScore
class BonusRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore, int round) override {
        // Contoh: reward = baseScore + 2, dan dikali 2 jika round genap
        if (round % 2 == 0) {
            return (baseScore + 2) * 2;
        }
        return baseScore + 2;
    }
};

// ==========================================
// 5. MAIN (Composition Root)
// ==========================================
int main() {
    // Kita merakit komponen (Dependency Injection) tanpa menyentuh RunSession
    auto inputGenerator = std::make_shared<RandomInputGenerator>(); // Modification 1
    auto scoringRule = std::make_shared<StandardScoringRule>();
    auto rewardRule = std::make_shared<BonusRewardRule>();        // Modification 2

    // Jalankan sesi permainan
    RunSession session(inputGenerator, scoringRule, rewardRule);
    session.run();

    return 0;
}