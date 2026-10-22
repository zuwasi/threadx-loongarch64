#!/usr/bin/env python3
"""Decode Parasoft CLOG frames emitted as ASCII hex on QEMU's UART."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


FRAME = re.compile(rb"@@CPPTCOV:([0-9A-Fa-f]{8}):([0-9A-Fa-f]*)\r?\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="output CLOG file")
    parser.add_argument("logs", nargs="+", type=Path, help="QEMU console logs")
    args = parser.parse_args()

    total_frames = 0
    missing_frames: list[Path] = []
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("wb") as clog:
        for log_path in args.logs:
            data = log_path.read_bytes()
            frames = list(FRAME.finditer(data))
            if not frames:
                missing_frames.append(log_path)
            for frame in frames:
                expected_size = int(frame.group(1), 16)
                payload_hex = frame.group(2)
                if (len(payload_hex) % 2) != 0:
                    raise SystemExit(f"odd-length coverage frame in {log_path}")
                payload = bytes.fromhex(payload_hex.decode("ascii"))
                if len(payload) != expected_size:
                    raise SystemExit(
                        f"coverage frame size mismatch in {log_path}: "
                        f"expected {expected_size}, got {len(payload)}"
                    )
                clog.write(payload)
                total_frames += 1

    if total_frames == 0:
        raise SystemExit("no Parasoft coverage frames found in QEMU logs")

    print(f"Decoded {total_frames} coverage frames from {len(args.logs)} QEMU logs")
    print(f"CLOG bytes: {args.output.stat().st_size} ({args.output})")
    if missing_frames:
        print(f"Note: {len(missing_frames)} logs had no coverage frame")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
