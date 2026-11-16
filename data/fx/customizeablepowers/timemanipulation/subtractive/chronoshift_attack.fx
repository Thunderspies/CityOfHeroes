#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 100
Flags InheritGeoScale

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_ChronoShift_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 4.0 25.0
		End
		Part	:Streaks.part
		Part	:Streaks2.part

		Lifespan 100
	End
End

Condition
	On	Time
	Time	5

	Event
		EName	StreakAnchor
		Type	Local
		At	Root
	End

	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 4.0 25.0
		End
		Part	:Swoosh.part
		Lifespan 80
	End
End


Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 5.0 7.5
		End
		Part	:Portal_Swirls.part

		Lifespan 30
	End
End

Condition
	On	Time
	Time	48

	Event
		Type	Start
		At	Chest
		Part	:Jaunt_Flash.part
		Part	:Jaunt_Glow.part
		Lifespan 30

	End
End

Condition
	On	Time
	Time	50



	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 5.0 7.5
		End
		Part	:Portal_Border.part
		Part	:Portal_Border_Swirls.part
		Part	:Portal_Spirals.part
		Lifespan 25

	End

	Event
		Type	Local
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0 5.0 7.5
		End
		Part	:Portal_Exploding_Swirls.part

		Lifespan 10
	End
End

#############################################################

End