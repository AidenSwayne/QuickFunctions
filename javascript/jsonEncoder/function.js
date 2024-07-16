function jsonEncode(data) {
    function customJsonEncoder(key, value) {
        if (value instanceof Set) {
            return Array.from(value);
        } else if (typeof value === 'number' && !isFinite(value)) {
            if (value === Infinity) return "Infinity";
            if (value === -Infinity) return "-Infinity";
            return "NaN";
        }
        return value;
    }

    return JSON.stringify(data, customJsonEncoder);
}
