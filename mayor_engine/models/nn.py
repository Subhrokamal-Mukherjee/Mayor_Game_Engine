import math
import random


class TinyMLP:
    """Small dependency-free MLP for deterministic scaffolding.

    This is *not* meant for production training; it provides portable NN-like
    behavior until PyTorch training loops are added.
    """

    def __init__(self, in_dim: int, hidden_dim: int, out_dim: int, seed: int) -> None:
        rng = random.Random(seed)
        self.w1 = [[rng.uniform(-0.1, 0.1) for _ in range(in_dim)] for _ in range(hidden_dim)]
        self.b1 = [rng.uniform(-0.05, 0.05) for _ in range(hidden_dim)]
        self.w2 = [[rng.uniform(-0.1, 0.1) for _ in range(hidden_dim)] for _ in range(out_dim)]
        self.b2 = [rng.uniform(-0.05, 0.05) for _ in range(out_dim)]

    @staticmethod
    def _relu(x: float) -> float:
        return x if x > 0 else 0.0

    def forward(self, x: list[float]) -> list[float]:
        h = []
        for row, b in zip(self.w1, self.b1):
            v = sum(w * xi for w, xi in zip(row, x)) + b
            h.append(self._relu(v))

        y = []
        scale = 1.0 / math.sqrt(len(h))
        for row, b in zip(self.w2, self.b2):
            v = (sum(w * hi for w, hi in zip(row, h)) * scale) + b
            y.append(v)
        return y
