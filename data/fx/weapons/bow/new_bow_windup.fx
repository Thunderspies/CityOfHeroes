#########################################################
##	template

FxInfo

Flags InheritAlpha InheritAnimScale IsWeapon


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Anim	New_bow
		Bhvr	Behaviors/turn_bow.bhvr
	End
End



End