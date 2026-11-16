#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Root
		Sound Seers_SootheMind_Hit 50 50 1
		Part	:Root_Rings_Spreading.part
		Part	:Root_Rings_Spreading_Fast.part
		Lifespan 45
	End

End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Root
		Part	:Torso_Rings_Rising.part
		Lifespan 45
	End

	Event
		EName 	EyeFX
		Type	Local
		At	Eyes
		Part	:PurpleHitMist.part
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeFX
		 AltPiv 1
		Part	:Eyes_Glow_Stationary.part
	End

	Event
		Type	Local
		At	EyeFX
		 AltPiv 2
		Part	:Eyes_Glow_Stationary.part
	End
End

##############################################################################

End