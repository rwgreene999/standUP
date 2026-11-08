#!/usr/bin/env python3
"""Render the standUP application icon to PNG sizes.

`standUP.svg` is the canonical scalable artwork. This script produces the
raster sizes embedded in the Qt resource file and installed into the hicolor
icon theme. Re-run it only if the artwork changes; the generated PNGs are
committed so the build does not depend on Python or Pillow.

Usage:  python3 icons/generate_icons.py
"""
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw

S = 4  # supersampling factor
N = 256
OUT = Path(__file__).resolve().parent / "png"

BADGE_M = 6
BADGE_R = 58

BG_STOPS = [
    (0.00, (255, 182, 92)),
    (0.52, (240, 86, 79)),
    (1.00, (163, 44, 116)),
]

# chair geometry (256-space)
BACK = (36, 54, 22, 104)       # x, y, w, h
SEAT = (36, 140, 104, 22)
LEG_W, LEG_H, LEG_Y = 20, 50, 156
BACK_LEG_X = 42
FRONT_LEG_X = 116

# arrow geometry
AX = 186
HEAD_TIP_Y = 54
HEAD_BASE_Y = 106
HEAD_HALF = 36
SHAFT_W = 22
SHAFT_TOP = 100
SHAFT_BOTTOM = 208

PLUM = (58, 18, 48, 255)
WHITE = (255, 255, 255, 255)


def _mix(a, b, t):
    return tuple(int(round(a[i] + (b[i] - a[i]) * t)) for i in range(3))


def _grad(t, stops):
    for i in range(len(stops) - 1):
        p0, c0 = stops[i]
        p1, c1 = stops[i + 1]
        if t <= p1:
            u = 0.0 if p1 <= p0 else max(0.0, min(1.0, (t - p0) / (p1 - p0)))
            return _mix(c0, c1, u)
    return stops[-1][1]


def diagonal_gradient(size, stops):
    img = Image.new("RGB", (size, size))
    d = ImageDraw.Draw(img)
    last = 2 * size - 2
    for k in range(2 * size - 1):
        col = _grad(k / last, stops)
        if k < size:
            d.line([(k, 0), (0, k)], fill=col)
        else:
            d.line([(size - 1, k - (size - 1)), (k - (size - 1), size - 1)], fill=col)
    return img


def rrect(d, box, radius, fill):
    x, y, w, h = box
    d.rounded_rectangle([x * S, y * S, (x + w) * S, (y + h) * S], radius=radius * S, fill=fill)


def build():
    size = N * S
    canvas = Image.new("RGBA", (size, size), (0, 0, 0, 0))

    mask = Image.new("L", (size, size), 0)
    ImageDraw.Draw(mask).rounded_rectangle(
        [BADGE_M * S, BADGE_M * S, (N - BADGE_M) * S, (N - BADGE_M) * S],
        radius=BADGE_R * S,
        fill=255,
    )

    canvas.paste(diagonal_gradient(size, BG_STOPS).convert("RGBA"), (0, 0), mask)

    # subtle top sheen
    gloss = Image.new("L", (size, size), 0)
    gd = ImageDraw.Draw(gloss)
    for y in range(size):
        gd.line([(0, y), (size, y)], fill=int(56 * max(0.0, 1.0 - y / (size * 0.55))))
    sheen = Image.new("RGBA", (size, size), (255, 255, 255, 255))
    sheen.putalpha(ImageChops.multiply(gloss, mask))
    canvas = Image.alpha_composite(canvas, sheen)

    d = ImageDraw.Draw(canvas)

    # inner border
    d.rounded_rectangle(
        [BADGE_M * S + S, BADGE_M * S + S, (N - BADGE_M) * S - S, (N - BADGE_M) * S - S],
        radius=(BADGE_R - 1) * S,
        outline=(255, 255, 255, 34),
        width=2 * S,
    )

    # chair
    rrect(d, BACK, 11, PLUM)
    rrect(d, SEAT, 11, PLUM)
    rrect(d, (BACK_LEG_X, LEG_Y, LEG_W, LEG_H), 10, PLUM)
    rrect(d, (FRONT_LEG_X, LEG_Y, LEG_W, LEG_H), 10, PLUM)

    # arrow
    d.polygon(
        [
            (AX * S, HEAD_TIP_Y * S),
            ((AX - HEAD_HALF) * S, HEAD_BASE_Y * S),
            ((AX + HEAD_HALF) * S, HEAD_BASE_Y * S),
        ],
        fill=WHITE,
    )
    rrect(d, (AX - SHAFT_W // 2, SHAFT_TOP, SHAFT_W, SHAFT_BOTTOM - SHAFT_TOP), 10, WHITE)

    return canvas


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    canvas = build()
    for px in (16, 24, 32, 48, 64, 128, 256, 512):
        canvas.resize((px, px), Image.LANCZOS).save(OUT / f"standUP-{px}.png")
    print(f"wrote {len(list(OUT.glob('*.png')))} icons to {OUT}")


if __name__ == "__main__":
    main()
