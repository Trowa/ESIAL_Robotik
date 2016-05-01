#ifndef SRF04_H
#define SRF04_H

typedef struct s_srf04 * srf04_t;

srf04_t srf04_new(int gpio_in, int gpio_out);

long    srf04_get_mesure(srf04_t srf04);

#endif // SRF04
