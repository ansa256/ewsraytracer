#ifndef FALLOFFFILTER_H
#define FALLOFFFILTER_H

class FalloffFilter {

public:
   virtual double filter(double f) const = 0;
};

#endif // FALLOFFFILTER_H
