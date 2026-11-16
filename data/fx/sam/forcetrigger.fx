#########################################################
##
FxInfo  

Input  
	InpName	hips
End

LifeSpan	200

Condition
	On 	time
	Time 	0

	Event 
		Type 	Local
		At 	root
		Lifespan 1
		Bhvr behaviors/null.bhvr
		BhvrOverride
			physForceType Up
			physForcePower 700
			physForcePowerJitter 200
			physForceRadius 10
		End
	End
End

End			
