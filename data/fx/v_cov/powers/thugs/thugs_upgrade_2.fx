#########################################################
##	pump_smoke
##
FxInfo

Flags InheritAnimScale

LifeSpan 297




#########  sound  ######################################



#######  animated duffel bag  ###########################


Condition
	Event
		EName	Bag
		Type	Start  ##Posit
		At	Root  ##Mystic  
		Anim	FX_Duffelbag
		Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                   FadeOutLength	10
				   PositionOffset	0 .3 0
			End
	End

	

End




End