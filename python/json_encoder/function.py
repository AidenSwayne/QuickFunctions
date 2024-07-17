def json_encode(data: any) -> str:
    import json

    def custom_json_encoder(obj):
        if isinstance(obj, set):
            return list(obj)
        elif isinstance(obj, complex):
            return [obj.real, obj.imag]
        return obj

    return json.dumps(data, default=custom_json_encoder)
