#include <iostream>
#include <vector>
#include <limits>


namespace Utils {
template<typename Iterator1, typename Iterator2, typename OutputIterator>
void merge(Iterator1 first1, Iterator1 last1,
           Iterator2 first2, Iterator2 last2,
           OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else {
            *result = *first2;
            ++first2;
        }
        ++result;
    }
    while (first1 != last1) {
        *result = *first1;
        ++result;
        ++first1;
    }
    while (first2 != last2) {
        *result = *first2;
        ++result;
        ++first2;
    }
}

template<typename Pointer, typename Iterator>
void partialCopy(Pointer buffer, Iterator first, Iterator last) {
    for (Iterator iterator = first; iterator != last; ++iterator, ++buffer) {
        *iterator = *buffer;
    }
}

template<typename Iterator, typename Pointer>
void mergeSortWithBuffer(Iterator first, Iterator last, Pointer buffer) {
    if (last - first < 2) {
        return;
    }

    const Iterator middle = first + (last - first + 1) / 2;
    mergeSortWithBuffer(first, middle, buffer);
    mergeSortWithBuffer(middle, last, buffer);
    Utils::merge(first, middle, middle, last, buffer);
    partialCopy(buffer, first, last);
}

template<typename Iterator>
void mergeSort(Iterator first, Iterator last) {
    typedef typename std::iterator_traits<Iterator>::value_type ValueType;
    std::vector<ValueType> buffer(first, last);
    mergeSortWithBuffer(first, last, buffer.begin());
}

template<typename Iterator, typename ValueType>
Iterator upperBound(Iterator first, Iterator last, ValueType value) {
    size_t length = last - first;

    while (length > 0) {
        size_t halfLength = length >> 1;

        const Iterator middle = first + halfLength;

        if (value < *middle) {
            length = halfLength;
        } else {
            length = length - halfLength - 1;
            first = middle;
            ++first;
        }
    }
    return first;
}
}

class FootballPlayer {
private:
    int64_t effectiveness;
    int playerId;
public:
    FootballPlayer() {
    }

    FootballPlayer(int64_t effectiveness, int playerId)
        : effectiveness(effectiveness), playerId(playerId)
    {
    }

    bool operator < (const FootballPlayer &player) const {
        if (effectiveness != player.effectiveness) {
            return effectiveness < player.effectiveness;
        }
        return playerId < player.playerId;
    }

    int64_t getEffectivness() const {
        return effectiveness;
    }

    int getId() const {
        return playerId;
    }
};

struct FootballTeam {
    int64_t totalEffectiveness;
    std::vector<int> playerIds;
};

std::ostream & operator << (std::ostream &os, const FootballTeam &team) {
    os << team.totalEffectiveness << "\n";
    const std::vector<int> &playerIds = team.playerIds;
    for (std::vector<int>::const_iterator iterator = playerIds.begin(); iterator != playerIds.end(); ++iterator) {
        if (iterator != playerIds.begin()) {
            os << " ";
        }
        os << *iterator;
    }
    os << "\n";
    return os;
}


std::vector<FootballPlayer> readInput() {
    int numberOfPlayers;
    std::cin >> numberOfPlayers;

    std::vector<FootballPlayer> footballPlayers(numberOfPlayers);

    for (int i = 0; i < numberOfPlayers; ++i) {
        int64_t effectiveness;
        std::cin >> effectiveness;
        footballPlayers[i] = FootballPlayer(effectiveness, i + 1);
    }
    return footballPlayers;
}

FootballTeam maximalEffectivenessTeam(std::vector<FootballPlayer> footballPlayers) {
    typedef std::vector<FootballPlayer>::iterator FootballPlayersIterator;

    FootballTeam bestTeam;
    if (footballPlayers.empty()) {
        bestTeam.totalEffectiveness = 0;
        bestTeam.playerIds = std::vector<int>();
        return bestTeam;
    }

    // Sort all players by effectiveness
    Utils::mergeSort(footballPlayers.begin(), footballPlayers.end());

    // Calculate partial sums of players' effectiveness
    std::vector<int64_t> partialSumOfEffectiveness(footballPlayers.size() + 1, 0);
    for (size_t i = 0; i < footballPlayers.size(); ++i) {
        partialSumOfEffectiveness[i + 1] = partialSumOfEffectiveness[i] + footballPlayers[i].getEffectivness();
    }

    // One player satisfies conditions
    int64_t bestTeamEffectiveness = footballPlayers.at(0).getEffectivness();
    FootballPlayersIterator bestSegmentFirst = footballPlayers.begin();
    FootballPlayersIterator bestSegmentLast  = footballPlayers.begin() + 1;

    // iterate the beginning of the segment of players
    for (FootballPlayersIterator first = footballPlayers.begin(); ; ++first) {
        FootballPlayersIterator next = first + 1;
        if (next == footballPlayers.end()) {
            break;
        }

        // Calculate the maximal effectiveness of the player that can be included in team with players (first) and (first + 1)
        int64_t maximalPlayerEffectiveness = first->getEffectivness() + next->getEffectivness();

        // Find the end of such segment by binary search
        FootballPlayersIterator last = Utils::upperBound(footballPlayers.begin(), footballPlayers.end(),
                                                         FootballPlayer(maximalPlayerEffectiveness, std::numeric_limits<int>::max()));

        // Calculate current team effectiveness of segment [first, last)
        size_t firstPosition = first - footballPlayers.begin();
        size_t lastPosition  = last - footballPlayers.begin();
        int64_t currentTeamEffectiveness = partialSumOfEffectiveness.at(lastPosition) - partialSumOfEffectiveness.at(firstPosition);

        // Update answer if needed
        if (bestTeamEffectiveness < currentTeamEffectiveness) {
            bestTeamEffectiveness = currentTeamEffectiveness;
            bestSegmentFirst = first;
            bestSegmentLast  = last;
        }
    }

    // Fill the answer
    std::vector<int> playersId;
    for (FootballPlayersIterator iterator = bestSegmentFirst; iterator != bestSegmentLast; ++iterator) {
        playersId.push_back(iterator->getId());
    }

    // Sort ids
    Utils::mergeSort(playersId.begin(), playersId.end());
    bestTeam.totalEffectiveness = bestTeamEffectiveness;
    bestTeam.playerIds.swap(playersId);
    return bestTeam;
}

int main()
{
    // Fast input
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::vector<FootballPlayer> footballPlayers = readInput();
    auto bestTeam = maximalEffectivenessTeam(footballPlayers);
    std::cout << bestTeam;

    return 0;
}

