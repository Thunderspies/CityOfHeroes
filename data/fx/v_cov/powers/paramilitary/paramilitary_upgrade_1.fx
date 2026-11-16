#########################################################
##	pump_smoke
##
FxInfo

Flags InheritAnimScale

LifeSpan 297




#########  sound  ######################################

Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	origin
		Sound Mercenary_upgrade2 60 60 .6
		
	End
End


#######  animated crate  ###########################


Condition
	Event
		EName	crate
		Type	Posit
		At	Root  ##Mystic  
		Anim	FX_Supply_Drop_1
		Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                    FadeOutLength	10
			End
	End

	Event
		EName	crate_chute
		Type	Posit
		At	Root   ##Mystic  
		Anim	FX_Supply_Drop_Chute
		Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                    FadeOutLength	20
			End

		Lifespan	97
	End


End




End