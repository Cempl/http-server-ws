// preloader start
function go_preloader()
{
    document.getElementById("close-popup").style.visibility = "hidden";
}

// Calculate rounder keys
function arrayKeys(inKey, amountRounds)
{
    rightAraayKeys = [];
    tmpRounderKeys = [];
	readyRounderRey = [];

    // Number of key expansion
	const sincerelyNumber = 2;
    
	sizeInKey = inKey.length;
	halfSizeInKey = sizeInKey / 2;

    // Take the right half of the key
	for (var i = halfSizeInKey; i < sizeInKey; ++i)
	{
		rightAraayKeys.push(inKey[i]);
	}

    // Calculate the key for each round
	var rounderKey;

	var sizeRightAraayKeys = rightAraayKeys.length;

	for (var i = 1; i <= amountRounds; ++i)
	{
		rounderKey = Math.pow(sincerelyNumber, i);
	
		tmpRounderKeys = rounderKey.toString(2);
		
		sizeTmpRounderKey = tmpRounderKeys.length;
		amountBitsToNull = halfSizeInKey - sizeTmpRounderKey;
		sizeReadyKey = sizeTmpRounderKey + amountBitsToNull;
		
		for (var k = 0; k < amountBitsToNull; ++k)
		{
			readyRounderRey.push(0);
		}

		for (var z = 0; z < sizeTmpRounderKey; ++z)
		{
			readyRounderRey.push(tmpRounderKeys[z]);
		}

		var tmp_sizeRightAraayKeys = rightAraayKeys.length;
		var lastRecordedKeyBegin = tmp_sizeRightAraayKeys - sizeReadyKey;

		for (var g = 0; g < sizeReadyKey; ++g)
		{
			readyRounderRey[g] = readyRounderRey[g] ^ rightAraayKeys[lastRecordedKeyBegin];

			++lastRecordedKeyBegin;
		}

		for (r = 0; r < sizeReadyKey; ++r)
		{
			rightAraayKeys.push(readyRounderRey[r]);
		}

		readyRounderRey = [];
	}

    return rightAraayKeys;
}


// Create subblock
function subblock(inBlock, leftBlock, rightBlock)
{
	var sizeInBlock = inBlock.length;
	var halfSizeInBlock = sizeInBlock / 2;

	// Add values in left block 
	for (var i = 0; i < halfSizeInBlock; ++i)
	{
		leftBlock.push(inBlock[i]);
	}

	// Add values in right block
	for (var i = halfSizeInBlock; i < sizeInBlock; ++i)
	{
		rightBlock.push(inBlock[i]);
	}
}


// Func F (black box)
function F(subblock, rightAraayKeys, numberRounds)
{
	var res = [];

	sizeSubblock = subblock.length;
	var endKey;
	var beginKey;

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

	var i = 0;

	for (beginKey; beginKey < endKey; ++beginKey)
	{
		res.push(subblock[i] ^ rightAraayKeys[beginKey]);

		++i;
	}

	return res;
}


// Encryption
function encryptFeistelNetwork(leftBlock, rightBlock, rightAraayKeys, amountRounds)
{
	var res = [];
	var temp = [];
	var tempResF = [];

	var sizeRightBlock = rightBlock.length;

	for (var i = 0; i < amountRounds; ++i)
	{
		tempResF = F(leftBlock, rightAraayKeys, i);

		for (var k = 0; k < sizeRightBlock; ++k)
		{
			temp.push(rightBlock[k] ^ tempResF[k]);
		}

		rightBlock = leftBlock;

		leftBlock = temp;

		temp = [];
	}

	for (var i = 0; i < sizeRightBlock; ++i)
	{
		res.push(leftBlock[i]);
	}

	for (var i = 0; i < sizeRightBlock; ++i)
	{
		res.push(rightBlock[i]);
	}

	return res.join('');
}


