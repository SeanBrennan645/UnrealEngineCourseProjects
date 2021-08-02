// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(ValidWords, *WordListPath, [](const FString Word)
        {
            return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
        });

    //ValidWords = GetValidWords(Words);

    SetupGame(); 
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        CheckGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Type your guess and\npress enter to continue..."));

    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num() - 1)];
    Lives = HiddenWord.Len();

    PrintLine(TEXT("Lives remaining %i"), Lives);

    PrintLine(TEXT("The hidden word is: %s. \nIt is %i characters long."), *HiddenWord, HiddenWord.Len()); //debug line, printf formatting included in PrintLine() 

    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to play again."));
}

void UBullCowCartridge::CheckGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Congrats you guessed the correct word!"));
        EndGame();
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    if (HiddenWord.Len() == Guess.Len())
    {
        PrintLine(TEXT("You have the right amount of letters"));
    }
    else
    {
        PrintLine(TEXT("You don't have the right amount of letters"));
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
    }

    --Lives;
    
    if (Lives <= 0)
    {      
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i bulls and %i cows."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("You have lost a life"));
    PrintLine(TEXT("Lives remaining %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Guess)
{
    for(int32 Index = 0; Index < Guess.Len(); Index++)
    {
        for (int32 Comparison = Index + 1;  Comparison< Guess.Len(); Comparison++)
        {
            if (Guess[Index] == Guess[Comparison])
                return false;
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 Index = 0; Index < Guess.Len(); Index++)
        {
            if (Guess[GuessIndex] == HiddenWord[Index])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}