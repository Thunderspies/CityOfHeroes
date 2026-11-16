#########################################################
##	jack
##

FxInfo

Input
	InpName	Origin
End

Input
	InpName	WepR
End

Input
	InpName	T_Chest
End

Flags    InheritAlpha


LifeSpan		200

Condition
	On 	Time
	Time 	0

	Event
		EName 	RockinHand
		Type	local
		At	WepR
		Geom	Rock
		Sound smallrockpickup 40 40 .4
		LifeSpan	60
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	start
		At	WepR
		Geom	Rock
		Bhvr	Behaviors/rockprojectile.bhvr
		Magnet	T_Chest
		Sound rockthrow 60 60 .28
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End
End

End