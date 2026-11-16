#########################################################
##	template

FxInfo

Flags InheritAlpha InheritAnimScale IsWeapon


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Anim	Goblin_bow
		Bhvr	Behaviors/TempPower_Turn_bow.bhvr
		Sound Bowout 50 50 .2
	End
End



End