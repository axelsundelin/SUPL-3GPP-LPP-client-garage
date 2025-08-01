#pragma once
#include <core/core.hpp>
#include <maths/float3.hpp>
#include <time/tai.hpp>

#include <string>

#ifndef DATA_TRACING
#error "DATA_TRACING is not defined"
#endif

namespace datatrace {

template <typename T>
struct Option {
    T    value;
    bool valid;

    Option() : valid(false) {}
    Option(T const& new_value) : value(new_value), valid(true) {}
};

void initialize(std::string const& device, std::string const& server, int port,
                std::string const& username, std::string const& password, bool reliable);
void finalize();

void set_ssr_data(bool enable);

struct Satellite {
    Option<Float3> position;
    Option<Float3> velocity;
    Option<double> elevation;
    Option<double> azimuth;
    Option<double> nadir;
    Option<long>   iod;
    Option<Float3> eph_position;
    Option<Float3> ground_position_ecef;
    Option<Float3> ground_position_llh;
    Option<double> reception_time;
    Option<double> emission_time;
    Option<Float3> sun_position;
    Option<Float3> moon_position;
    Option<double> gmst;
};

struct Observation {
    Option<double> frequency;
    Option<double> geo_range;
    Option<double> eph_range;
    Option<double> eph_relativistic_correction;
    Option<double> sat_clock;
    Option<double> orbit;
    Option<double> clock;
    Option<double> code_bias;
    Option<double> phase_bias;
    Option<double> stec;
    Option<double> stec_grid;
    Option<double> stec_poly;
    Option<double> tropo;
    Option<double> tropo_dry;
    Option<double> tropo_wet;
    Option<double> tropo_dry_mapping;
    Option<double> tropo_wet_mapping;
    Option<double> tropo_dry_height_correction;
    Option<double> tropo_wet_height_correction;
    Option<double> tropo_model_dry;
    Option<double> tropo_model_wet;
    Option<double> shapiro;
    Option<double> earth_solid_tides;
    Option<double> phase_windup;
    Option<double> phase_windup_velocity;
    Option<double> phase_windup_angle;
    Option<double> antenna_phase_variation;
    Option<double> code_range;
    Option<double> phase_range;
    Option<double> phase_range_rate;
    Option<double> carrier_to_noise_ratio;
    Option<double> phase_lock_time;
    Option<Float3> orbit_radial_axis;
    Option<Float3> orbit_cross_axis;
    Option<Float3> orbit_along_axis;
    Option<double> orbit_delta_t;
    Option<long>   eph_iod;
    Option<double> vtec_mapping;
    Option<double> stec_height_correction;
};

void report_satellite(ts::Tai const& time, std::string const& satellite, Satellite const& sat);
void report_observation(ts::Tai const& time, std::string const& satellite,
                        std::string const& signal, Observation const& obs);

void report_ssr_orbit_correction(ts::Tai const& time, std::string const& satellite,
                                 Option<Float3> delta, Option<Float3> dot_delta,
                                 Option<long> ssr_iod, Option<long> eph_iod);
void report_ssr_clock_correction(ts::Tai const& time, std::string const& satellite,
                                 Option<double> c0, Option<double> c1, Option<double> c2,
                                 Option<long> ssr_iod);
void report_ssr_ionospheric_polynomial(ts::Tai const& time, std::string const& satellite,
                                       Option<double> c00, Option<double> c01, Option<double> c10,
                                       Option<double> c11, Option<double> reference_point_latitude,
                                       Option<double> reference_point_longitude,
                                       Option<double> stec_quality_indicator,
                                       Option<long>   stec_quality_indicator_cls,
                                       Option<long>   stec_quality_indicator_val,
                                       Option<long>   ssr_iod);
void report_ssr_tropospheric_grid(ts::Tai const& time, long grid_point_id,
                                  Option<Float3> position_llh, Option<double> tropo_wet,
                                  Option<double> tropo_dry, Option<long> ssr_iod);
void report_ssr_ionospheric_grid(ts::Tai const& time, long grid_point_id,
                                 Option<Float3> position_llh, std::string const& satellite,
                                 Option<double> residual, Option<long> ssr_iod);

}  // namespace datatrace
