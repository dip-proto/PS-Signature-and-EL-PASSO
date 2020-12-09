#ifndef PS_SRC_PS_SIGNER_H_
#define PS_SRC_PS_SIGNER_H_

#include "ps-encoding.h"

using namespace mcl::bls12;

/**
 * The PS Signer to sign over committed single message or messages.
 */
class PSSigner {
public:
  PSSigner(size_t attribute_num);
  PSSigner(size_t attribute_num, const G1& g, const G2& gg);

  /**
   * Generate PS private key and public key.
   * Caution: This function will overwrite the existing private key!
   *
   * @return the public key.
   */
  PSPubKey
  key_gen();

  /**
   * Get the public key.
   */
  PSPubKey
  get_pub_key();

  /**
   * EL PASSO ProvideID.
   *
   * @p A, input, the commited attributes.
   * @p c, input, used for used for NIZK Schnorr verification.
   * @p rs, input, used for NIZK Schnorr verification.
   * @p attributes, input, the plaintext attributes. Notice that
   *   the size of the vector is the total number of attributes, in
   *   which the committed attributes are replaced with empty string.
   * @p associated_data, input, used for NIZK Schnorr verification.
   * @p sig1, output, the PS signature, first element.
   * @p sig2, output, the PS signature, second element.
   * @return true if the NIZK verification succeeds; otherwise return false
   *   and @p sig1 and @p sig2 will not be initialized.
   */
  bool
  el_passo_provide_id(const PSCredRequest& request,
                      const std::string& associated_data, PSCredential& sig) const;

  /**
   * Use PS key to sign over a committed message.
   *
   * @p commitment, input, the committed message.
   * @return
   *  - G1, the PS signature, first element
   *  - G2, the PS signature, second element
   */
  PSCredential
  sign_commitment(const G1& commitment) const;

  /**
   * Use PS Key to sign over both committed message and plaintext message.
   * This function will simulate the commitment of plaintext attributes to generate
   * a single commitment and invoke PSSigner::sign_commitment().
   *
   * @p commitment, input, the committed message.
   * @p attributes, input, the plaintext attributes.
   * @return
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