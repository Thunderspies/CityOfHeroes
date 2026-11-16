#############################################################
## SmokeBomb.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			FadeInLength		25
		End

		Flags	AdoptParentEntity
	End

	Event
		Type	Local
		At	LArmL
		Part	:SpeedLines_Small.part
		Lifespan 3
	End

	Event
		Type	Local
		At	LArmR
		Part	:SpeedLines_Small.part
		Lifespan 3

	End

	Event
		Type	Local
		At	LLegL
		Part	:SpeedLines_Small.part
		Lifespan 5

	End

	Event
		Type	Local
		At	LLegR
		Part	:SpeedLines_Small.part
		Lifespan 5

	End
End

Condition
	On	Time
	Time	12

	Event
		Type	StartPositOnly
		At	Root
		Part	:SmokePillar.part
		Part	:SmokePillar_Linger.part
		Part	:SmokePillar_Linger_Long.part
		Lifespan 2
	End

#############################################################

