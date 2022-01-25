#ifndef TPM_0_H
#define TPM_0_H

#include "frdm_bsp.h"

void TPM0_Init_PWM(void);
/**
 * @brief Get value of counter from channel of TPM1.
 *
 * @return Value of counter.
 */
uint32_t TPM0_GetVal(void);
/**
 * @brief Set value of counter from channel of TPM0.
 *
 * @param Value of counter.
 */
void TPM0_SetVal(uint32_t value);

#endif /* TPM_0_H */
