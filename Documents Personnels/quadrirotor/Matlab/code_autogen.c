void Discrete_step(void)
{
     /* local block i/o variables */

     real_T Deriv;
     real_T Integrator;
     real_T u;

   
     /* DiscreteIntegrator: '<S1>/Integrator' */
    Integrator += 0.005 * ki * error;
   
     /* Gain: '<S1>/Filter Coefficient' incorporates:
      *  DiscreteIntegrator: '<S1>/Filter'
      *  Gain: '<S1>/Derivative Gain'
      *  Inport: '<Root>/u'
      *  Sum: '<S1>/SumD'
     */
     Deriv = (kd * error - filter_state) * kf;
    
      /* Sum: '<S1>/Sum' incorporates:
       *  Gain: '<S1>/Proportional Gain'
       *  Inport: '<Root>/u'
       */
      output = kp * error + Integrator + Deriv;
      
        /* Saturate: '<S1>/Saturation' */
        if (output >= Discrete_P.Saturation_UpperSat) {
          /* Outport: '<Root>/y' */
          Discrete_Y.y = Discrete_P.Saturation_UpperSat;
       } else if (output <= Discrete_P.Saturation_LowerSat) {
         /* Outport: '<Root>/y' */
          Discrete_Y.y = Discrete_P.Saturation_LowerSat;
        } else {
          /* Outport: '<Root>/y' */
          Discrete_Y.y = output;
        }
      
        /* End of Saturate: '<S1>/Saturation' */
      
        /* Update for DiscreteIntegrator: '<S1>/Integrator' */
        Integrator += 0.005 * ki * error;
      
        /* Update for DiscreteIntegrator: '<S1>/Filter' */
        filter_state += 0.01 * Deriv;

      
}
