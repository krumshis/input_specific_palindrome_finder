# input_specific_palindrome_finder
Find palindromes by annotating input of dwarf's names.
####Five Dwarves ( Gimli, Fili, Ilif, Ilmig and Mark) met at the Prancing Pony and played a word game to determine which combinations of their names resulted in a palindrome. Write a program in that prints out all of those combinations.

Here I want to explain another approach to the palindrome problem and the assumptions that I made about the input . These assumptions allowed me to do some preliminary processing on the set of input words and save substantially time on constructing palindromes. The goal was to move away from factorial complexity in all possible places.

In this problem we have personal (dwarf's) names as input (capital letter at the beginning), some names are inversions of other names, like _Fili <-> Ilif_ and _Gimli <-> Ilmig_, names are short. So here are the assumptions about the data that I made. Based on these assumptions, I significantly improved the algorithm and time of the solution.
	

 1. All input words are names - first letter uppercase, all others lower case.
 2. A word can be present in any given palindrome only once (otherwise we are looking at infinity of palindromes),  there won't be palindromes like "_A_ _Man,_ _A_ _Plan,_ _A_ _Canal-Panama_".
 3. Treat input words as "building blocks" of palindrome: palindrome can contain a word and its inversion, plus one word that is inversion to itself: "_pot_ _top_" or "_top_ _a_ _pot_". Such palindrome can never look like "_never_ _odd_ _or_ _even_" where single words are not necessarily inversions of each other.

Saying that, I can outline the following steps of the algorithm:

 1. Do some pre-processing of the input in one traverse of in.
	* gather all palindromic words in one separate vector;
	* build a map, matching words to their inversions, also collect those words in another separate vector - that will be our new and (hopefully!) reduced input.
 2.  We can't avoid building combinations and permutations altogether, but here is where we can save:
	* build combinations from new/reduced input and don't check for them being palindromes, they will be because we set it up;
	* build only the left half of each combination/permutation, the right half will be done automatically from the "mirror image" - inversions of the words from the left half by the means of map that we build in step 1;
	* if we happen to have a vector of palindromic words - insert them one by one (linear complexity)	in each palindrome;
	* finally all palindromic words are also palindromes - output them as results.

I run this on https://www.codechef.com/ide C++14(G++4.9.2). The results were equivalent for both solutions - number of palindromes and palindromes themselves (no errors). Obviously, the order was different. However the time difference was pretty big between two solutions, and bigger for larger input size.

__INPUT:__ 5 names:  _Mark, Ilif, Gimli, Fili, Ilmig_
__OUTPUT:__ 12 palindromes.
__Straight solution: __
 - Computing palindromes took __572__ microseconds
__New solution:__
 - Input preparation took __36__ microseconds
 - Actual palindrome construction took __22__ microseconds
 - Total time elapsed was __58__ microseconds

__INPUT:__ 9 names: _Mark, Bib, Gimim, Tom, Mimig, Ilif, Gimli, Fili, Ilmig_
_OUTPUT:_ 157 palindromes
__Straight solution:__
 - Computing palindromes took __1.0206e+06__ microseconds (or 1,020,600 microseconds)
__New solution:__
 - Input preparation took __49__ microseconds
 - Actual palindrome construction took __314__ microseconds
 - Total time elapsed was __363__ microseconds
