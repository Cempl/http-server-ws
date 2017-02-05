/*******************************************************************************/
#include "Encryption.h"


/*******************************************************************************/
Encryption Enc;


/*******************************************************************************/
int Encryption::get_mod_and_gen(int gen, int mod, int& private_number)
{
	// initialization random number generator
	srand((unsigned int)time(0));

	// generate private number
	private_number = rand() % (6 - 1) + 1;

	// calculate the degree
	int degree = (int)pow(gen, private_number);

	// calculate modulus
	int out_mod_res = degree % mod;

	// give results
	return out_mod_res;
}


/*******************************************************************************/
int Encryption::calculated_mod(int in_mod_res, int& mod, int& private_number)
{
	// calculate the degree (an input module)
	int degree = (int)pow(in_mod_res, private_number);

	// calculate the secret key
	int secret_key = degree % mod;

	// give results
	return secret_key;
}


/*******************************************************************************/
vector<int> Encryption::hashingKey(int key)
{
	vector<int> resKey(3072);

	string secretKeyToStr = to_string(key);

	unsigned char digest[SHA512_DIGEST_LENGTH];

	char secretKeyHash[SHA512_DIGEST_LENGTH * 2 + 1];
	char secretKeyToChar[5];

	strncpy_s(secretKeyToChar, secretKeyToStr.c_str(), sizeof(secretKeyToChar));

	SHA512((unsigned char*)&secretKeyToChar, strlen(secretKeyToChar), (unsigned char*)&digest);

	for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
	{
		sprintf_s(&secretKeyHash[i * 2], (SHA512_DIGEST_LENGTH * 2 + 1), "%02x", (unsigned int)digest[i]);
	}
	
	// Copy in string
	string secretKeyStrHash(secretKeyHash);

	resKey = strSymbolsToIntCodeSymbol(secretKeyStrHash);

	return resKey;
}


/*******************************************************************************/
string Encryption::vectorCharToString(const vector<int>& inData)
{
	vector<char> tmp_res;

	tmp_res = IntbinaryToChar(inData);

	string res(tmp_res.begin(), tmp_res.end());

	return res;
}


/*******************************************************************************/
string Encryption::decode_rc4(const string &data, string key)
{
	RC4_KEY key_rc;

	size_t len = data.size();

	size_t keyLength = key.length();

	unsigned char *obuf = (unsigned char*)malloc(len + 1);
	memset(obuf, 0, len + 1);

	RC4_set_key(&key_rc, (int)keyLength, (const unsigned char*)key.c_str());
	RC4(&key_rc, len, (const unsigned char*)data.c_str(), obuf);

	string decode_data((char*)obuf, len);
	free(obuf);

	return decode_data;
}


/*******************************************************************************/
string Encryption::encode_rc4(const string &data, string key)
{
	RC4_KEY key_rc;

	size_t len = data.size();

	size_t keyLength = key.length();

	unsigned char *obuf = (unsigned char*)malloc(len + 1);
	memset(obuf, 0, len + 1);

	RC4_set_key(&key_rc, (int)keyLength, (const unsigned char*)key.c_str());
	RC4(&key_rc, len, (const unsigned char*)data.c_str(), obuf);

	string encode_data((char*)obuf, len);
	free(obuf);

	return encode_data;
}


/*******************************************************************************/
vector<int> Encryption::inBinary(int inNumber)
{
	vector<int> res;

	int tmp_number = int();

	do
	{
		tmp_number = (inNumber % 2);
		inNumber /= 2;

		res.push_back(tmp_number);
	} while (inNumber != 0);

	reverse(res.begin(), res.end());

	return res;
}


