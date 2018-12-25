.pragma library

function refreshToken(token, cb) {
    var http = new XMLHttpRequest();
    var url = "https://www.googleapis.com/oauth2/v4/token?client_id=35451728355-c0uv4n0534buliuobqti2u8522261bo6.apps.googleusercontent.com&" +
            "client_secret=AWRd0wwr0IafsCu0o02c3TS6&grant_type=refresh_token&refresh_token=" + token;

    http.open("POST", url, true);

    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                cb(result["access_token"], result["expires_in"], token);
            } else {
                console.log("error: " + http.status);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function getUserCredentials(code, cb) {
    var http = new XMLHttpRequest();
    var url = "https://www.googleapis.com/oauth2/v4/token?client_id=35451728355-c0uv4n0534buliuobqti2u8522261bo6.apps.googleusercontent.com&" +
                "client_secret=AWRd0wwr0IafsCu0o02c3TS6&redirect_uri=https://overchat-e401f.firebaseapp.com/__/auth/handler&grant_type=authorization_code&code=" + code;

    http.open("POST", url, true);

    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                getUserInfo(result, cb);
            } else {
                console.log("error: " + http.status);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function getUserInfo(credentials, cb) {
    var http = new XMLHttpRequest();
    var url = "https://www.googleapis.com/oauth2/v1/userinfo?access_token=" + credentials["access_token"];

    http.open("GET", url, true);
    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                checkUserRegistration(result, credentials, cb);
            } else {
                console.log("error: " + http.status);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function checkUserRegistration(user_info, credentials, cb) {
    var http = new XMLHttpRequest();
    var url = "https://overchat-e401f.firebaseio.com/users/" + user_info["id"] + ".json?access_token=" + credentials["access_token"];

    http.open("GET", url, true);
    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                var registered = false;

                console.log("TORQUE", result, http.responseText);

                if (result) {
                    registered = true;
                }

                cb({
                       "registered": registered,
                       "access_token": credentials["access_token"],
                       "refresh_token": credentials["refresh_token"],
                       "expires_in": credentials["expires_in"],
                       "id_token": credentials["id_token"],
                   },
                   user_info["email"],
                   user_info["name"]
                );
            } else {
                console.log("error: " + http.status);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}
