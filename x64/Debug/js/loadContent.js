var beta_socket = new WebSocket ("wss://127.0.0.1/wss");

function showContent(link)
{
    var contentContx;

    if (link == 'home.html')
    {
        contentContx = 'homeBody';
    }
    if (link == 'authorization.html')
    {
        contentContx = 'chatBody';
    }
    if (link == 'reloadFullHome')
    {
        contentContx = 'chatBody';
        link = 'FullHome.html';
    }

        var cont = document.getElementById(contentContx);   
        var http = createRequestObject();  
        if( http )   
        {  
            http.open('get', link);  
            http.onreadystatechange = function ()   
            {  
                if(http.readyState == 4)   
                {  
                    cont.innerHTML = http.responseText;  
                }  
            }  
            http.send(null); 
        }  
        else   
        {  
            document.location = link;  
        }

        /*if (link == 'authorization.html')
        {
            startWebsocket();
        } 
        if (link == 'chat.html')
        {
            
        }*/
}

// create ajax object  
function createRequestObject()   
{  
  try { return new XMLHttpRequest() }  
    catch(e)   
     {  
       try { return new ActiveXObject('Msxml2.XMLHTTP') }  
         catch(e)   
         {  
           try { return new ActiveXObject('Microsoft.XMLHTTP') }  
           catch(e) { return null; }  
         }  
    }  
}


// load websocket
function startWebsocket () 
{ 
var data;
    $.ajax({
          async: true,
          type: 'GET',
          dataType : 'text', 
          data: data,
          url: 'https://127.0.0.1/chat.js',
          success: function(data) {
              var y = document.createElement('script');
              y.defer = true;
              y.text = data;
              document.body.appendChild(y);
          }});
    var y = document.createElement('script');
    y.defer = true;
    y.text = data;
    document.body.appendChild(y);
} 

function sendAuthData()
{
    var flagSignin = false;

    var gLogin = String($("input[name='AuthLogin']").val());
    var gPassword = String($("input[name='AuthPass']").val());

    gLogin = 'Login[' + hex_sha512(gLogin) + ']';
    gPassword = 'Password[' + hex_sha512(gPassword) + ']';

    // generate token
    var t_token ='';
    while(t_token.length < 30)
        t_token += String.fromCharCode(Math.random() *127).replace(/\W|\d|_/g,'');
    var Token = 'Token[' + t_token + ']';

    if(flagSignin == false)
    {
        beta_socket.send(gLogin + gPassword);
        beta_socket.onmessage = function(event)
        {
            if(event.data != "error")
            {
                var contOne = document.getElementById('authorization');
                contOne.innerHTML = event.data;   
                flagSignin = true;
                validProc();
            }
            else
            {
                flagSignin = false;
                document.getElementById('user_name').style.backgroundColor = '#FF4500';
                document.getElementById('user_pass').style.backgroundColor = '#FF4500';
            }
        };
    }
} 

function validProc()
{
    var user_name = sessionStorage.getItem("u_name");

    document.getElementById('name_in_chat').innerHTML = "Your name in chat: " + user_name;
    document.getElementById('connection').innerHTML = "Connect: Yes";

    document.forms.message_test.onsubmit = function ()
    {
            var outgoingMessage = this.message.value;

            var flag = 0;

            for (var i = 0; i < outgoingMessage.length; i++)
            { 
                if (outgoingMessage.charAt(i)!=' ')flag=1;
            }
            if (flag == 0) 
            {
                return false;
            }
            else
            {
                beta_socket.send(outgoingMessage);
                
                document.forms.message_test.reset();
                
                return false;
            }
    };

    beta_socket.onopen = function()
    {
        //document.getElementById('connection').innerHTML = "Connect: Yes";
    };

    beta_socket.onclose = function(event)
    {
    if (event.wasClean)
    {
        var ConnectClose = "Соединение закрыто чисто";
        document.getElementById('connection').innerHTML = "Соединение закрыто чисто";
    }
    else
    {
        document.getElementById('connection').innerHTML = "Connect: Not";
    }
    var codeError = "Error (code): " + event.code + event.reason;
    document.getElementById('errors_ws').innerHTML = codeError;
    };

    beta_socket.onmessage = function(event)
    {
    var textarea = document.getElementById('input-message'); 

    document.getElementById('input-message').innerHTML += user_name + ": " + event.data + "\n";

    textarea.scrollTop = textarea.scrollHeight;
    };

    beta_socket.onerror = function(error)
    {
    document.getElementById('errors_ws').innerHTML = "Error: " + error.message;
    };

    $('textarea').bind('keypress', function(e) {
    if (e.keyCode == 13 && !e.shiftKey) {
        $(this).parents('form').submit();
    }

    if (e.keyCode == 13 && !e.shiftKey)
        {
            e.preventDefault();
            return true;
        }
    });
}