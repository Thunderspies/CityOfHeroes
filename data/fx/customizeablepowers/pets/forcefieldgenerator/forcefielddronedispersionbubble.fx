#############################################################
## ForceFieldDroneDispersionBubble.fx
#############################################################

FxInfo

#####################
##Hands
############################################


Condition
	On	Time
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands3c 80 80 .8
	End

End


Condition
	On	Time
	Time 	17

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName 	  Prime
		Type	  PositOnly
		At	  HIPS

		Bhvr	behaviors/powers/forcefield/DispersionBubbleLarge2.bhvr
		Part1	:bubblesphereLarge.part
	End

	Event
		EName 	Prime2
		Type	PositOnly
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/DispersionBubbleLarge.bhvr
		BhvrOverride
			Alpha		255
			TintGeom	1
		End
		Geom	Tintable_FX_ForceBubble
	End

	Event
		EName 	Ring
		Type	PositOnly
		At	Root
		Part1	:RepulsionBubbleRing.part

	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Ring
		Type	PositOnly
		At	Root
		Part1	:RepulsionBubbleRingBurst.part

	End

End

End
