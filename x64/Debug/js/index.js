// get user name for show in chat (save in storage)
function get_Name_Auth_Form()
{
    var tmp_user_name = document.getElementById("user_name").value;

    sessionStorage.setItem("u_name", tmp_user_name);
}

// get user name for show in chat
function get_Name_Reg_Form()
{
    var tmp_user_name = document.getElementById("reg_user_name").value;

    sessionStorage.setItem("u_name", tmp_user_name);
}

document.getElementById("send_login_data").onclick = get_Name_Auth_Form;
document.getElementById("send_reg_data").onclick = get_Name_Reg_Form;