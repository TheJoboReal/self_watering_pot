import numpy as np
import matplotlib.pyplot as plt

# --------------------------------------------------
# Component current consumption (mA)
# --------------------------------------------------

i_4060 = 0.00800005
i_driver_standby = 0.001
i_eink_standby = 0.00001

i_pico_active = 25.0
i_pico_dormant = 0.8

i_pump = 100.0
i_moisture = 5.0
i_eink_active = 8.0

# --------------------------------------------------
# Total current during each phase (mA)
# --------------------------------------------------

current = [
    i_pico_active + i_4060 + i_pump,
    i_pico_active + i_4060 + i_moisture + i_driver_standby,
    i_pico_active + i_4060 + i_eink_active + i_driver_standby,
    i_pico_dormant + i_4060 + i_driver_standby + i_eink_standby,
]

# Phase boundaries in seconds
# Pump:    0–5 seconds
# Sensor:  5–7 seconds
# E-ink:   7–10 seconds
# Dormant: 10–21600 seconds (6 hours)

times = np.array([0, 5, 7, 10, 21600])

# --------------------------------------------------
# Nonlinear time axis
# --------------------------------------------------

# First 10 seconds occupy 65% of the graph.
# Remaining 21590 seconds occupy 35%.


def compressed_time(seconds):
    seconds = np.asarray(seconds)

    return np.where(
        seconds <= 10, 0.65 * seconds / 10, 0.65 + 0.35 * (seconds - 10) / (21600 - 10)
    )


# --------------------------------------------------
# Create plot
# --------------------------------------------------

fig, ax = plt.subplots(figsize=(12, 5.5))

ax.stairs(current, compressed_time(times), linewidth=2.5)

# Line marking the change in time scale
ax.axvline(0.65, linestyle="--", linewidth=1)

# --------------------------------------------------
# Annotations
# --------------------------------------------------

ax.annotate(
    "Pump\n5 seconds",
    xy=(0.16, 125),
    xytext=(0.16, 100),
    ha="center",
    arrowprops={"arrowstyle": "-", "lw": 1},
)

ax.text(0.39, 39, "Sensor\n2 s", ha="center", fontsize=10)

ax.text(0.55, 45, "E-ink\n3 s", ha="center", fontsize=10)

ax.text(0.82, 15, "Dormant\n5 h 59 m 50 s", ha="center", fontsize=11)

ax.text(0.65, 136, "Time scale changes here", ha="center", fontsize=10)

# --------------------------------------------------
# Axis labels and ticks
# --------------------------------------------------

ticks_seconds = [0, 5, 7, 10]
ticks_hours = [3600, 10800, 21600]

ax.set_xticks([*compressed_time(ticks_seconds), *compressed_time(ticks_hours)])

ax.set_xticklabels(["0 s", "5 s", "7 s", "10 s", "1 h", "3 h", "6 h"])

ax.set_xlim(0, 1)
ax.set_ylim(0, 145)

ax.set_xlabel("Time since wake-up (expanded active phase / compressed dormant phase)")

ax.set_ylabel("Estimated system current (mA)")

ax.set_title("Six-hour watering cycle — active period expanded")

ax.grid(axis="y", alpha=0.3)

plt.tight_layout()

# Save high-resolution figure for Typst report
plt.savefig("current_draw_6h.png", dpi=300, bbox_inches="tight")

plt.show()