/*******************************************************************************/
vector<int> Encryption::strSymbolsToIntCodeSymbol(string inString)
{
	size_t sizeInString = inString.size();

	vector<int> intCodeSymbols;
	vector<int> binaryCodeSymbols;
	vector<int> tmp_binaryCode;

	for (int i = 0; i < sizeInString; ++i)
	{
		intCodeSymbols.push_back(inString[i]);
	}

	size_t sizeIntCodeSymbols = intCodeSymbols.size();

	for (int i = 0; i < sizeIntCodeSymbols; ++i)
	{
		tmp_binaryCode = inBinary(intCodeSymbols[i]);

		size_t sizeTmpBinaryCode = tmp_binaryCode.size();
		size_t amountAddNull = 24 - sizeTmpBinaryCode;

		for (int i = 0; i < amountAddNull; ++i)
		{
			binaryCodeSymbols.push_back(0);
		}

		for (size_t i = 0; i < sizeTmpBinaryCode; ++i)
		{
			binaryCodeSymbols.push_back(tmp_binaryCode[i]);
		}

		tmp_binaryCode.clear();
	}

	return binaryCodeSymbols;
}


/*******************************************************************************/
int Encryption::fromBinary(vector<int> inBinary)
{
	vector<int> tmp_res;

	size_t sizeInData = inBinary.size();
	size_t degree = sizeInData;
	int res = int();

	for (int i = 0; i < sizeInData; ++i)
	{
		tmp_res.push_back(inBinary[i] * ((int)(pow(2, --degree))));
	}

	for (int i = 0; i < sizeInData; ++i)
	{
		res += tmp_res[i];
	}

	return res;
}


/*******************************************************************************/
vector<int> Encryption::strBinaryToIntBinary(string inBinary)
{
	string tmp = string();

	size_t lenInBinary = inBinary.size();

	vector<int> res;

	for (unsigned int i = 0; i < lenInBinary; ++i)
	{
		tmp = inBinary[i];

		int tmp_int_bin = stoi(tmp);

		res.push_back(tmp_int_bin);
	}

	return res;
}


/*******************************************************************************/
vector<char> Encryption::IntbinaryToChar(vector<int> inBinary)
{
	size_t sizeInBinary = inBinary.size();

	int sizeBlock = 24;
	int beginBlock = 0;
	int endBlock = sizeBlock;
	int countBlock = (int)sizeInBinary / sizeBlock;

	vector<int> symbolCodeBinary;
	vector<char> res;

	for (int i = 0; i < sizeInBinary; ++i)
	{
		if (countBlock)
		{
			for (int d = beginBlock; d < endBlock; ++d)
			{
				symbolCodeBinary.push_back(inBinary[d]);
			}

			beginBlock += sizeBlock;
			endBlock += sizeBlock;
			--countBlock;

			res.push_back(fromBinary(symbolCodeBinary));

			symbolCodeBinary.clear();
		}
		else
		{
			break;
		}
	}

	return res;
}


/*******************************************************************************/
vector<int> Encryption::arrayKeys(vector<int> inKey, int amountRounds)
{
	vector<int> rightAraayKeys;
	vector<int> tmpRounderKeys;
	vector<int> readyRounderRey;

	// Number of key expansion
	const int sincerelyNumber = 2;

	size_t sizeInKey = inKey.size();
	size_t halfSizeInKey = sizeInKey / 2;

	// Take the right half of the key
	for (size_t i = halfSizeInKey; i < sizeInKey; ++i)
	{
		rightAraayKeys.push_back(inKey[i]);
	}

	// Calculate the key for each round
	int rounderKey = int();

	for (int i = 1; i <= amountRounds; ++i)
	{
		rounderKey = (int)pow(sincerelyNumber, i);

		tmpRounderKeys = inBinary(rounderKey);

		size_t sizeTmpRounderKey = tmpRounderKeys.size();
		size_t amountBitsToNull = halfSizeInKey - sizeTmpRounderKey;
		size_t sizeReadyKey = sizeTmpRounderKey + amountBitsToNull;

		for (int i = 0; i < amountBitsToNull; ++i)
		{
			readyRounderRey.push_back(0);
		}

		for (int i = 0; i < sizeTmpRounderKey; ++i)
		{
			readyRounderRey.push_back(tmpRounderKeys[i]);
		}

		size_t tmp_sizeRightAraayKeys = rightAraayKeys.size();
		size_t lastRecordedKeyBegin = tmp_sizeRightAraayKeys - sizeReadyKey;

		for (int i = 0; i < sizeReadyKey; ++i)
		{
			readyRounderRey[i] = readyRounderRey[i] ^ rightAraayKeys[lastRecordedKeyBegin];

			++lastRecordedKeyBegin;
		}

		for (int i = 0; i < sizeReadyKey; ++i)
		{
			rightAraayKeys.push_back(readyRounderRey[i]);
		}

		readyRounderRey.clear();
	}

	return rightAraayKeys;
}


