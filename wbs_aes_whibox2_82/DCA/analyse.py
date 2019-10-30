from __future__ import division, print_function
from collections import defaultdict
import sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)




def load_round():
    with open("rounds.trace", "r") as f:
        rounds = map(int, f.readlines()[:-1])
    eprint("done reading rounds")
    return rounds


def filter_good(next):
    twonext = []
    for key, val in next.iteritems():
        if len(val) == 2:
            # eprint("next for key:", key)
            # eprint(list(val.iteritems())[0])
            # eprint(list(val.iteritems())[1])
            total = list(val.iteritems())[1][1] + list(val.iteritems())[0][1]
            rating = list(val.iteritems())[0][1] / total

            twonext.append((abs(0.5 - rating), total, val, key))

    return filter(lambda x: x[0] < 0.2 and x[1] > 9, twonext)


def goodforall(rounds):
    next = defaultdict(lambda: defaultdict(int))
    prev = rounds[0]
    for number in rounds[1:]:
        next[prev][number] += 1
        prev = number
    return next

def print_code(good, trace_ouput=True):
    for rat, total, val, key in good:
        hmm = list(val.iteritems())
        print("""if (prev == %d) {
    if (round == %d) {
        printf("0 ");
    } else if (round == %d){
        printf("1 ");
    }
}""" % (key, hmm[0][0], hmm[1][0]))


if __name__ == "__main__":
    rounds = load_round()
    next_round = goodforall(rounds)
    good = filter_good(next_round)

    print_code(good)
