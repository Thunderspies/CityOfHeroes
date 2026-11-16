#############################################################
## OrestesRifle.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Geom	Soul_Tickler
		Sound shotgunout 50 50 .3
	End
End

#############################################################

End