#########################################################
##	template

FxInfo

Flags InheritAnimScale IsWeapon

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Anim	New_bow
		Bhvr	Behaviors/turn_bow.bhvr
	End
End



End