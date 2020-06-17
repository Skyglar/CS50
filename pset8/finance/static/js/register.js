        let inputs = document.getElementsByClassName("form-group");
        let errors = document.getElementsByClassName("invalid-feedback");
        let xhttp;

        function CreateRequest() {
            var Request = false;

            if (window.XMLHttpRequest) {
                //Gecko-совместимые браузеры, Safari, Konqueror
                Request = new XMLHttpRequest();
            }
            else if (window.ActiveXObject) {
                //Internet explorer
                try {
                    Request = new ActiveXObject("Microsoft.XMLHTTP");
                }
                catch (CatchException) {
                    Request = new ActiveXObject("Msxml2.XMLHTTP");
                }
            }

            if (!Request) {
                console.log("Cannot create XMLHttpRequest");
            }
            xhttp = Request;
        }

        function ajaxUsernameQuery(username, synh) {

            console.log(username);
            xhttp.onreadystatechange = function() {

                if (this.readyState == 4 && this.status == 200) {
                    // все в порядке, ответ получен
                    console.log("ready. response=" + this.response["text"]);

                    userExistence(this);
                } else {
                    // все еще не готово
                    console.log("not ready");
                }
            };

            xhttp.open("GET", "/api/username?username=" + username, synh);
            xhttp.send();

        }

        function userExistence(xhttp) {
            let username = inputs[0].children[0];

            if(xhttp.readyState == 0) {
                ajaxUsernameQuery(username, false);
            }

            let isUserExists = JSON.parse(xhttp.response)["text"];
            if(isUserExists) {
                errors[0].innerHTML = "user already exists";
                errors[0].style.display = "block";
                username.classList.add("is-invalid");
                return false;
            } else if(errors[0].style.display == "block") {
                errors[0].style.display = 'none';
                username.classList.remove('is-invalid');
            }

            return true;
        }

        function valid() {
            debugger;
            CreateRequest();
            let exists = userExistence(xhttp);
            let username = usernameValid();
            let password = passwordValid();
            return username && password && exists;
        }

        function usernameValid() {
            let username = inputs[0].children[0];

            if(username.value.length == 0) {
                errors[0].innerHTML = "username cannot be empty";
                errors[0].style.display = "block";
                username.classList.add("is-invalid");
                return false;
            } else if(errors[0].style.display == "block") {
                errors[0].style.display = 'none';
                username.classList.remove('is-invalid');
            }

            return true;
        }

        function passwordValid() {
            let password1 = inputs[1].children[0];

            if(password1.value.length == 0) {
                errors[1].innerHTML = "password cannot be empty";
                errors[1].style.display = "block";
                password1.classList.add("is-invalid");

                return false;
            } else if(errors[1].style.display == "block") {
                errors[1].style.display = 'none';
                password1.classList.remove('is-invalid');
            }

            return true;
        }

        function passwordsDifferend() {
            let password1 = inputs[1].children[0];
            let password2 = inputs[2].children[0];

            if(password1.value.length != 0 && password1.value != password2.value) {
                errors[2].innerHTML = "passwords are different";
                errors[2].style.display = "block";
                password2.classList.add("is-invalid");
                return;
            } else if(errors[1].style.display == "block" || errors[2].style.display == "block") {
                errors[2].style.display = 'none';
                password2.classList.remove('is-invalid');
            }

        }