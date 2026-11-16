#########################################################
##	pump_smoke
##
FxInfo

Flags InheritAnimScale

LifeSpan 108

###########  crate  ############################

Condition
	Event
		EName	Chute
		Type	Local  ##Posit 
		At	Back
		Anim	FX_Parachute
		Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                    FadeOutLength	25
			End
	End




End




End