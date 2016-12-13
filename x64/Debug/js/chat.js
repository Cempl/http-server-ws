var beta_socket = new WebSocket ("ws://127.0.0.1:27015/ws");
var user_name = sessionStorage.getItem("u_name");

document.getElementById('name_in_chat').innerHTML = "Your name in chat: " + user_name;

// send masg from form
document.forms.message_test.onsubmit = function ()
{
  var outgoingMessage = this.message.value;

  beta_socket.send(outgoingMessage);
  
  document.forms.message_test.reset();
  
  return false;
};

beta_socket.onopen = function()
{
  document.getElementById('connection').innerHTML = "Connect: Yes";
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

// hotkeys
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