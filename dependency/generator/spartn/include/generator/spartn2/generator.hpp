#pragma once
#include <core/core.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

struct LPP_Message;
struct ProvideAssistanceData_r9_IEs;

namespace generator {
namespace spartn {

/// SPARTN message
class Message {
public:
    EXPLICIT Message(uint8_t message_type, uint8_t message_subtype, uint32_t message_time,
                            std::vector<uint8_t>&& payload);

    /// Message type
    uint8_t message_type() const { return mMessageType; }
    /// Message subtype
    uint8_t message_subtype() const { return mMessageSubtype; }
    /// Message data
    std::vector<uint8_t> const& payload() const { return mPayload; }

    std::vector<uint8_t> build();

private:
    uint8_t              mMessageType;
    uint8_t              mMessageSubtype;
    uint32_t             mMessageTime;
    std::vector<uint8_t> mPayload;
};

struct CorrectionPointSet;
struct CorrectionData;

enum class StecMethod {
    Default,
    DiscardC01C10C11,
    MoveToResiduals,
};

/// Generates SPARTN messages based on LPP SSR messages.
class Generator {
public:
    /// Constructor.
    Generator();

    /// Destructor.
    ~Generator();

    void set_ura_override(int ura_override) { mUraOverride = ura_override; }
    void set_ura_default(int ura_default) { mUraDefault = ura_default; }

    void set_continuity_indicator(double continuity_indicator) {
        mContinuityIndicator = continuity_indicator;
    }

    void set_ublox_clock_correction(bool ublox_clock_correction) {
        mUBloxClockCorrection = ublox_clock_correction;
    }

    void set_compute_average_zenith_delay(bool compute_average_zenith_delay) {
        mComputeAverageZenithDelay = compute_average_zenith_delay;
    }

    void set_iode_shift(bool iode_shift) { mIodeShift = iode_shift; }
    void set_sf055_override(int sf055_override) { mSf055Override = sf055_override; }
    void set_sf055_default(int sf055_default) { mSf055Default = sf055_default; }

    void set_sf042_override(int sf042_override) { mSf042Override = sf042_override; }
    void set_sf042_default(int sf042_default) { mSf042Default = sf042_default; }

    void set_increasing_siou(bool increasing_siou) { mIncreasingSiou = increasing_siou; }
    void set_filter_by_residuals(bool filter_by_residuals) {
        mFilterByResiduals = filter_by_residuals;
    }
    void set_filter_by_ocb(bool filter_by_ocb) { mFilterByOcb = filter_by_ocb; }
    void set_ignore_l2l(bool ignore_l2l) { mIgnoreL2L = ignore_l2l; }
    void set_stec_invalid_to_zero(bool stec_invalid_to_zero) {
        mStecInvalidToZero = stec_invalid_to_zero;
    }
    void set_sign_flip_c00(bool sign_flip_c00) { mSignFlipC00 = sign_flip_c00; }
    void set_sign_flip_c01(bool sign_flip_c01) { mSignFlipC01 = sign_flip_c01; }
    void set_sign_flip_c10(bool sign_flip_c10) { mSignFlipC10 = sign_flip_c10; }
    void set_sign_flip_c11(bool sign_flip_c11) { mSignFlipC11 = sign_flip_c11; }
    void set_sign_flip_stec_residuals(bool sign_flip_stec_residuals) {
        mSignFlipStecResiduals = sign_flip_stec_residuals;
    }

    void set_generate_ocb(bool generate_ocb) { mGenerateOcb = generate_ocb; }
    void set_generate_hpac(bool generate_hpac) { mGenerateHpac = generate_hpac; }
    void set_generate_gad(bool generate_gad) { mGenerateGad = generate_gad; }

    void set_gps_supported(bool gps_supported) { mGpsSupported = gps_supported; }
    void set_glonass_supported(bool glonass_supported) { mGlonassSupported = glonass_supported; }
    void set_galileo_supported(bool galileo_supported) { mGalileoSupported = galileo_supported; }
    void set_beidou_supported(bool beidou_supported) { mBeidouSupported = beidou_supported; }

    void set_code_bias_translate(bool value) { mCodeBiasTranslate = value; }
    void set_code_bias_correction_shift(bool value) { mCodeBiasCorrectionShift = value; }
    void set_phase_bias_translate(bool value) { mPhaseBiasTranslate = value; }
    void set_phase_bias_correction_shift(bool value) { mPhaseBiasCorrectionShift = value; }
    void set_hydrostatic_in_zenith(bool value) { mHydrostaticResidualInZenith = value; }
    void set_stec_method(StecMethod method) { mStecMethod = method; }
    void set_stec_transform(bool value) { mStecTranform = value; }
    void set_flip_grid_bitmask(bool flip_grid_bitmask) { mFlipGridBitmask = flip_grid_bitmask; }
    void set_flip_orbit_correction(bool flip_orbit_correction) {
        mFlipOrbitCorrection = flip_orbit_correction;
    }

    /// Generate SPARTN messages based on LPP SSR messages.
    /// @param[in] lpp_message The LPP SSR message.
    /// @return The generated SPARTN messages.
    std::vector<Message> generate(LPP_Message const* lpp_message);

private:
    void find_correction_point_set(ProvideAssistanceData_r9_IEs const* message);
    void find_ocb_corrections(ProvideAssistanceData_r9_IEs const* message);
    void find_hpac_corrections(ProvideAssistanceData_r9_IEs const* message);
    void find_rti_corrections(ProvideAssistanceData_r9_IEs const* message);

    void generate_gad(uint16_t iod, uint32_t epoch_time, uint16_t set_id);
    void generate_ocb(uint16_t iod);
    void generate_hpac(uint16_t iod);

    uint16_t next_area_id() {
        auto id     = mNextAreaId;
        mNextAreaId = (mNextAreaId + 1) % 256;
        return id;
    }

private:
    uint32_t mGenerationIndex;
    uint16_t mNextAreaId;

    std::unordered_map<uint16_t, std::unique_ptr<CorrectionPointSet>> mCorrectionPointSets;
    std::unique_ptr<CorrectionData>                                   mCorrectionData;
    std::vector<Message>                                              mMessages;

    int    mUraOverride;  // <0 = no override
    int    mUraDefault;
    double mContinuityIndicator;  // <0 = no override
    bool   mUBloxClockCorrection;

    int mSf055Override;  // <0 = no override
    int mSf055Default;
    int mSf042Override;  // <0 = no override
    int mSf042Default;

    bool       mComputeAverageZenithDelay;
    bool       mGroupByEpochTime;
    bool       mIodeShift;
    bool       mIncreasingSiou;
    uint16_t   mSiouIndex;
    bool       mCodeBiasTranslate;
    bool       mCodeBiasCorrectionShift;
    bool       mPhaseBiasTranslate;
    bool       mPhaseBiasCorrectionShift;
    bool       mHydrostaticResidualInZenith;
    StecMethod mStecMethod;
    bool       mStecTranform;
    bool       mFlipGridBitmask;

    bool mFilterByResiduals;
    bool mFilterByOcb;
    bool mIgnoreL2L;
    bool mStecInvalidToZero;
    bool mSignFlipC00;
    bool mSignFlipC01;
    bool mSignFlipC10;
    bool mSignFlipC11;
    bool mSignFlipStecResiduals;
    bool mFlipOrbitCorrection;

    bool mGenerateGad;
    bool mGenerateOcb;
    bool mGenerateHpac;
    bool mGpsSupported;
    bool mGlonassSupported;
    bool mGalileoSupported;
    bool mBeidouSupported;
};

}  // namespace spartn
}  // namespace generator
