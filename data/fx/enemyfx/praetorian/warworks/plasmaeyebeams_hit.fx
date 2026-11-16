#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Root
		Sound Plasma_Welding_Ray_Hit 200 200 1
	End

	Event
		Type	Local
		At	Chest

		Part	:Hit_Flash_Large2.part

		Lifespan 2
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Hit_Rings_Blue_Continuing.part
		Part	:Plasma_Streak.part
		Part	:Plasma_Sparks.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Root
		ChildFX :LethalForce_Continuing.fx
		Lifespan 180
	End
End

#############################################################

End