//
function decryptFeistelNetwork(inBlock, amountRounds, rightAraayKeys)
{
	var res = [];
	var leftBlock = [];
	var rightBlock = [];
	var temp = [];
	var tempResF = [];

	subblock(inBlock, leftBlock, rightBlock);

	var sizeRightBlock = rightBlock.length;

	for (var i = amountRounds - 1; i >= 0; --i)
	{
		tempResF = F(rightBlock, rightAraayKeys, i);

		for (var k = 0; k < sizeRightBlock; ++k)
		{
			temp.push(leftBlock[k] ^ tempResF[k]);
		}

		leftBlock = rightBlock;

		rightBlock = temp;

		temp = [];
	}

	for (var i = 0; i < sizeRightBlock; ++i)
	{
		res.push(leftBlock[i]);
	}

	for (var i = 0; i < sizeRightBlock; ++i)
	{
		res.push(rightBlock[i]);
	}

	return res.join('');
}


// Encoding in binary
function inBinary(inData)
{
    var tmp_inData = String(inData);
    var lengthInData = tmp_inData.length;
    var tmp_Binary;
    var res;
    var additionZero = [];

    var someSymbol = [];

    for(var i = 0; i < lengthInData; ++i)
    {
        tmp_Binary = tmp_inData[i].charCodeAt().toString(2);
        
        var lengthTmpBinary = tmp_Binary.length;
        var beginBorder = 24 - lengthTmpBinary;

        for(var k = 0; k < 24; ++k)
        {
          
            additionZero[k] = 0;
        }

        var l = 0;

        for(var t = beginBorder; t < 24; ++t)
        {
            additionZero[t] = tmp_Binary[l];

            ++l
        }

        someSymbol[i] = additionZero.join('');
    }

    return res = someSymbol.join('');
}


// RC4
function rc4(key, str) {
	var s = [], j = 0, x, res = '';
	for (var i = 0; i < 256; i++) {
		s[i] = i;
	}
	for (i = 0; i < 256; i++) {
		j = (j + s[i] + key.charCodeAt(i % key.length)) % 256;
		x = s[i];
		s[i] = s[j];
		s[j] = x;
	}
	i = 0;
	j = 0;
	for (var y = 0; y < str.length; y++) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		x = s[i];
		s[i] = s[j];
		s[j] = x;
		res += String.fromCharCode(str.charCodeAt(y) ^ s[(s[i] + s[j]) % 256]);
	}
  
	return res;
}


// DH and Encrypt Auth data
function encryptAuthData()
{
    // gen  
    var g_beg = 2;
    var g_end = 15;

    var g_val = Math.random()*(g_end - g_beg + 1);
    var gen = Math.floor(g_val) + g_beg;

    // simple mod
    var sm_beg = 2;
    var sm_end = 100;
    
    var sm_val = Math.random()*(sm_end - sm_beg + 1);
    var simple_mod = Math.floor(sm_val) + sm_beg;

    // private_number
    var p_beg = 1;
    var p_end = 6;
    
    var p_val = Math.random() / 32767 *(p_end - p_beg + 1);
    var private_number = Math.floor(p_val) + p_beg;

    var res_degree = Math.pow(gen, private_number);
    var res_module = res_degree % simple_mod;

    // send gen, mod to server
    var msg = "DH_proto" + gen + "&" + simple_mod + "&" + res_module;
        $.ajax({
          async: false,
          type: 'POST',
          url: '127.0.0.1',
          data: msg,
          success: function(data) {

            // generate secret key
            var degree = Math.pow(data, private_number);
            var secret_key = String(degree % simple_mod);
            //alert(secret_key);

            var amoundRounds = 16;

            var leftBlock = [];
            var rightBlock = [];
            var arrayRounderKeys = [];
            var encryptLogin;
            var encryptPass;

            // get auth data
            var get_login = String($("input[name='AuthLogin']").val());
            var get_pass = String($("input[name='AuthPass']").val());

            // hashing auth data (SHA512)
            secret_key = hex_sha512(secret_key);
            get_login = hex_sha512(get_login);
            get_pass = hex_sha512(get_pass);
            
            if(get_login.length != 128 || get_pass.length != 128 || secret_key.length != 128)
            {
              alert("Error SHA512, length != 128 symbols or secret key != 128");
            }

            // in Binary auth 
            secret_key = inBinary(secret_key);
            get_login = inBinary(get_login);
            get_pass = inBinary(get_pass);

            //
            subblock(get_login.split(''), leftBlock, rightBlock);

            //
            arrayRounderKeys = arrayKeys(secret_key.split(''), amoundRounds);

            //
            encryptLogin = encryptFeistelNetwork(leftBlock, rightBlock, arrayRounderKeys, amoundRounds);

            //
            leftBlock = [];
            rightBlock = [];

            //
            subblock(get_pass.split(''), leftBlock, rightBlock);

            //
            encryptPass = encryptFeistelNetwork(leftBlock, rightBlock, arrayRounderKeys, amoundRounds);

            // start preloader
            go_preloader();

            // data overwrite
            $('#user_name').val(encryptLogin);
            $('#user_pass').val(encryptPass);

          },
          error:  function(xhr, str){
	        alert('Возникла ошибка: ' + xhr.responseCode);
          }
        });  
}


