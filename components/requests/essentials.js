const API_KEY = "AIzaSyAnxv6M_ALnWIzO4LpSsAFukER50gb3Umw";


function validate(type, name, value, e) {
    var errors = e;

    switch (type) {
        case "required":
            if (!value) {errors.push("Не указано '" + name + "'")};
            break;
    }

    return errors;
}
