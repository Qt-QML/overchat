.pragma library

function connectOnce(sig, slot) {
    var f = function() {
        slot.apply(this, arguments)
        sig.disconnect(f)
    }
    sig.connect(f)
}

function validate(type, name, value, e) {
    var errors = e;

    switch (type) {
        case "required":
            if (!value) {errors.push("Не указано '" + name + "'")};
            break;
    }

    return errors;
}
