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
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Nemesis_04
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound nemesishalberd_loop 50 50 .56

	End

	Event
		EName 	FireVents
		Type	Local
		At	WepR
		Geom	nemesis_Halberd
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound shotgunout 80 80 .5

	End

	Event
		EName 	FireVentsFire
		Type	Local
		At	FireVents
		Altpiv	3

		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	WeaponFx/HalberdFlame.part
		Part	WeaponFx/HalberdFlame2.part
		Part	WeaponFx/HalberdSmoke.part
		Part	WeaponFx/HalberdFlame1.part
	End

End

End