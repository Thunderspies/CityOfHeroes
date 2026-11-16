#########################################################
##	template

FxInfo

Condition
	On 	Time
	Time 	12

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .7
		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	66

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan 1
	End

End

################################################################################
##Streaks
################################################################

Condition
	On 	Time
	Time 	64

	Event
		EName	Prime
		Type	start
		At	FootR
		Bhvr	behaviors/Focusprojectile.bhvr
		Part	:BattleAxeRays.part
		Part	:BattleAxeRaysCenter.part
		Part	:BattleAxeSparks.part

		Part	:EnergyTrail.part

		Magnet	T_Root
		LookAt	T_Root
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	64

	Event
		EName	streak
		Type	Destroy
		
	End
End

Condition
	On 	Time
	Time 	74

	Event
		EName	all
		Type	Destroy
		
	End

End

End

	