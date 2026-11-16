#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Origin
End

###########################################################

Condition

	On 	Time
	Time 	38
	
	Event
		Type	local
		At	Origin
		Sound forcebolthit 120 120 .7
	End

End

Condition

	On 	Time
	Time 	9
	
	Event
		EName 	hand
		Type	local
		At	WepR
		
		Geom	FX_NemesisStaff
	End
	
	
End

Condition

	On 	Time
	Time 	48
	
	Event
		EName 	Prime
		Type	start
		At	hand
		Altpiv	1
		Bhvr	behaviors/projectileNOspin.bhvr
		Geom	ForceBolt
		Magnet	Target
		LookAt	Target
		Part1	:ForceMist01.part
		Part2	:ForceBallTrail.part
		
	End

	Event
		EName 	Prime2
		Type	start
		At	hand
		Altpiv	1
		Bhvr	behaviors/projectileSLIGHTspin.bhvr
		Geom	ForceBoltTail
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	hand
		Type	local
		At	hand
		Altpiv	1
		Part1	:HandGlow.part
		LifeSpan	30		
	End

	Event
		EName 	hand2
		Type	local
		At	hand
		Altpiv	1
	
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands3.bhvr

		LifeSpan	1

	End
End

Condition
	On 	PrimeHit

	Event
		Type    Destroy
		EName 	Prime

	End

	Event
		Type    Destroy
		EName 	Prime2

	End

End

End
		