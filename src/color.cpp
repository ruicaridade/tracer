#include "color.h"
#include <iostream>
#include "external\TinyPngOut.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
: r(r), g(g), b(b), a(a) { }

Color operator*(const Color& color, float scalar)
{
    return Color((unsigned char)(color.r * scalar),
        (unsigned char)(color.g * scalar),
        (unsigned char)(color.b * scalar),
        (unsigned char)(color.a * scalar));
}

Color operator*(float scalar, const Color& color)
{
    return Color((unsigned char)(color.r * scalar),
        (unsigned char)(color.g * scalar),
        (unsigned char)(color.b * scalar),
        (unsigned char)(color.a * scalar));
}

Color operator*(const Color& color, const Vector3& vector)
{
    return Color(color.r * vector.x,
        color.g * vector.y,
        color.b * vector.z,
        color.a);
}

Color operator*(const Vector3& vector, const Color& color)
{
    return Color(color.r * vector.x,
        color.g * vector.y,
        color.b * vector.z,
        color.a);
}

Color Color::fromVector(const Vector3& vector)
{
    return Color(255.0f * vector.x,
        255.0f * vector.y,
        255.0f * vector.z,
        255.0f);
}

Vector3 Color::vector() const
{
    return Vector3(r / 255.0f, 
        g / 255.0f,
        b / 255.0f);
}

void Color::saveToPng(const std::string& filename, const std::vector<std::vector<Color>>& data)
{
    if (data.size() == 0)
        return;
    
    unsigned int height = data.size();
    unsigned int width = data[0].size();

    std::vector<uint8_t> image;

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            image.push_back(data[j][i].r);
            image.push_back(data[j][i].g);
            image.push_back(data[j][i].b);
        }
    }

    FILE* file = fopen(filename.c_str(), "wb");
    if (file == nullptr)
    {
        std::cout << "Couldn't open file stream " << filename << std::endl;
        return;
    }
    
    struct TinyPngOut png;
    if (TinyPngOut_init(&png, file, width, height) != TINYPNGOUT_OK)
    {
        fclose(file);
        std::cout << "Couldn't load png file " << filename << std::endl;
        return;
    }

    if (TinyPngOut_write(&png, image.data(), width * height) != TINYPNGOUT_OK)
    {
        fclose(file);
        std::cout << "Couldn't write to png file " << filename << std::endl;
        return;
    }

    if (TinyPngOut_write(&png, nullptr, 0) != TINYPNGOUT_DONE)
    {
        std::cout << "File might be corrupted " << filename << std::endl;
    }

    fclose(file);
}