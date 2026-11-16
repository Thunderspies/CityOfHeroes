#########################################################
##	template

FxInfo

Flags InheritAlpha InheritAnimScale


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepL
		Anim	Manticore_bow
		Bhvr	Behaviors/turn_bow.bhvr
		BhvrOverride
			pyrRotate	180 0 0 #-.35
		End
		Sound Bowout 50 50 .2
	End
End



End		