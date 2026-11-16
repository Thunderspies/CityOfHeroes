#############################################################
## HalberdAOE.fx
#############################################################

FxInfo
Lifespan	500

#############################################################

############################	WARM UP

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFx	IncarnatePowers/Incarnate_Gather.fx
		Lifespan 45
	End

End

##	Glowing hands

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Rebirth_01 100 100 .3
	End

	Event
		EName	Left
		Type	Local
		At	wepL
		Part1	:HandFlare.part
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	:HandFlare.part
		Lifespan 45
	End
End


##	Ground beneath character


Condition
	On 	Time
	Time 	18

	Event
		Type	StartPositOnly
		At	Root
		part	:Start_Flash_01.part
		Lifespan 80
	End

	Event
		Type	StartPositOnly
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		part	:Start_Flash_03.part
		Lifespan 80
	End
End


################################	INITIAL BLAST

Condition
	On	Time
	Time	0

	Event
		EName	Hookup
		Type	Start
		At	Root
	End
End

Condition
	On	Time
	Time	23

	Event
		Type	StartPositOnly
		At	Hookup
		Part	:Shockwave_01.part
		Lifespan	5
	End

	Event
		Type	Local
		At	Hookup
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 1.5 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		2.25 2.25 2.25
		End
		Lifespan 15
	End

End

##	RADIALS

Condition
	On 	Time
	Time 	20

	Event
		Type	StartPositOnly
		At	Hips
		BhvrOverride
			PyrRotateJitter	1 5 1
			PositionOffset	0 0.5 0
		End
		Part	:Rings_Dropping_LocalFacing.part
		Lifespan 80
	End

End

################################	OUTER RIM EFFECTS


Condition
	On	Time
	Time	0
	Event
		Type	StartPositOnly
		At	Root
		Ename	StreakAnchor
		BhvrOverride
			PositionOffset	0.0 4.5 0.0
		End
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	StartPositOnly
		At	Root
		part	:Glow_Ring.part
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
	End
	Event
		EName	ThunderTracks
		Type	PositOnly
		At	StreakAnchor
		part	:StreakUp.part
		Lifespan 40
	End

	Event
		EName	ThunderTracks
		Type	PositOnly
		At	StreakAnchor
		BhvrOverride
			PositionOffset	0.0 0 0.0
		End
		part	:StreakUpSparks.part
		Lifespan 80
	End
End


#############################################################


End