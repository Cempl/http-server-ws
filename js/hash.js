
function sha ()
    {
		var login = document.getElementById("write_login").value;
		var password = document.getElementById("write_password").value;
		
		login += password;
		
		login = hex_sha512(login);
		
		var boundary = String(Math.random()).slice(2);
		var boundaryMiddle = '--' + boundary + '\r\n';
		var boundaryLast = '--' + boundary + '--\r\n'
		
		var xhr = new XMLHttpRequest();
		xhr.open('POST', 'chat.html', false);
		
		xhr.setRequestHeader('Content-Type', 'multipart/form-data; boundary=' + boundary, 'Location:', 'http://chat.html');
		
		xhr.send(login);
		
		function get_()
		{
			var x = new XMLHttpRequest();
			x.open("GET", "chat.html", true);
			x.onload = function (){
			}
			x.send(null);
		}
	}