#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

class FootballPlayer {
private:
    long long effectiveness;
    int playerId;
public:
    FootballPlayer(const long long &effectiveness, const int &playerId)
        : effectiveness(effectiveness), playerId(playerId)
    {
    }

    bool operator < (const FootballPlayer &player) const {
        if (effectiveness != player.effectiveness) {
            return effectiveness < player.effectiveness;
        }
        return playerId < player.playerId;
    }

    long long getEffectivness() const {
        return effectiveness;
    }

    int getId() const {
        return playerId;
    }
};

std::vector<FootballPlayer> readInput() {
    int numberOfPlayers;
    std::cin >> numberOfPlayers;

    std::vector<FootballPlayer> footballPlayers;

    for (int i = 1; i <= numberOfPlayers; ++i) {
        long long effectiveness;
        std::cin >> effectiveness;
        footballPlayers.push_back(FootballPlayer(effectiveness, i));
    }
    return footballPlayers;
}


std::pair<long long, std::vector<int> > maximalEffectivenessTeam(std::vector<FootballPlayer> footballPlayers) {
    // If no players were given
    if (footballPlayers.empty()) {
        return make_pair(0ll, std::vector<int>());
    }

    // Sort all players by effectiveness
    std::stable_sort(footballPlayers.begin(), footballPlayers.end());

    // Calculate partial sums of players' effectiveness
    std::vector<long long> partialSumOfEffectiveness(footballPlayers.size() + 1, 0);
    for (size_t i = 0; i < footballPlayers.size(); ++i) {
        partialSumOfEffectiveness[i + 1] = partialSumOfEffectiveness[i] + footballPlayers[i].getEffectivness();
    }

    // One player satisfies conditions
    long long bestTeamEffectiveness = footballPlayers.at(0).getEffectivness();
    size_t bestSegmentBegin = 0;
    size_t bestSegmentEnd   = 1;

    // iterate the beginning of the segment of players
    for (size_t begin = 0; begin + 1 < footballPlayers.size(); ++begin) {
        // Calculate the maximal effectiveness of the player that can be included in team with players (begin) and (begin + 1)
        long long maximalPlayerEffectiveness = footballPlayers[begin].getEffectivness() + footballPlayers[begin + 1].getEffectivness();
        // Find the end of such segment by binary search
        size_t end = std::upper_bound(footballPlayers.begin(), footballPlayers.end(), FootballPlayer(maximalPlayerEffectiveness, std::numeric_limits<int>::max())) - footballPlayers.begin();

        // Calculate current team effectiveness of segment [begin, end)
        long long currentTeamEffectiveness = partialSumOfEffectiveness[end] - partialSumOfEffectiveness[begin];

        // Update answer if needed
        if (bestTeamEffectiveness < currentTeamEffectiveness) {
            bestTeamEffectiveness = currentTeamEffectiveness;
            bestSegmentBegin = begin;
            bestSegmentEnd = end;
        }
    }

    // Fill the answer
    std::vector<int> playersId;
    for (size_t i = bestSegmentBegin; i < bestSegmentEnd; ++i) {
        playersId.push_back(footballPlayers[i].getId());
    }
    // Sort id
    std::stable_sort(playersId.begin(), playersId.end());
    return make_pair(bestTeamEffectiveness, playersId);
}

// Output results in stdout
void printResults(const std::pair<long long, std::vector<int> > &result) {
    std::cout << result.first << "\n";
    const std::vector<int> &footballPlayerIds = result.second;
    for (size_t i = 0; i < footballPlayerIds.size(); ++i) {
        if (i) {
            std::cout << " ";
        }
        std::cout << footballPlayerIds.at(i);
    }
    std::cout << "\n";
}

int main()
{
    // Fast input
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::vector<FootballPlayer> footballPlayers = readInput();
    auto bestTeam = maximalEffectivenessTeam(footballPlayers);
    printResults(bestTeam);

    return 0;
}

