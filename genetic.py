#!/usr/bin/env/ python
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
from random import randint
import random
from math import sin

class Problem:
	def __init__(self, x = None ):
		if x is not None:
			self.x = x
		else:
			self.x = random.random()*50

	def cross_over( self, partner ):
		kid = Problem( ( self.x + partner.x )/2.0 )
		return kid

	def mutation(self, seed):
		self.x = self.x + self.x*seed

'''
population: list of objects that should implement cross-over, random_selection and mutation methods.
mutation_probability: float between 0 and 1. indicates the probability of mutation.
'''
class GeneticAlgorithm:
	def __init__( self, fitness, select_parents, population, frequency = 0, mutation_probability=0.02, mutation_range = 0.1 ):
		self.fitness = fitness
	       	self.select_parents = select_parents
		self.mutation_probability = mutation_probability
		self.population = population
		self.means = []
		self.variancia = []
		self.melhor = []
		self.frequency = frequency
		self.population_size = len( population )
		self.best_of_the_best = 0
		self.mutation_range = mutation_range

	def evolve(self, max_iterations = 5000):
		for i in range( max_iterations ):
			self.generation()

	def generation(self):
		mom,dad = self.select_parents( self.population )
		kid = mom.cross_over( mom )
		if  random.random() <= self.mutation_probability :
			kid.mutation( self.mutation_range )
		indvidual = randint( 0, self.population_size-1 )
		kid_fit = self.fitness( kid ) 
		ind_fit = self.fitness( self.population[ indvidual ] )
		if kid_fit > self.best_of_the_best:
			self.best_of_the_best = kid_fit
		if ind_fit > self.best_of_the_best:
			self.best_of_the_best = ind_fit
		if kid_fit >= ind_fit :
			self.population[ indvidual ] = kid
		
		return

	def plot(self):	
		#TODO
		return


def select_parents( population ):
	return random.sample( population, 2 )

def fitness( problem ):
	return function( problem.x )


def function( x ):

         if( x < 5 ):
                 return sin(x)
         if( x < 10 ):
                 return 4*sin(x)
         return 15*sin(x)

'''
'''
def rank( population ):
	#TODO
	return


class NN:
	def __init__():
		#TODO
		return
	def cross_over():
		#TODO
		return
	def mutation():
		#TODO
		return

class Game2048:
	def __init__():
		#TODO
		return

class Architecture:
	def __init__( inteiro = 0 ):
		self.inteiro = inteiro
	def cross_over(self):
		#TODO
		return
	def mutation(self):
		#TODO
		return
	def random(self):
		#TODO
		return
		
'''
Recebe uma arquitetura, cria uma população de redes neurais neste modelo e a evolui através de um AG.
retorna o score da melhor rede neural
'''
def testar_arq( architecture, fitness ):
	#TODO
	return


'''
Recebe uma rede neural, coloca-a para jogar n vezes e retorna a media dos scores.
n_runs: int, opcional
NN: objeto redes neurais
'''
def jogar( NN, n_runs = 1 ):
	#TODO
	return


def main():
	population = []
	for i in range( 100 ):
		population.append( Problem() )
	ag = GeneticAlgorithm( fitness, select_parents, population )
	ag.evolve( 2000 )
	print ag.best_of_the_best
	return	

main()
