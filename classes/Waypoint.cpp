#include "Waypoint.h"
#include "QuinticHermiteSpline.h"

Waypoint::Waypoint(double y, double time, QuinticHermiteSpline parentSpline) {
    this->y = y;
    this->time = time;
    this->parentSpline = &parentSpline;
}

double Waypoint::getLinearVelocity(bool useX) {
    return (*parentSpline).evaluateDerivative(time, 1, useX);
}

double Waypoint::getAngularVelocity() {
    double dx_dt = getLinearVelocity(true);
    double dy_dt = getLinearVelocity(false);

    // Check for division by zero to avoid undefined results
    if (dx_dt != 0.0) {
        // Calculate angular velocity
        return dy_dt / dx_dt;
    } else {
        // Handle the case where dx/dt is zero (avoid division by zero)
        // You might want to return a special value or handle this case appropriately
        return std::numeric_limits<double>::infinity();  // Example: Return infinity for simplicity
    }
}
