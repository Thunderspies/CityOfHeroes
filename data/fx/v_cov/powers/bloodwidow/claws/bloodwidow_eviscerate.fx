#############################################################
## Blood Widow "Eviscerate" FX
#############################################################

FxInfo

Lifespan 200

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Eviscerate2 100.0 100.0 .8
	End
End

#############################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade_R
		LifeSpan 35
	End

	Event
		Type	local
		At	WepL
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade
		LifeSpan 35
	End
End

#############################################################

Condition
	On 	Time
	Time 	49

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part	Powers\Claws\EviscerateStreak.part
		Lifespan 2
	End

	Event
		EName	Streak9
		Type	start
		At	wepL
		Part	Powers\Claws\ClawEmbersBroadEvicerate.part
		LifeSpan 3
	End

	Event
		EName	Streak8
		Type	start
		At	wepR

		Part	Powers\Claws\ClawEmbersBroadEvicerate.part
		LifeSpan 3
	End
End

#############################################################

Condition
	On	Time
	Time	49

	Event
		Type 	Local
		At	T_Chest
		Part1	Powers\Claws\Clawflash.part
		Part3	Powers\Claws\ClawEmbers.part
		Part4	Powers\Claws\ClawEmbersBroad.part
		Part5	Powers\Claws\ClawsStar.part
		Sound clawhit6 100.0 100.0 .5
		Lifespan 2
	End
End

#############################################################

End