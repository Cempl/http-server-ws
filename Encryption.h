#pragma once


/*******************************************************************************/
#include "Server.h"


class Encryption
{
	public:///////////////////////////////////////////////////////////////////

		// DH simplified implementation
		int get_mod_and_gen(int gen, int mod, int& private_number);

		// DH simplified implementation
		int calculated_mod(int in_mod_res, int& mod, int& private_number);

		// Hashing key (DH) and return him in its binary form
		vector<int> hashingKey(int key);

		// symbols vector in string
		string vectorCharToString(const vector<int>& inData);

		// Decode C++ wrapper over RC4 (OpenSSL - C)
		string decode_rc4(const string &data, string key);

		// Encode C++ wrapper over RC4 (OpenSSL - C)
		string encode_rc4(const string &data, string key);

		// In binary number system
		vector<int> inBinary(int inNumber);

		// String of symbols in the binary vector code symbols
		vector<int> strSymbolsToIntCodeSymbol(string inString);

		// Binary code of symbol in denary code of symbol (input one symbol)
		int fromBinary(vector<int> inBinary);

		// binary string in binary vector
		vector<int> strBinaryToIntBinary(string inBinary);

		// Binary vector in vector of symbols
		vector<char> IntbinaryToChar(vector<int> inBinary);

		// Calculate rounder keys
		vector<int> arrayKeys(vector<int> inKey, int amountRounds);

		// Divide a message into sub-blocks
		void subblock(vector<int> inBlock, vector<int>& leftBlock, vector<int>& rightBlock);

		// Func F (black box)
		vector<int> F(vector<int> subblock, vector<int> rightAraayKeys, int numberRounds);

		// Encrypt incoming data
		vector<int> encryptFeistelNetwork(vector<int> leftBlock, vector<int> rightBlock, const vector<int>& rightAraayKeys, int amountRounds);

		// Decrypt incoming data
		vector<int> decryptFeistelNetwork(const vector<int>& inBlock, int amountRounds, const vector<int>& rightAraayKeys);

};

extern Encryption Enc;