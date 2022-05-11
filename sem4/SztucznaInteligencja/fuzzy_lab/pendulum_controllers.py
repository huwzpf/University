from abc import ABC, abstractmethod
from simpful import *


##### Cart Pole Env info:

# ### Action Space

# The action is a `ndarray` with shape `(1,)` representing the torque applied to free end of the pendulum.

# | Num | Action | Min  | Max |
# |-----|--------|------|-----|
# | 0   | Torque | -2.0 | 2.0 |


# ### Observation Space

# The observation is a `ndarray` with shape `(3,)` representing the x-y coordinates of the pendulum's free end and its
# angular velocity.

# | Num | Observation      | Min  | Max |
# |-----|------------------|------|-----|
# | 0   | x = cos(theta)   | -1.0 | 1.0 |
# | 1   | y = sin(angle)   | -1.0 | 1.0 |
# | 2   | Angular Velocity | -8.0 | 8.0 |


# ### Rewards

# The reward function is defined as:
# *r = -(theta<sup>2</sup> + 0.1 * theta_dt<sup>2</sup> + 0.001 * torque<sup>2</sup>)*
# where `$\theta$` is the pendulum's angle normalized between *[-pi, pi]* (with 0 being in the upright position).
# Based on the above equation, the minimum reward that can be obtained is *-(pi<sup>2</sup> + 0.1 * 8<sup>2</sup> +
# 0.001 * 2<sup>2</sup>) = -16.2736044*, while the maximum reward is zero (pendulum is upright with zero velocity and
# no torque applied).


# ### Starting State

# The starting state is a random angle in *[-pi, pi]* and a random angular velocity in *[-1,1]*.

# ### Episode Termination
# The episode terminates at 200 time steps.


class Controller(ABC):
    def __init__(self, action_space, **kwargs):
        self.action_space = action_space

    @abstractmethod
    def get_action(self, observation):
        pass


class RandomController(Controller):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def get_action(self, observation):
        return self.action_space.sample()


class TestController(Controller):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def get_action(self, observation):
        return [1.3]  # mozna zmieniac wartosci by naocznie zobaczyc wplyw wartosci akcji


class FuzzyController(Controller):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.fis = self.construct_fis()
        self.fis.produce_figure("my_pendulum_controller")
        self.observation_keys = ["Y", "X", "AngV"]
        self.output_keys = ["Torque"]

    def construct_fis(self):
        FS = FuzzySystem(show_banner=False)

        # Zdefiniuj wartości i zmienne lingwistyczne dla wejść do systemu
        x1 = FuzzySet(function=Trapezoidal_MF(a=-0.6, b=-0.4, c=-0.01, d=0), term="little_right")
        x2 = FuzzySet(function=Trapezoidal_MF(a=0, b=0.01, c=0.4, d=0.6), term="little_left")
        FS.add_linguistic_variable("X", LinguisticVariable([x1, x2], concept="X",
                                                                 universe_of_discourse=[-1, 1]))

        v1 = FuzzySet(function=Trapezoidal_MF(a=-8, b=-8, c=-2, d=0.2), term="neg")
        v2 = FuzzySet(function=Trapezoidal_MF(a=-7, b=-1, c=-0.2, d=0), term="little_neg")
        v3 = FuzzySet(function=Trapezoidal_MF(a=-0.2, b=2, c=8, d=8), term="pos")
        v4 = FuzzySet(function=Trapezoidal_MF(a=0, b=0.2, c=1, d=7), term="little_pos")
        FS.add_linguistic_variable("AngV", LinguisticVariable([v1, v2, v3, v4], concept="V",
                                                                 universe_of_discourse=[-8, 8]))

        y1 = FuzzySet(function=Triangular_MF(a=-1, b=-1, c=0), term="down")
        y2 = FuzzySet(function=Trapezoidal_MF(a=0.5, b=0.7, c=1, d=1), term="far_up")
        FS.add_linguistic_variable("Y", LinguisticVariable([y1, y2], concept="Y",
                                                           universe_of_discourse=[-1, 1]))

        # Zdefiniuj wartości i zmienne lingwistyczne dla wyjść systemu

        t1 = FuzzySet(function=Trapezoidal_MF(a=-2, b=-2, c=-1.5, d=0), term="neg")
        t2 = FuzzySet(function=Trapezoidal_MF(a=0, b=1.5, c=2, d=2), term="pos")
        FS.add_linguistic_variable("Torque", LinguisticVariable([t1, t2], concept="Torque",
                                                                 universe_of_discourse=[-2, 2]))
        # Zdefiniuj reguły systemu
        r1 = "IF (AngV IS little_neg) AND (Y IS down) THEN (Torque IS neg)"
        r2 = "IF (AngV IS little_pos) AND (Y IS down) THEN (Torque IS pos)"
        r3 = "IF ((AngV IS neg) AND (Y IS far_up)) THEN (Torque IS pos)"
        r4 = "IF ((AngV IS pos) AND (Y IS far_up)) THEN (Torque IS neg)"
        r5 = "IF ((Y IS far_up) AND (X IS little_left)) THEN (Torque IS neg)"
        r6 = "IF ((Y IS far_up) AND (X IS little_right)) THEN (Torque IS pos)"

        FS.add_rules([r1, r2, r3, r4, r5, r6])

        return FS

    def get_action(self, observation):
        for k, v in zip(self.observation_keys, observation):
            self.fis.set_variable(k, v)

        res = self.fis.Mamdani_inference([k for k in self.output_keys])
        return [res[k] for k in self.output_keys]

