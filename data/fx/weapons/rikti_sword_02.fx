#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	WepR
		Sound axeout 100 100 .67
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Rikti_Sword_02
		Sound swordshing 100 100 .5
	End

	Event
		EName 	Geomhookup1
		Type	Local
		At	WepR
		Geom	RiktiBlade02
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound glow8_loop 60 60 .55
	End

#########################################################################3
##Anims
#############################################

#	Event
#		EName 	anim
#		Type	Local
#		At	WepR
#		Anim	Fx_RiktiSword02
#		Bhvr	Behaviors/RiktiSwordScale.bhvr
#
#	End
#
#	Event
#		EName 	nodes
#		Type	Local
#		At	anim
#		Animpiv	Chest
#		Part1	WeaponFx/RiktiSwordTrail.part
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	1
#
#	Event
#		EName 	anim2
#		Type	Local
#		At	WepR
#		Anim	Fx_RiktiSword02
#		Bhvr	Behaviors/RiktiSwordScale.bhvr
#
#	End
#
#	Event
#		EName 	nodes
#		Type	Local
#		At	anim2
#		Animpiv	Chest
#		Part1	WeaponFx/RiktiSwordTrail.part
#	End
#

End


End