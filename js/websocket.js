var beta_socket = new WebSocket ("ws://127.0.0.1:27015/ws");

// отправить сообщение из формы publish
document.forms.message_test.onsubmit = function ()
{
  var outgoingMessage = this.message.value;

  beta_socket.send(outgoingMessage);
  
  document.forms.message_test.reset();
  
  return false;
};

beta_socket.onopen = function()
{
  document.getElementById('input-message').innerHTML += "Соединение установлено" + "\n";
};

beta_socket.onclose = function(event)
{
  if (event.wasClean)
  {
    var ConnectClose = "Соединение закрыто чисто" + "\n";
    document.getElementById('input-message').innerHTML += "Соединение закрыто чисто" + "\n";
  }
  else
  {
    document.getElementById('input-message').innerHTML += "Обрыв соединения" + "\n";
  }
  var codeError = "Код: " + event.code + event.reason + "\n";
  document.getElementById('input-message').innerHTML += codeError;
};

beta_socket.onmessage = function(event)
{
  var textarea = document.getElementById('input-message'); 
  document.getElementById('input-message').innerHTML += event.data + "\n";
  textarea.scrollTop = textarea.scrollHeight;
};

beta_socket.onerror = function(error)
{
  document.getElementById('input-message').innerHTML += "Ошибка " + error.message + "\n";
};