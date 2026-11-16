#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Sound Riktigun_loop 60 60 .45
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Ename	BlastNode
		Type	local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		geom	RiktiNode03

	End

End

End