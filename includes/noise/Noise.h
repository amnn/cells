#ifndef NOISE_H
#define NOISE_H

#include <iostream>
#include <chrono>
#include <random>
#include <cstring>
#include <cmath>

#include "GL_includes.h"

namespace {

    struct grad_t {
        double x, y;
    };

    static constexpr double sqrt2 = sqrt( 2.0 );

    static const grad_t gradients[]  = {
        {  1.0,      0.0 },
        {  sqrt2,  sqrt2 },
        {  0.0,      1.0 },
        { -sqrt2,  sqrt2 },
        { -1.0,      0.0 },
        { -sqrt2, -sqrt2 },
        {  0.0,     -1.0 },
        {  sqrt2, -sqrt2 }
    };

};

namespace engine {

class Noise {

    template <class T>
    static T interpolate( T x, T y, double t )
    {
        double f = 6 * t*t*t*t*t - 15 * t*t*t*t + 10 * t*t*t;
        return    x * f + y * (1 - f );
    }

    static void
    octave
    (

        GLsizei w,
        GLsizei h,
        grad_t *grads,
        GLuint *buff,
        int N,
        int sf

    )
    {
        GLsizei gW = w / N + 1;

        for( int y = 0; y < h; ++y )
        for( int x = 0; x < w; ++x )
        {
            GLuint i   = x / N,
                   j   = y / N;

            grad_t g00 = grads[ j       * gW +       i ],
                   g01 = grads[ (j + 1) * gW +       i ],
                   g10 = grads[ j       * gW + (i + 1) ],
                   g11 = grads[ (j + 1) * gW + (i + 1) ];

            double u   = ( x % N ) / static_cast<double>( N ),
                   v   = ( y % N ) / static_cast<double>( N );

            double n00 = g00.x * u       + g00.y *       v,
                   n01 = g01.x * u       + g01.y * (1 - v),
                   n10 = g10.x * (1 - u) + g10.y *       v,
                   n11 = g11.x * (1 - u) + g11.y * (1 - v),
                   nx0 = interpolate( n10, n00, u ),
                   nx1 = interpolate( n11, n01, u );

            buff[ y*w + x ] += sf * fabs( interpolate( nx1, nx0, v ) );

        }

    }

public:

    template < int P = 3, int UB = 0 >
    static unsigned int
    perlin( GLsizei w, GLsizei h, GLuint *buff, unsigned int seed = 0 )
    {
        memset( buff, 0, w * h * sizeof( GLuint ) );

        using clock = std::chrono::high_resolution_clock;
        using ms    = std::chrono::milliseconds;

        if( !seed )
            seed = static_cast<unsigned int>(

                std::chrono::duration_cast<ms>(
                    clock::now().time_since_epoch()
                ).count()

            );


        int     N  =      1 <<  P;
        int     M  = UB ? 1 << UB : 0;

        GLsizei gW = w / N + 1,
                gH = h / N + 2;

        std::default_random_engine         generator(    seed );
        std::uniform_int_distribution<int> distribution( 0, 7 );

        auto grads = new grad_t[ gW * gH ];

        for( int y = 0; y < gH; ++y )
        for( int x = 0; x < gW; ++x )
            grads[ y * gW + x ] = gradients[ distribution( generator ) ];

        int sf10 = 10, sf = 1, sum = 0;
        while( N != M && N < w && N < h )
        {
            octave( w, h, grads, buff, N, sf10 );

            N    = N    << 1;
            sum +=        sf;
            sf   = sf   << 1;
            sf10 = sf10 << 1;

        }

        for( int y = 0; y < h; ++y )
        for( int x = 0; x < w; ++x )
            buff[ y*w + x ] = buff[ y*w + x ] / sum;

        delete[] grads;

        return seed;
    }

};

}; // namespace engine

#endif
