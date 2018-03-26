#include "Trans.h"
#include <math.h>

#define PI 3.14159265

std::vector<std::vector<float> > Trans::translating_matrix(Vector2z vector) {
    std::vector<std::vector<float> > transform_matrix;

    transform_matrix = {{1, 0, 0},
    {0, 1, 0},
    {vector.getX(), vector.getY(), 1}
    };

    return transform_matrix;
}

std::vector<std::vector<float> > Trans::scaling_matrix(Vector2z vector, Vector2z point) {
    std::vector<std::vector<float> > transform_matrix;

    transform_matrix = {{vector.getX(), 0, 0},
    {0, vector.getY(), 0},
    {point.getX() * (-vector.getX() + 1), point.getY() * (-vector.getY() + 1), 1}
    };  // matriz considerando a translação de ida e volta

    return transform_matrix;
}

std::vector<std::vector<float> > Trans::rotating_matrix(float angle, Vector2z point) {
    std::vector<std::vector<float> > transform_matrix;

    float cos_ang = cos(angle * PI/180);
    float sin_ang = sin(angle * PI/180);

    transform_matrix = {{cos_ang, -sin_ang, 0},
    {sin_ang, cos_ang, 0},
    {-point.getY() * sin_ang + point.getX() * (1 - cos_ang), point.getX() * sin_ang + point.getY() * (1 - cos_ang), 1}
    };  // matriz considerando a translação de ida e volta

    return transform_matrix;
}

void Trans::translate(Figure* figure, Vector2z vector) {
    auto trans_matrix = translating_matrix(vector);
    figure->transform(trans_matrix);  // o translate nao precisa daquelas mutiplicação de matriz, é só mover a figura msm
}

void Trans::scale(Figure* figure, Vector2z vector) {
    auto it_coords = figure->coords.begin();

    Vector2z geo_middle = Vector2z(0,0);
    for (; it_coords != figure->coords.end(); ++it_coords) {
        geo_middle = geo_middle + *it_coords;
    }
    geo_middle = geo_middle / figure->coords.size();  // meio geométrico calculado

    auto scal_matrix = scaling_matrix(vector, geo_middle);
    
    figure->transform(scal_matrix);
}

void Trans::rotate_default(Figure* figure, float angle) {
    auto it_coords = figure->coords.begin();

    Vector2z geo_middle = Vector2z(0,0);
    for (; it_coords != figure->coords.end(); ++it_coords) {
        geo_middle = geo_middle + *it_coords;
    }
    geo_middle = geo_middle / figure->coords.size();  // meio geométrico calculado

    auto rot_matrix = rotating_matrix(angle, geo_middle);

    figure->transform(rot_matrix);  // aplica a transformação na figura com a matriz resultante
}

void Trans::rotate_by_point(Figure* figure, float angle, Vector2z vector){
    auto it_coords = figure->coords.begin();

    float cos_ang = cos(angle * PI/180);
    float sin_ang = sin(angle * PI/180);
    
    auto rot_matrix = rotating_matrix(angle, vector);

    figure->transform(rot_matrix);
}
