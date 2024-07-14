function jsonEncode(data) {
    const customJsonEncoder = (key, value) => {
        if (value instanceof Set) {
            return [...value];
        }
        if (typeof value === 'number' && !isFinite(value)) {
            return value.toString();
        }
        return value;
    };

    return JSON.stringify(data, customJsonEncoder);
}
