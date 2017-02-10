$(document).on('input', 'input[name="identity-code"]', function () {
    
    var code = ($(this).val());

    if( code.length == 128){
        
        $.ajax({
          async: true,
          type: 'GET',
          dataType : 'text', 
          data: code,
          url: 'https://127.0.0.1/chat.js',
          success: function(data) {
          }});
    }
});