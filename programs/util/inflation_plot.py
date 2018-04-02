#!/usr/bin/env python3

import datetime
import json
import sys

import matplotlib
matplotlib.use("Agg")

import matplotlib.pyplot as plt
from matplotlib.ticker import AutoMinorLocator

x = []
y = []
y2 = []
y3 = []
y4 = []
y5 = []

names = ["Curate", "Content", "Producer", "Liquidity"]
inflections = {}
markers = []

colors = iter("mgbr")
shapes = iter("ovx+")

ax = plt.gca()

plt.axis([0,10,5e9,320e9])
ax.tick_params(axis="y", which="minor", left="off", right="off")
ax.tick_params(axis="y2", which="minor", left="off", right="off")
ax.tick_params(axis="y3", which="minor", left="off", right="off")
ax.tick_params(axis="y4", which="minor", left="off", right="off")
ax.tick_params(axis="y5", which="minor", left="off", right="off")

# ax.set_yscale("log")
# ax.set_xticks(range(11))
# ax.set_yticks([10e6, 20e6, 40e6, 80e6, 160e6, 320e6, 640e6, 1300e6, 2600e6, 5200e6, 10e9, 20e9, 40e9, 80e9, 160e9, 320e9])
# ax.set_yticklabels(["10M", "20M", "40M", "80M", "160M", "320M", "640M", "1.3B", "2.6B", "5.2B", "10B", "20B", "40B", "80B", "160B", "320B"])
# ax.set_ylabel("Supply")
# ax.set_xlabel("Time")
# plt.grid(True, which="major", linestyle="-")

plt.axis([0,1,10e6,300e7])
ax.set_yscale("linear")
ax.set_xticks(range(2))
ax.set_yticks([10e6, 20e6, 30e6, 40e6, 50e6, 60e6, 70e6, 80e6, 90e6, 100e6, 110e6, 120e6, 1e6, 100e6, 200e6, 300e7])
ax.set_yticklabels(["10e6", "20e6", "30e6", "40e6", "50e6", "60e6", "70e6", "80e6", "90e6", "100e6", "110e6", "120e6", "130e6", "140e6", "150e6", "300e7"])
ax.set_ylabel("Supply")
ax.set_xlabel("Time")
plt.grid(True, which="major", linestyle="-")


BLOCKS_PER_YEAR = 20*60*24*30*3

with open(sys.argv[1], "r") as f:
    n = 0
    for line in f:
        n += 1
        d = json.loads(line)
        block = int(d["b"])
        total_supply = int(d["s"])
        content_rewards = int(d['rvec'][0])
        producer_rewards = int(d['rvec'][1])
        vesting_rewards = int(d['rvec'][2])
        liquidity_rewards = int(d['rvec'][3])
        

        if (block%10000) != 0:
            continue

        px = block / BLOCKS_PER_YEAR
        py = total_supply

        x.append(px)
        y.append(py)
        y2.append(content_rewards)
        y3.append(producer_rewards)
        y4.append(vesting_rewards)
        y5.append(liquidity_rewards)
        for i in range(len(names)):
            if i == 1:
                continue
            if names[i] in inflections:
                continue
            
            inflections[names[i]] = d["b"]

plt.plot(x, y)
plt.plot(x, y, label='Total Supply')
plt.plot(x, y2, label='Content rewards')
plt.plot(x, y3, label='Producer_rewards')
plt.plot(x, y4, label='Vesting rewards')
plt.plot(x, y5, label='Liquidity rewards')

for m in markers:
    print(m)
    plt.plot(*m[0], **m[1])

lgd = plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
plt.title("10-year supply projection")
plt.savefig('supply.png', dpi=150, format='png', bbox_extra_artists=(lgd,), bbox_inches='tight')
