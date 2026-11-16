#############################################################
## FX System Name
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Nemesis_03
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .6
	End

	Event
		EName 	hookup
		Type	Local
		At	WepR
		Geom	Nemesis_Gat
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound torch1_loop 50 50 .4
	End

	Event
		Type	Local
		At	hookup
		Altpiv	1
		Geom	FX_BarrellSpin
		Bhvr	Behaviors/BarrellSpin.bhvr
	End

	Event
		Type	Local
		At	hookup
		Altpiv	2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	WeaponFx/FireSpecksFlame.part
		Part	WeaponFx/FireSpecksFalling2.part
	End
End

#############################################################

End