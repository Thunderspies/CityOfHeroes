#########################################################
##	template

FxInfo

#########################################################
Condition
	On Time
	Time 0

	Repeat	5
	RepeatJitter 5

	Event
		Type	Local
		At	CHEST

		ChildFx	V_COV\PhysicsEnabled/IceChunkScatterSmallNonJagged.fx
		LifeSpan	120
	End

End

Condition
	On Time
	Time 3

	Event
		Type	Local
		At	CHEST
		Bhvr 	:icearmor.bhvr
		Part	:Frost2.part
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	20
	End

	Event
		Type	Local
		At	Hips
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	20
	End


	Event
		Type	Local
		At	UARML
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	20

	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	20

	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	20

	End

		Event
		Type	Local
		At	LARML
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	20

	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan	20
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan	20

	End

End

##############################33
##Feet
################################

Condition
	On Time
	Time 5

	Event
		Type	Local
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan	13
	End

	Event
		Type	Local
		At	FootR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		Part	:FrostSmall.part
		LifeSpan	13

	End

###############################################################################
End

Condition
	On Time
	Time 3

	Event
		Type	Local
		At	UlegL
		Bhvr 	:icearmor.bhvr
#		part1   :bodychillbig.part
#		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	17

	End

	Event
		Type	Local
		At	UlegR
		Bhvr 	:icearmor.bhvr
#		part1   :bodychillbig.part
#		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	17

	End

	Event
		Type	Local
		At	LlegR
		Bhvr 	:icearmor.bhvr
#		part1   :bodychillbig.part
#		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	17

	End

		Event
		Type	Local
		At	LlegL
		Bhvr 	:icearmor.bhvr
#		part1   :bodychillbig.part
#		part1   :SnowBurst.part
		Part	:FrostSmall.part
		LifeSpan	17

	End
End

######################################################3

Condition
	On Time
	Time 0

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	Powers\FollowUpGlow.part
		Part2	Powers\FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 10

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part2	Powers\FollowUpRing01.part
		Part3	Powers\FollowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 20

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

Condition
	On Time
	Time 30

	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks2.part
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


Condition
	On Time
	Time 13

	Event
		EName	pow2
		Type 	Destroy

	End

End

Condition
	On Time
	Time 75

	Event
		EName	all
		Type 	Destroy

	End

End



End

