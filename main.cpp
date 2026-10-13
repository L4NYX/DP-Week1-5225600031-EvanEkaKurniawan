#include <iostream>
using namespace std;


struct Hand {
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
    RoyalFlush,
    FiveOfAKind,
    FlushHouse,
    FlushFive
};


class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker = nullptr;

public:
    virtual ~PokerHandChecker() = default;
    virtual HandRank check(const Hand& hand) = 0;

    void setNext(PokerHandChecker* next) {
        nextChecker = next;
    }
};

class HighCardChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "High Card" << endl;
        return HandRank::HighCard;
    }
};

class PairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Pair" << endl;
        return HandRank::Pair;
    }
};

class TwoPairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Two Pair" << endl;
        return HandRank::TwoPair;
    }
};

class ThreeOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Three of a Kind" << endl;
        return HandRank::ThreeOfAKind;
    }
};

class StraightChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Straight" << endl;
        return HandRank::Straight;
    }
};

class FlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Flush" << endl;
        return HandRank::Flush;
    }
};

class FullHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Full House" << endl;
        return HandRank::FullHouse;
    }
};

class FourOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Four of a Kind" << endl;
        return HandRank::FourOfAKind;
    }
};

class StraightFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Straight Flush" << endl;
        return HandRank::StraightFlush;
    }
};

class RoyalFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Royal Flush" << endl;
        return HandRank::RoyalFlush;
    }
};

class FiveOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Five of a Kind" << endl;
        return HandRank::FiveOfAKind;
    }
};

class FlushHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Flush House" << endl;
        return HandRank::FlushHouse;
    }
};

class FlushFiveChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        (void)hand;
        cout << "Flush Five" << endl;
        return HandRank::FlushFive;
    }
};

int main() {
    Hand exampleHand;

    HighCardChecker highCard;
    PairChecker pair;
    TwoPairChecker twoPair;
    ThreeOfAKindChecker threeOfAKind;
    StraightChecker straight;
    FlushChecker flush;
    FullHouseChecker fullHouse;
    FourOfAKindChecker fourOfAKind;
    StraightFlushChecker straightFlush;
    RoyalFlushChecker royalFlush;
    FiveOfAKindChecker fiveOfAKind;
    FlushHouseChecker flushHouse;
    FlushFiveChecker flushFive;

    // Demonstrasi: setiap checker mencetak nama hand-nya.
    highCard.check(exampleHand);
    pair.check(exampleHand);
    twoPair.check(exampleHand);
    threeOfAKind.check(exampleHand);
    straight.check(exampleHand);
    flush.check(exampleHand);
    fullHouse.check(exampleHand);
    fourOfAKind.check(exampleHand);
    straightFlush.check(exampleHand);
    royalFlush.check(exampleHand);
    fiveOfAKind.check(exampleHand);
    flushHouse.check(exampleHand);
    flushFive.check(exampleHand);

    return 0;
}
