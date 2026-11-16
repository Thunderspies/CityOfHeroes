#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 200


Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/fadein.bhvr
                        BhvrOverride
                                    Behavior
                                    FadeInLength	15
			End
		Flags	AdoptParentEntity
	End
End


End			