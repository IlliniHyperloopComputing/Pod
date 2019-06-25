# Converts bytes to integer values
def bytes_to_int(bytes, length):
    new_bytes = []
    for x in range(0, length):
        new_bytes.append(int.from_bytes(
            bytes[(x*4):(x*4+4)], byteorder='little', signed=False))
    return new_bytes
