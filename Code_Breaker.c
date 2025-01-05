#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Function prototypes
void GenSecCode(int SecretCode[]);
void CheckGuess(int UserGuess[], int SecretCode[]);
void clearInputBuffer(); // Clears the input buffer to prevent invalid inputs
void Instructions();

int main() {
    int SecretCode[4];    // Secret 4-digit SecretCode
    int UserGuess[4];     // User's UserGuess
    int Attempts;         // Number of Attempts
    int IsUnlocked;       // Flag for unlocking
    char PlayAgain;       // Replay option
    char Input[5];        // Input buffer for UserGuess

    srand((unsigned)time(0) ^ getpid());

    do {

        // Display Instructions
        printf("\n============================================\n");
        printf("     4-Digit Digital Door Lock Simulation    \n");
        printf("============================================\n");
        printf("Game Rules:\n");
        printf("1. A random 4-digit SecretCode with unique digits (0-9) will be generated.\n");
        printf("2. You will get detailed feedback for each digit:\n");
        printf("   - Which digit is correct and in the correct position.\n");
        printf("   - Which digit is correct but in the wrong position.\n");
        printf("3. Number of Attempts depends on Difficulty:\n");
        printf("   - Easy: 10 Attempts\n");
        printf("   - Normal: 5 Attempts\n");
        printf("   - Hard: 3 Attempts\n");
        printf("4. After each game, you can choose to restart or exit.\n");
        printf("============================================\n");

        // Select Difficulty
        int Difficulty;
        printf("\nChoose Difficulty level:\n");
        printf("1. Easy (10 Attempts)\n");
        printf("2. Normal (5 Attempts)\n");
        printf("3. Hard (3 Attempts)\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &Difficulty);
        clearInputBuffer(); // Clear the input buffer after reading difficulty

        // Set Attempts based on Difficulty
        if (Difficulty == 1) {
            Attempts = 10;
        } else if (Difficulty == 2) {
            Attempts = 5;
        } else if (Difficulty == 3) {
            Attempts = 3;
        } else {
            printf("\nInvalid choice entered. Setting difficulty to Normal by default.\n");
            Attempts = 5; // Default to Normal
        }

        // Generate a random 4-digit SecretCode
        GenSecCode(SecretCode);

        // Start the game loop
        int remainingAttempts = Attempts;
        IsUnlocked = 0;

        printf("\nYou have %d Attempts to UserGuess the 4-digit SecretCode.\n", Attempts);

        while (remainingAttempts > 0) {
            // Get the user's guess
            printf("\nEnter your 4-digit guess (e.g., 1234): ");
            scanf("%4s", Input); // Limit input to 4 characters
            clearInputBuffer(); // Clear the input buffer after reading guess

            // Validate input: Ensure it's exactly 4 digits
            int valid = 1;
            for (int i = 0; i < 4; i++) {
                if (Input[i] < '0' || Input[i] > '9') valid = 0; // Check for non-digit characters
            }
            if (Input[4] != '\0') valid = 0; // Ensure there are no extra characters beyond 4 digits
            if (!valid) {
                printf("\nInvalid input! Please enter exactly 4 digits.\n");
                continue; // Ask for input again
            }

            // Split the input into digits
            for (int i = 0; i < 4; i++) UserGuess[i] = Input[i] - '0';

            // Check the guess
            int correctDigits = 0;
            int usedCode[4] = {0}; // Tracks which SecretCode digits are already matched
            int usedGuess[4] = {0}; // Tracks which UserGuess digits are already matched

            printf("\nFeedback:\n");

            // First pass: Check for correct digits in the correct position
            for (int i = 0; i < 4; i++) {
                if (UserGuess[i] == SecretCode[i]) {
                    printf("-> Number %d is correct and in the correct position.\n", UserGuess[i]);
                    usedCode[i] = 1;  // Mark SecretCode digit as used
                    usedGuess[i] = 1; // Mark UserGuess digit as matched
                    correctDigits++;
                }
            }

            // Second pass: Check for correct digits in the wrong position
            for (int i = 0; i < 4; i++) {
                if (usedGuess[i]) continue; // Skip already matched digits

                for (int j = 0; j < 4; j++) {
                    if (!usedCode[j] && UserGuess[i] == SecretCode[j]) {
                        printf("-> Number %d is correct but in the wrong position.\n", UserGuess[i]);
                        usedCode[j] = 1; // Mark SecretCode digit as used
                        usedGuess[i] = 1; // Mark UserGuess digit as matched
                        break;
                    }
                }
            }

            // Check if all digits are correct
            if (correctDigits == 4) {
                printf("\nCongratulations! The door has been UNLOCKED!\n");
                IsUnlocked = 1;
                break;
            }

            // Decrease remaining Attempts
            remainingAttempts--;
            if (remainingAttempts > 0) {
                printf("\nAttempts remaining: %d\n", remainingAttempts);
            } else {
                printf("\nYou've used all your Attempts. The door is LOCKED!\n");
                printf("The correct SecretCode was: ");
                for (int i = 0; i < 4; i++) {
                    printf("%d", SecretCode[i]);
                }
                printf("\n");
            }
        }

        // Ask the user to play again
        printf("\nDo you want to play again? (Y/N): ");
        scanf(" %c", &PlayAgain);
        clearInputBuffer(); // Clear the input buffer after reading play again option

    } while (PlayAgain == 'Y' || PlayAgain == 'y');

    printf("\nThank you for playing! Goodbye!\n");
    return 0;
}

// Function to generate a 4-digit secret SecretCode with unique digits
void GenSecCode(int SecretCode[]) {
    int usedDigits[10] = {0}; // Array to track used digits
    for (int i = 0; i < 4; i++) {
        int digit;
        do {
            digit = rand() % 10;  // Generate digits between 0 and 9
        } while (usedDigits[digit]);
        SecretCode[i] = digit;
        usedDigits[digit] = 1; // Mark digit as used
    }
}

void clearInputBuffer() {
    while (getchar() != '\n'); // Consume all characters in the input buffer until a newline is found
}