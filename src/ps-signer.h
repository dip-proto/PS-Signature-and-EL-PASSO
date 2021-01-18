#ifndef PS_SRC_PS_SIGNER_H_
#define PS_SRC_PS_SIGNER_H_

#include "ps-encoding.h"

using namespace mcl::bls12;

/**
 * The PS Signer to sign over committed single message or messages.
 */
class PSSigner {
public:
  /**
   * @brief Construct a new PSSigner object
   *
   * The generators for G1 and G2 will be randomly selected.
   *
   * @param attribute_num input The maximum number of attributes supported by the system.
   */
  PSSigner(size_t attribute_num);

  /**
   * @brief Construct a new PSSigner object
   *
   * @param attribute_num input The maximum number of attributes supported by the system.
   * @param g input The generator of G1.
   * @param gg input The generator of G2.
   */
  PSSigner(size_t attribute_num, const G1& g, const G2& gg);

  /**
   * @brief Generate PS private key and public key.
   *
   * Caution: This function will overwrite the existing private key!
   *
   * @return the public key.
   */
  PSPubKey
  key_gen();

  /**
   * @brief Get the public key.
   */
  PSPubKey
  get_pub_key() const;

  /**
   * @brief EL PASSO ProvideID.
   *
   * @param request input The ID request generated by the PSRequester.
   * @param associated_data input Associated data used for NIZK Schnorr verification.
   * @param sig output The PS signature
   * @return true If the NIZK verification succeeds.
   * @return false Otherwise. @p sig will not be generated.
   */
  bool
  el_passo_provide_id(const PSCredRequest& request,
                      const std::string& associated_data, PSCredential& sig) const;

  /**
   * @brief Use PS key to sign over a committed message.
   *
   * @param commitment input The committed message.
   * @return PSCredential containing
   *  - G1, the PS signature, first element
   *  - G2, the PS signature, second element
   */
  PSCredential
  sign_commitment(const G1& commitment) const;

  /**
   * @brief Use PS Key to sign over both committed message and plaintext message.
   *
   * This function will simulate the commitment of plaintext attributes to generate
   * a single commitment and invoke PSSigner::sign_commitment().
   *
   * @param commitment input The committed message.
   * @param attributes input The plaintext attributes and empty strings are used as
   *        placeholders for secret attributes.
   * @return PSCredential containing
   *  - G1, the PS signature, first element
   *  - G2, the PS signature, second element
   */
  PSCredential
  sign_hybrid(const G1& commitment, const std::vector<std::string>& attributes) const;

private:
  bool
  el_passo_nizk_verify_request(const PSCredRequest& request,
                               const std::string& associated_data) const;

private:
  size_t m_attribute_num;  // maximum supported number of attributes
  G1 m_sk_X;               // private key, X
  PSPubKey m_pk;           // public key
};

#endif  // PS_SRC_PS_SIGNER_H_