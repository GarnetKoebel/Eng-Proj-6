function errorMessage1(inputtxt) {
        var error1 = document.getElementById("error1")
        var u = /^\w{7,}$/;
        if (!(inputtxt.value.match(u)))
        {
            // Changing content and color of content
            error1.textContent = "Please enter a valid username"
            error1.style.color = "red"
        } else {
            error1.textContent = ""
        }
    }