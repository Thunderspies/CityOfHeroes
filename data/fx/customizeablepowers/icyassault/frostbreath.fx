#########################################################
##	IceBolt
###########################################################################
FxInfo

Input  
	InpName	head
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
		EName 	fist
		Type	Local 
		At	head
		Sound coldblast2 80.0 80.0 .4

	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		
		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		LifeSpan	70
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part

		LifeSpan	70
	End
		

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part

		LifeSpan	70

	End

End

Condition
	On 	Time
	Time 	30



	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivot01
		bhvr	CustomizeablePowers\IcyAssault\scale1a.bhvr
		Sound Breathattack 80.0 80.0 .8
		Lifespan	55
	End

	Event
		Ename	PushForce
		Type	Local
		At	Spike
		
		BhvrOverride
			PositionOffset		0 0 10
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		15
			PhysForcePower		80
			PhysForcePowerJitter	10
		End
		

		LifeSpan	30

	End
	Event
		EName 	Prime
		Type	Local 
		At	Spike
		Altpiv	1

		Part1	CustomizeablePowers\IcyAssault\FrostBreathIce.part
		Part2	CustomizeablePowers\IcyAssault\FrostBreathSnow.part
		Part3	CustomizeablePowers\IcyAssault\FrostBreathMist.part
			
	End
	
End


End				