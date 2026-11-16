#############################################################
## Arachnos Soldier - Wolf Spider - Machine Gun GEO
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

#Condition
#	On 	Time
#	Time 	10
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	WepR
#		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
#		Geom	GEO_WepR_Arachnos_MGun_02
#		Sound	shotgunout 50 40 .22
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		#Anim	CustomWeapon_AssaultRifle_Arachnos02
		Sound shotgunout 50 50 .22
	End
End

#############################################################

End