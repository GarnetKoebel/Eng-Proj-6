function errorMessage2(inputtxt) {
        var error2 = document.getElementById("error2")
        var p = /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[ `!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?~])[A-Za-z\d `!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?~]{8,}$/;
        if (!(inputtxt.value.match(p)))
        {
            // Changing content and color of content
            error2.textContent = "Password requires eight characters, at least one uppercase letter, one lowercase letter, one number and one special character "
            error2.style.color = "red"
        } else {
            error2.textContent = ""
        }
    }    