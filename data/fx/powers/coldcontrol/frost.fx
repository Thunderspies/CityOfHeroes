#########################################################
##	IceBolt
###########################################################################
FxInfo

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	200

#####################################################################
Condition
	On	Time
	Time	8

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Sound frost 75 75 1.0
		
		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		LifeSpan	70
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	:ColdArmMist.part

		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	:ColdArmMist.part

		LifeSpan	70

	End

End

Condition
	On 	Time
	Time 	30



	Event
		EName 	Prime
		Type	Local 
		At	Mystic

		Part1	:FrostIce.part
		Part3	:FrostMist.part
		LifeSpan	30	
	End
	
	Event
		EName 	Prime2
		Type	Local 
		At	Mystic

		Part2	:FrostSnow.part
		Part3	:FrostMist.part
		LifeSpan	35	
	End

	Event
		Ename	PushDirection
		Type	Posit
		At	Mystic
		
		Lookat	Target

		LifeSpan	30

	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		10
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		

		LifeSpan	30

	End
End


End				