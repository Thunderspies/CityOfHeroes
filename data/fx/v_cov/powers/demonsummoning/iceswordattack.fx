#############################################################
## IceSwordAttack.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	SwordAnchor
		Type	Local
		At	WepR
		Bhvr	behaviors/powers/coldcontrol/IceWeapon.bhvr
		BhvrOverride
			StartColor	180 230 255
		End
		Geom	Tintable_GreaterIceWeapon
		SOund	coldswordout1 75 30 1.0
	End

	Event
		EName	SwordBottom
		Type	Local
		At	SwordAnchor
		AltPiv	1
		Part	:Sword_Snowfall.part
		Part	:Sword_Mist_Detached.part
		Part	:Sword_Sparkles.part
		POther	SwordTop
	End

	Event
		EName	SwordTop
		Type	Local
		At	SwordAnchor
		AltPiv	2
		Part	:Sword_Frost.part
		Part	:Sword_Mist.part
		Part	:Sword_Mist_Trail.part
		POther	SwordBottom
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	snowburst
		Type	LOCAL
		At	WepR
		Part1	:Shiver_Snowburst.part

	End

	Event
		EName 	snowburstSkatter
		Type	LOCAL
		At	WepR
		Part1	:Sword_SnowScatter.part
		Sound cold_loop 70 70 .77
	End


End


End