// DH and encrypt Reg data
function encryptRegData()
{
	// gen  
    var g_beg = 2;
    var g_end = 15;

    var g_val = Math.random()*(g_end - g_beg + 1);
    var gen = Math.floor(g_val) + g_beg;

    // simple mod
    var sm_beg = 2;
    var sm_end = 100;
    
    var sm_val = Math.random()*(sm_end - sm_beg + 1);
    var simple_mod = Math.floor(sm_val) + sm_beg;

    // private_number
    var p_beg = 1;
    var p_end = 6;
    
    var p_val = Math.random() / 32767 *(p_end - p_beg + 1);
    var private_number = Math.floor(p_val) + p_beg;

    var res_degree = Math.pow(gen, private_number);
    var res_module = res_degree % simple_mod;

    // send gen, mod to server
    var msg = "DH_proto" + gen + "&" + simple_mod + "&" + res_module;
        $.ajax({
          async: false,
          type: 'POST',
          url: '127.0.0.1',
          data: msg,
          success: function(data) {

            // generate secret key
            var degree = Math.pow(data, private_number);
            var secret_key = String(degree % simple_mod);
            //alert(secret_key);

            var amoundRounds = 16;

            var leftBlock = [];
            var rightBlock = [];
            var arrayRounderKeys = [];

			var encryptName;
            var encryptLogin;
            var encryptPass;

            // get auth data
			var get_name = String($("input[name='RegName']").val());
            var get_login = String($("input[name='RegLogin']").val());
            var get_pass = String($("input[name='RegPass']").val());

            // hashing key
            secret_key = hex_sha512(secret_key);

			// hashing auth data (SHA512)
			get_name = hex_sha512(get_name);
            get_login = hex_sha512(get_login);
            get_pass = hex_sha512(get_pass);
            
            if(get_login.length != 128 || get_pass.length != 128 || secret_key.length != 128 || get_name.length != 128)
            {
              alert("Error SHA512, length != 128 symbols or secret key != 128");
            }

            // in Binary
            secret_key = inBinary(secret_key);

			get_name = inBinary(get_name);
            get_login = inBinary(get_login);
            get_pass = inBinary(get_pass);

			 // Generate round keys
            arrayRounderKeys = arrayKeys(secret_key.split(''), amoundRounds);

            // Divide into sub-blocks name
            subblock(get_name.split(''), leftBlock, rightBlock);

			// Encrypt name
			encryptName = encryptFeistelNetwork(leftBlock, rightBlock, arrayRounderKeys, amoundRounds);

			// Cleaning temp array
            leftBlock = [];
            rightBlock = [];

			 // Divide into sub-blocks login
            subblock(get_login.split(''), leftBlock, rightBlock);

            // Encrypt login
            encryptLogin = encryptFeistelNetwork(leftBlock, rightBlock, arrayRounderKeys, amoundRounds);

            // Cleaning temp array
            leftBlock = [];
            rightBlock = [];

            // Divide into sub-blocks pass
            subblock(get_pass.split(''), leftBlock, rightBlock);

            // Encrypt pass
            encryptPass = encryptFeistelNetwork(leftBlock, rightBlock, arrayRounderKeys, amoundRounds);

            // start preloader
            go_preloader();

            // data overwrite
            $('#reg_user_name').val(encryptName);
            $('#reg_user_login').val(encryptLogin);
			$('#reg_user_pass').val(encryptPass);

          },
          error:  function(xhr, str){
	        alert('Возникла ошибка: ' + xhr.responseCode);
          }
        });  
}