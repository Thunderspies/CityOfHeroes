#############################################################
## SmokeBomb.fx
#############################################################

FxInfo
Lifespan 30

#############################################################


Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	origin
		Sound	Ninja_JumpEmote(noMX) 100 100 1
		# Sound	Ninja_JumpEmote 100 100 1
	End

#############################################################

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		15
		End

		Flags	AdoptParentEntity
		Lifespan 1
	End
	Event
		Type	StartPositOnly
		At	Root
		Part	:SmokePuff_Land.part
		Part	:SmokePuff_Linger.part
	End

	Event
		Type	Local
		At	FootR
		Part	:SmokeTrail.part
		Lifespan 5
	End

	Event
		Type	Local
		At	LArmL
		Part	:SpeedLines_Small.part
		Lifespan 1
	End

	Event
		Type	Local
		At	LArmR
		Part	:SpeedLines_Small.part
		Lifespan 1

	End

	Event
		Type	Local
		At	LLegL
		Part	:SpeedLines_Small.part
		Lifespan 2

	End

	Event
		Type	Local
		At	LLegR
		Part	:SpeedLines_Small.part
		Lifespan 2

	End
End



#############################################################

