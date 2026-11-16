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
		EName 	stungun
		Type	Local
		At	WepR
		Geom	GEO_WepR_Stungun_1
		Sound stungunout 50 50 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr

	End

	Event
		Type	Local
		At	stungun
		Altpiv	1

		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	:Glow.part
		Part	:GlowStar.part
	End
End

End

