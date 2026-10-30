# Dynamic Text Mining and Linguistic Pattern Analyzer

A menu-driven text analyzer written in C. It reads a paragraph from a file or from pasted input, splits it into words using pointer traversal, stores each distinct word with its frequency in a dynamically growing array and reports statistics about the text.

## Files

| File | Purpose |
|------|---------|
| `main.h` | Shared header: `wordStat` and `Analyzer` structures, constants, prototypes |
| `main.c` | Header output, menu and the function-pointer dispatch table |
| `text_input.c` | Input helpers, growing text buffer, loading from a file, pasting text |
| `word_extract.c` | Pointer-based word extraction and the word-statistics array |
| `operations.c` | `count_words`, `longest_word`, `most_frequent`, `display_word_statistics`, `sort_word_statistics`, `search_word`, `generate_text_summary` |
| `analysis.c` | Lexical Diversity Score, average word length, memory inspection |
| `paragraph.txt` | The analysed paragraph (253 words) |
| `sample_output.txt` | A complete sample run |

## Source text

*Alice's Adventures in Wonderland*, Chapter I, Lewis Carroll (1865, public domain). Any other text can be pasted with menu option 2 (finish with a line containing only `END`). If `paragraph.txt` is replaced, update `DEFAULT_SOURCE` in `main.h`.

## Features

- No fixed text length: the text buffer starts at 256 bytes and doubles with `realloc`; the word array starts at 16 entries and doubles the same way.
- Words are extracted with pointers; duplicates are found with `strcmp`, lengths with `strlen` and words are copied with `strcpy`.
- Empty input and allocation failures are handled; all memory is freed before exit.
- Memory inspection (option 11) prints the `%p` addresses of the text buffer, the word array and one entry, with an explanation of each.

## Text-processing rules

| Item | Treatment |
|------|-----------|
| Case | Converted to lowercase (`The` and `the` are one word) |
| Punctuation | Separates words and is dropped |
| Whitespace | Any amount of spaces, tabs or newlines separates words |
| Apostrophes | Kept inside a word (`don't`, `alice's`); the curly `’` is treated the same |
| Hyphens | Kept inside a word (`rabbit-hole`); leading, trailing and double hyphens are separators |
| Words over 39 characters | Truncated to 39 characters, counted and reported |

## Menu

```
 1. Load paragraph from file
 2. Enter / paste a paragraph
 3. Count words
 4. Find the longest word
 5. Find the most frequent word
 6. Display word statistics
 7. Sort word statistics
 8. Search for a word
 9. Custom analysis (Lexical Diversity Score)
10. Generate text summary
11. Memory inspection
12. Exit
```

## Build and run

```bash
bash compiler.sh
./text_mining_app
```

or directly:

```bash
gcc -Wall -Wextra -std=c11 -g main.c text_input.c word_extract.c operations.c analysis.c -o text_mining_app
./text_mining_app
```

## Custom analysis: Lexical Diversity Score

```
LDS = distinct words / total words
```

| LDS | Meaning |
|-----|---------|
| 0.70 or higher | High diversity: words are rarely repeated |
| 0.40 to 0.69 | Moderate diversity |
| Below 0.40 | Low diversity: the text is repetitive or long |

The analysis also reports hapax words (words used exactly once) and the share of word occurrences that belong to repeated words. Longer texts score lower because common words repeat, so only compare passages of similar length.

## Sample session

```
Student ID: 00001
Project: Dynamic Text Mining and Linguistic Pattern Analyzer
Source: Alice's Adventures in Wonderland, Chapter I - Lewis Carroll (1865, public domain)
Custom Analysis: Lexical Diversity Score
Loaded paragraph.txt: 253 words, 135 distinct.

Enter your choice: 10

============ TEXT SUMMARY ============
Total number of words : 253
Distinct words        : 135
Average word length   : 4.10 characters
Longest word          : 'waistcoat-pocket' (16 characters)
Most frequent word    : 'the' (13 times)
Lexical Diversity     : 0.534 (moderate diversity)
======================================

Enter your choice: 2
Paste your text. Finish with a line containing only END.
The cat sat. The cat sat again -- and the cat didn't stop; "didn't" is a word!
END
Read 105 characters: 19 words, 12 distinct.
```

`sample_output.txt` also shows the buffer growth messages, the word table, sorting, searching and the memory addresses.

## Design notes

- `Analyzer` owns every heap block (text and statistics), so loading new text simply frees the old blocks and starts again.
- `scan_word` is used both for the text and for search queries, so a query such as `Rabbit-hole,` is normalized exactly like the text.
- Every operation has the signature `void (*)(Analyzer *)`, so one table dispatches the whole menu.
