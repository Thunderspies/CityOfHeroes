#########################################################
##	jack
##

FxInfo

Flags    InheritAlpha



Condition

	On	Time
	Time	8

	Event
		Type	Local
		At	WepR
		Sound daggersout 70 70 .33
	End
End


Condition
	On 	Time
	Time 	12

	Event
		EName 	right
		Type	Local 
		At	WepR
		Geom	SthenoSnakeDagger
		Bhvr	Behaviors/Reverse_grip.bhvr
		BhvrOverride
			Scale 1 1 2
		End
	End

	Event
		EName 	left
		Type	Local 
		At	WepL
		Geom	SthenoSnakeDagger
		Bhvr	Behaviors/Reverse_grip.bhvr
		BhvrOverride
			Scale 1 1 2
		End
	End
End

End		