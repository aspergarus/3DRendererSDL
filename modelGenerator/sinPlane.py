import math

def generate_sin_plane(width, length):
    vertices = []
    faces = []

    frequency = 3
    amplitude = 4


    for x in range(width):
        for z in range(length):
            # y = amplitude * math.sin(frequency * x / width * 2 * math.pi) * math.sin(frequency * z / length * 2 * math.pi)
            y = amplitude * math.sin(frequency * 2 * math.pi * x / width) * amplitude * math.cos(frequency * 2 * math.pi * z / length)
            # newX = (x / width) * 2.0 - 1.0
            # newZ = (z / length) * 2.0 - 1.0 
            newX = x
            newZ = z
            vertices.append((newX, y, newZ))

    for x in range(width - 1):
        for z in range(length - 1):
            v1 = x * length + z
            v2 = v1 + 1
            v3 = (x + 1) * length + z
            v4 = v3 + 1
            faces.append((v1, v2, v4))
            faces.append((v1, v4, v3))

    return vertices, faces

def save_to_obj(filename, vertices, faces):
    with open(filename, 'w') as f:
        for v in vertices:
            f.write("v {} {} {}\n".format(v[0], v[1], v[2]))
        
        for face in faces:
            f.write("f {} {} {}\n".format(face[0] + 1, face[1] + 1, face[2] + 1))

if __name__ == "__main__":
    # Parameters for the sinusoidal plane
    width = 100
    length = 100

    # Generate the sinusoidal plane
    vertices, faces = generate_sin_plane(width, length)

    # Save it to an OBJ file
    save_to_obj("sinusoidal_plane.obj", vertices, faces)
