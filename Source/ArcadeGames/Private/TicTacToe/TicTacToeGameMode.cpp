// Fill out your copyright notice in the Description page of Project Settings.

#include "TicTacToeGameMode.h"
#include "TicTacToePC.h"
#include "TicTacToePawn.h"

ATicTacToeGameMode::ATicTacToeGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATicTacToePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATicTacToePC::StaticClass();
}
