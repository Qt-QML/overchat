.pragma library

const OAUTH_URL = "https://www.googleapis.com/oauth2/v4/token";
const USERINFO_URL = "https://www.googleapis.com/oauth2/v1/userinfo";

const CLIENT_ID = "35451728355-c0uv4n0534buliuobqti2u8522261bo6.apps.googleusercontent.com";
const CLIENT_SECRET = "AWRd0wwr0IafsCu0o02c3TS6";
const REDIRECT_URI = "https://overchat-e401f.firebaseapp.com/__/auth/handler";

const FIREBASE_URL = "https://overchat-e401f.firebaseio.com";

function refreshToken(token, cb) {
    var http = new XMLHttpRequest();
    var url = OAUTH_URL +
                "?client_id=" + CLIENT_ID +
                "&client_secret=" + CLIENT_SECRET +
                "&grant_type=refresh_token" +
                "&refresh_token=" + token;

    http.open("POST", url, true);

    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                cb(result["access_token"], result["expires_in"], token);
            } else {
                console.log("refreshToken error: " + http.status, token);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function getUserCredentials(code, cb) {
    var http = new XMLHttpRequest();
    var url = OAUTH_URL +
                "?client_id=" + CLIENT_ID +
                "&client_secret=" + CLIENT_SECRET +
                "&redirect_uri=" + REDIRECT_URI +
                "&grant_type=authorization_code" +
                "&code=" + code;

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
    var url = USERINFO_URL +
            "?access_token=" + credentials["access_token"];

    http.open("GET", url, true);
    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                checkUserRegistration(result, credentials, cb);
            } else {
                console.log("getUserInfo error: " + http.status, credentials);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function checkUserRegistration(user_info, credentials, cb) {
    var http = new XMLHttpRequest();
    var url = FIREBASE_URL + "/users/" + user_info["id"] + ".json?"; // "access_token=" + credentials["access_token"];

    http.open("GET", url, true);
    http.setRequestHeader("Content-type", "application/json");

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                var result = JSON.parse(http.responseText);
                var registered = false;

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
                console.log("checkUserRegistration error: " + http.status, url, JSON.stringify(credentials));
                console.log(http.responseText);
            }
        }
    }
    http.send();
}

function getFileFromStorage(access_token, file_uri, cb) {
    var http = new XMLHttpRequest();

    console.log(access_token);

    http.open("GET", file_uri, true);
    http.setRequestHeader("Content-type", "application/json");
    http.setRequestHeader("Authorization", "Bearer " + access_token);

    http.onreadystatechange = function() { // Call a function when the state changes.
        if (http.readyState == 4) {
            if (http.status == 200) {
                console.log("ok");

                cb(http.responseText);
            } else {
                console.log("error: " + http.status);
                console.log(http.responseText);
            }
        }
    }
    http.send();
}
