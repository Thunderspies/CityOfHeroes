#########################################################
##	iceweapon
########################################################
FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName 	Prime8
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Sword_07
	End
	Event
		EName 	Next
		Type	Local
		At	Prime8
		AltPiv 1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		POther	Prime
	End
	Event
		EName 	Prime
		Type	Local
		At	Prime8
		AltPiv 2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part5	EnemyFX\Carnival_Of_Shadows\Rapier\RapierGlow01.part
		POther	Next
		#PMagnet	Next

	End
End
Condition
	On 	Time
	Time 	50

	Event
		EName 	glowhand
		Type	Destroy


	End

End


End