/*******************************************************************************/
void Encryption::subblock(vector<int> inBlock, vector<int>& leftBlock, vector<int>& rightBlock)
{
	size_t sizeInBlock = inBlock.size();
	size_t halfSizeInBlock = sizeInBlock / 2;

	// Add values in left block 
	for (int i = 0; i < halfSizeInBlock; ++i)
	{
		leftBlock.push_back(inBlock[i]);
	}

	// Add values in right block
	for (size_t i = halfSizeInBlock; i < sizeInBlock; ++i)
	{
		rightBlock.push_back(inBlock[i]);
	}
}


/*******************************************************************************/
vector<int> Encryption::F(vector<int> subblock, vector<int> rightAraayKeys, int numberRounds)
{
	vector<int> res;

	size_t sizeSubblock = subblock.size();
	size_t endKey = size_t();
	size_t beginKey = size_t();

	if (numberRounds == 0)
	{
		endKey = sizeSubblock;
		beginKey = 0;
	}
	else
	{
		endKey = sizeSubblock * (numberRounds + 1);
		beginKey = endKey - sizeSubblock;
	}

	int i = 0;

	for (beginKey; beginKey < endKey; ++beginKey)
	{
		res.push_back(subblock[i] ^ rightAraayKeys[beginKey]);

		++i;
	}

	return res;
}


/*******************************************************************************/
vector<int> Encryption::encryptFeistelNetwork(vector<int> leftBlock, vector<int> rightBlock, const vector<int>& rightAraayKeys, int amountRounds)
{
	vector<int> res;
	vector<int> temp;
	vector<int> tempResF;

	size_t sizeRightBlock = rightBlock.size();

	for (int i = 0; i < amountRounds; ++i)
	{
		tempResF = F(leftBlock, rightAraayKeys, i);

		for (int k = 0; k < sizeRightBlock; ++k)
		{
			temp.push_back(rightBlock[k] ^ tempResF[k]);
		}

		rightBlock = leftBlock;

		leftBlock = temp;

		temp.clear();
	}

	for (int i = 0; i < sizeRightBlock; ++i)
	{
		res.push_back(leftBlock[i]);
	}

	for (int i = 0; i < sizeRightBlock; ++i)
	{
		res.push_back(rightBlock[i]);
	}

	return res;
}


/*******************************************************************************/
vector<int> Encryption::decryptFeistelNetwork(const vector<int>& inBlock, int amountRounds, const vector<int>& rightAraayKeys)
{
	vector<int> res;
	vector<int> leftBlock;
	vector<int> rightBlock;
	vector<int> temp;
	vector<int> tempResF;

	subblock(inBlock, leftBlock, rightBlock);

	size_t sizeRightBlock = rightBlock.size();

	for (int i = amountRounds - 1; i >= 0; --i)
	{
		tempResF = F(rightBlock, rightAraayKeys, i);

		for (int k = 0; k < sizeRightBlock; ++k)
		{
			temp.push_back(leftBlock[k] ^ tempResF[k]);
		}

		leftBlock = rightBlock;

		rightBlock = temp;

		temp.clear();
	}

	for (int i = 0; i < sizeRightBlock; ++i)
	{
		res.push_back(leftBlock[i]);
	}

	for (int i = 0; i < sizeRightBlock; ++i)
	{
		res.push_back(rightBlock[i]);
	}

	return res;
}