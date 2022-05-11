import random

import numpy as np
from copy import deepcopy
from utils import fitness


class Genetic:
    def __init__(self, coords, population_size=100, elite_size=10, mutation_rate=0.01):
        self.coords = coords
        self.population_size = population_size
        self.elite_size = elite_size
        self.mutation_rate = mutation_rate

    def population_fitness(self, population):
        """
        Returns the list of fitness values: at i-th position, the fitness values of i-th individual in population.

        :param population: List of individual solutions
        :return: List of fitness values
        """
        return [fitness(self.coords, individual) for individual in population]

    def best_solution(self, population):
        """

        :param population:
        :return: best individual from the population, its fitness
        """
        population_fitness = self.population_fitness(population)
        best_ind = np.argmax(population_fitness)
        return population[best_ind], population_fitness[best_ind]

    def initial_population(self):
        """

        :return: List of individual solutions (random order of coords)
        """
        return [np.random.permutation(len(self.coords)) for _ in range(self.population_size)]

    def selection(self, population):
        new_population = []
        population_fitness = self.population_fitness(population)

        for _ in range(len(population)):
            randint1, randint2 = random.sample(range(len(population)), 2)
            if population_fitness[randint1] >= population_fitness[randint2]:
                new_population.append(population[randint1])
            else:
                new_population.append(population[randint2])
        """
        fitness_sum = sum(population_fitness)
        for _ in range(len(population)):
            rand = np.random.random()
            for i in range(len(population)):
                if sum(population_fitness[:i]) >= rand * fitness_sum:
                    new_population.append(population[i])
        """

        return new_population

    def crossover_population(self, population):
        # Ordered crossover
        # TODO Dla chętnych:
        # Zasadę działania tego operatora krzyżowania ilustruje przykład:
        # p1 = [1 2 3 4 5 6]; p2 = [6 5 3 4 2 1];
        # Losujemy dwa miejsca krzyżowania: np. pozycja 3 i pozycja 5, wtedy:
        # c = [1 2 5 3 4 6]
        # (liczby między miejscami krzyżowania w c1 należy przepisać w kolejności
        # w jakiej występują w p2)

        return population

    def mutate_population(self, population):

        def swap_mutate(individual):

            for x in range(len(individual)):
                if np.random.random() <= self.mutation_rate:
                    mutation_place = np.random.randint(0, len(individual))
                    individual[x], individual[mutation_place] = individual[mutation_place], individual[x]

            return individual

        def invert_mutate(individual):
            """
            if np.random.random() <= self.mutation_rate * len(individual):
                left, right = random.sample(range(len(individual)), 2)
                if left > right:
                    left, right = right, left
                while left < right:
                    individual[left], individual[right] = individual[right], individual[left]
                    left += 1
                    right -= 1
            """
            return individual

        def mutate(individual):
            if np.random.random() < 0.5:
                return swap_mutate(individual)
            else:
                return invert_mutate(individual)

        return [mutate(individual) for individual in population]

    def elitism(self, old_population, new_generation):
        pop_fitness = self.population_fitness(old_population)
        fitness_agrsorted = np.argsort(pop_fitness)
        #random.shuffle(new_generation)
        for i in range(self.elite_size):
            new_generation[i] = old_population[fitness_agrsorted[-i]]
        return new_generation

    def next_generation(self, population):
        selection = self.selection(deepcopy(population))
        next_generation = self.mutate_population(self.crossover_population(selection))
        new_population = self.elitism(population, next_generation)
        return new_population
