// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDTestPlayerState.h"

void ANGDTestPlayerState::DoScore(int ChainPosition) {
	Score += ChainPositionToFibonacciRec(ChainPosition);
	
}

int ANGDTestPlayerState::ChainPositionToFibonacci(int ChainPosition)
{
	//We use Binet's Fibonacci Number Formula to get the value at a given index
	return (FMath::Pow((1 + FMath::Sqrt(5)), ChainPosition) - FMath::Pow((1 - FMath::Sqrt(5)), ChainPosition)) /
		(FMath::Pow(2, ChainPosition) * FMath::Sqrt(5));
}

int ANGDTestPlayerState::ChainPositionToFibonacciRec(int ChainPosition)
{
	int a = 1, b = 1;
	for (int i = 3; i <= ChainPosition; i++) {
		int c = a + b;
		a = b;
		b = c;
	}
	return b;
}


