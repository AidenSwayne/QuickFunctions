function validatePassword(password) {
    /*
    Validates a password based on the following requirements:
    - Must be between 8 and 64 characters long.
    - Must contain at least one uppercase letter.
    - Must contain at least one lowercase letter.
    - Must contain at least one digit.
    - Must contain at least one special character (`~!@#$%^&*()-_=+[]{}\|;:'",<.>/?).
    Returns true if the password meets all requirements, false otherwise.
    */
    if (password.length < 8 || password.length > 64) {
        return false;
    }
    
    
    let hasUppercase = false;
    let hasLowercase = false;
    let hasDigit = false;
    let hasSpecial = false;
    
    const specialChars = "`~!@#$%^&*()-_=+[]{}\|;:'\",./<>?";
    
    for (let char of password) {
        if (char >= 'A' && char <= 'Z') {
            hasUppercase = true;
        } else if (char >= 'a' && char <= 'z') {
            hasLowercase = true;
        } else if (char >= '0' && char <= '9') {
            hasDigit = true;
        } else if (specialChars.includes(char)) {
            hasSpecial = true;
        }
    }

    return hasUppercase && hasLowercase && hasDigit && hasSpecial;
}
