#########################################################
##	RadiationAura
FxInfo
LifeSpan	120

Input  
	InpName	Hips
End

Input  
	InpName	Head
End

Input  
	InpName	Root
End

##################################
Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Hips
		Part4	:RadiationRings.part
		Part5	:RadiationRingBurst.part

		Sound darkregen 66 66 .5
	End

End

Condition
	On	Time
	Time	7
	Event
		EName	Burst
		Type	local
		At	Hips
		Part5	:RadiationRingBurst.part
		Sound radcharge 66 66 .55
		
	End
End

Condition
	On	Time
	Time	15
	Event
		EName	Burst
		Type	local
		At	Hips
		Part1	:RadiationLightHitLocal.part
	End
End

Condition
	On	Time
	Time	21
	Event
		EName	Burst2
		Type	local
		At	Hips
		Sound radiation5_loop 66 66 .75
	End

	
End




End				