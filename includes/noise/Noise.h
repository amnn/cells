#ifndef NOISE_H
#define NOISE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstring>
#include <cmath>

#include "GL_includes.h"

namespace {
    struct grad_t { double x, y; };

    static constexpr double sqrt2 = sqrt(2.0);

    static const grad_t gradients[] = {
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

namespace Engine {

class Noise {
    template <class T>
    static T
    interpolate(T x, T y, double t)
    {
        double f = 6 * t*t*t*t*t - 15 * t*t*t*t + 10 * t*t*t;
        return x * f + y * (1 - f);
    }

    static void
    octave
    (
        GLsizei               w,
        GLsizei               h,
        std::vector<grad_t> & grads,
        std::vector<GLuint> & buff,
        int                   N,
        int                   sf
    )
    {
        GLsizei gW = w / N + 1;

        for(int y = 0; y < h; ++y)
        for(int x = 0; x < w; ++x)
        {
            GLuint i   = x / N,
                   j   = y / N;

            grad_t g00 = grads[ j       * gW +       i ],
                   g01 = grads[ (j + 1) * gW +       i ],
                   g10 = grads[ j       * gW + (i + 1) ],
                   g11 = grads[ (j + 1) * gW + (i + 1) ];

            double u   = (x % N) / static_cast<double>(N),
                   v   = (y % N) / static_cast<double>(N);

            double n00 = g00.x * u       + g00.y *       v,
                   n01 = g01.x * u       + g01.y * (1 - v),
                   n10 = g10.x * (1 - u) + g10.y *       v,
                   n11 = g11.x * (1 - u) + g11.y * (1 - v),

                   nx0 = interpolate(n10, n00, u),
                   nx1 = interpolate(n11, n01, u);

            buff[y*w + x] += sf * fabs( interpolate(nx1, nx0, v) );
        }
    }

    static unsigned int
    generate_seed() {
        using clock = std::chrono::high_resolution_clock;
        using ms    = std::chrono::milliseconds;

        return static_cast<unsigned int>(
            std::chrono::duration_cast<ms>(
                clock::now().time_since_epoch()
            ).count()
        );
    }

    static std::vector<grad_t>
    gradient_grid(GLsizei w, GLsizei h, unsigned int seed)
    {
        std::default_random_engine         generator(   seed);
        std::uniform_int_distribution<int> distribution(0, 7);

        std::vector<grad_t> grads(w * h);

        for(auto & g : grads)
            g = gradients[distribution(generator)];

        return grads;
    }

public:
    template <int P = 3, int UB = 0>
    static unsigned int
    perlin(
        GLsizei                  w,
        GLsizei                  h,
        std::vector<GLuint> & buff,
        unsigned int          seed = 0
    )
    {
        if(!seed) seed = generate_seed();

        int N      =      1 <<  P;
        int M      = UB ? 1 << UB : 0;

        GLsizei gW = w / N + 1,
                gH = h / N + 2;

        auto grads = gradient_grid(gW, gH, seed);

        buff = std::vector<GLuint>(w * h);

        int sf10 = 10, sf = 1, sum = 0;
        while(N != M && N < w && N < h)
        {
            octave(w, h, grads, buff, N, sf10);

            N    = N    << 1;
            sum +=        sf;
            sf   = sf   << 1;
            sf10 = sf10 << 1;
        }

        for(auto & x : buff) x /= sum;

        return seed;
    }

    template <int P = 3, int UB = 0>
    static unsigned int
    diffuse(
        GLsizei               w,
        GLsizei               h,
        unsigned int          max_ratio,
        std::vector<GLuint> & buff,
        unsigned int          floor = 0,
        unsigned int          seed  = 0
    )
    {
        seed = perlin<P,UB>(w, h, buff, seed);

        unsigned int threshold = std::min(10u, floor + max_ratio);

        std::default_random_engine         gen(seed);

        std::uniform_int_distribution<int> peak_distrib(threshold - 10, max_ratio),
                                           skip_distrib(             0, max_ratio);

        for(int y = 0; y < h; ++y)
        for(int x = 0; x < w; ++x)
        {
            unsigned int i    = y*w + x,
                         peak = buff[i] + peak_distrib(gen);

            buff[i] = !skip_distrib(gen) && peak >= threshold ? buff[i] : 0;
        }

        return seed;
    }
};

}; // namespace Engine

#endif
