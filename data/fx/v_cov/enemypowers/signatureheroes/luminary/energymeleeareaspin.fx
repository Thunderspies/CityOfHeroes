#########################################################
##	Lightning Bolt
##
FxInfo

lifespan	60

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	mystic
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Audio
		Type	Local 
		At	Chest
		
		Sound powerup2 100 100 .8
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFX	:EnergyMeleeAttached.fx
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	hand1
		Type	Local 
		At	WepL
		Bhvr	behaviors/genericParticleFade.bhvr
		Part1	:Largepowerpunch3.part
		Part3   :LargepowerpunchGlow.part

		LifeSpan	30

	End


	Event
		EName 	hand2
		Type	Local 
		At	WepR
		Bhvr	behaviors/genericParticleFade.bhvr
		Part1	:Largepowerpunch3.part
		Part3   :LargepowerpunchGlow.part

		LifeSpan	30

	End

End

Condition
	On 	Time
	Time 	36

	Event
		EName 	flash1
		Type	Local 
		At	WepL
		Part2	:Largepowerpunch2.part
		LifeSpan	5

	End
	
	Event
		EName 	flash2
		Type	Local 
		At	WepR
		Part2	:Largepowerpunch2.part
		LifeSpan	5

	End

	Event
		EName 	Prime
		Type	Local 
		At	mystic
		Part1	:HitRingFlat.part
		Part2	:HitRingThinFlat.part
		Part3   :QuickRings.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		800
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
#####################################################################################

End


Condition
	On 	Time
	Time 	55

End

End		