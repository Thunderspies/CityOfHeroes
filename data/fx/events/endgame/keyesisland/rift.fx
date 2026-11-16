#############################################################
## AntimatterPulse.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Rift_Hurricane.part
		Part	:Rift_Hurricane_Small.part
		Part	:Rift_Blackhole.part
		Part	:Rift_Hurricane_Swirl.part
		Lifespan	10
	End
	Event
		Type Local
		At Root
		Sound KeyesIsland_Rift_Loop 100 100 .7
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Rift_Flash.part
		Lifespan	15
	End


	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Rift_BlackHole_Expand.part
	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Rift_TechLabs.part
		Part	:Rift_BlackHole_NoiseScreen.part
	End
End


Condition
	On	Time
	Time	50

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0.0 10.0 0.0
		End
		Part	:Rift_BlackHole_Boils.part
	End
End

#############################################################

End
