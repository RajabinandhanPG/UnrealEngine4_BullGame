// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() 
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);                                     ///Locating the text file HiddenWordList.txt
    Isograms = GetValidWords(Words);
    SetupGame();                                                                                      //Game starts here                                                                 
}
void UBullCowCartridge::OnInput(const FString& PlayerInput)                   // When the player hits enter
{
    if (bGameOver)                                                      //This is made true from the EndGame() Function and reset to false in SetupGame()
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput); 
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Hi there!, Welcome to the Bull&Cow game"));             // Welcoming the Player
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    Lives = HiddenWord.Len()*2;
    //PrintLine(TEXT("The hidden words is %s"),*HiddenWord);
    bGameOver = false;

    PrintLine(TEXT("You have %i lives"),Lives);
    PrintLine(TEXT("Guess the %i letter word"),HiddenWord.Len());
    PrintLine(TEXT("Press Enter to Continue"));   

}

void UBullCowCartridge::EndGame()
{
    PrintLine(TEXT("\nPress Enter To Play Again..."));
    bGameOver=true;
}



void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)                                             //Winning The game
    {
    PrintLine(TEXT("You got it right!"));
    EndGame();
    return;
    }
    
    
    if(Guess.Len() != HiddenWord.Len())                                                     //Where it starts going wrong XoX
    {
        PrintLine(TEXT("The Hidden word is %i letters long,Try Again"),HiddenWord.Len());
        templives++;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating Letters,Guess Again"));
        templives++;
    }

    if(templives>=2)
    {
        PrintLine(TEXT("2 or more Errors are made,You have lost a life"));                                         //Removing the lives
        Lives -=1;
        PrintLine(TEXT("Guess Again, You have Got %i Lives left"),Lives);
        templives = templives%2;
        return;
    }
    else if(templives==1)
    {
        PrintLine(TEXT("Another mistake will lead to losing a Life"));
        return;
    }
   
    PrintLine(TEXT("You have Lost a Life,Guess Again"));                                         //Removing the lives
    Lives -=1;
    PrintLine(TEXT("Guess Again, You have Got %i Lives left"),Lives);
            
           
    if (Lives <= 0)
    {

        ClearScreen();
        PrintLine(TEXT("You have no Lives left!!"));
        PrintLine(TEXT("The Hidden Word was : %s"), *HiddenWord);
        
        EndGame();
        return;
   
    }
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
 
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const               //Comparing Individual array characters with each other
{
    int32 Index = 0;
    
    for ( ; Index < Word.Len(); Index++)
    {
        for ( int32 Comparision = Index + 1; Comparision < Word.Len(); Comparision++)
        {
            if(Word[Index] == Word[Comparision])
            {
                return false;
            }
        }
    
    }
    

     return true;

}

TArray<FString> UBullCowCartridge::GetValidWords (const TArray<FString>& WordList) const
{
 TArray<FString> ValidWords;

    for(FString Word : WordList)
    {   
        if(Word.Len() >= 4 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
            ValidWords.Emplace(Word);
            }
        }
       
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows (const FString& Guess) const
{

    FBullCowCount count;
    for(int32 GuessIndex=0; GuessIndex<Guess.Len();GuessIndex++)
    {
        if(Guess[GuessIndex]== HiddenWord[GuessIndex])
        {
            count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
        {
           
            if(Guess[GuessIndex]==HiddenWord[HiddenIndex])
            {
                count.Cows++;
                break;
            }
        }
        
    }
    return count;
}
