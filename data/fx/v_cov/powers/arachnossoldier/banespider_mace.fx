#############################################################
## Arachnos Soldier - Bane Spider - Mace GEO
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
		#Anim	CustomWeapon_Mace_Arachnos03
		Sound MaceOut 70 70 .44
	End
End

#############################################################

End