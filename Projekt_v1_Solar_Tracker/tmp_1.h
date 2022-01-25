#ifndef TPM_1_H
#define TPM_1_H

#include "frdm_bsp.h"

void TPM1_Init_PWM(void);
/**
 * @brief Get value of counter from channel of TPM1.
 *
 * @return Value of counter.
 */
uint32_t TPM1_GetVal(void);
/**
 * @brief Set value of counter from channel of TPM1.
 *
 * @param Value of counter.
 */
void TPM1_SetVal(uint32_t value);

#endif /* TPM_1_H */
