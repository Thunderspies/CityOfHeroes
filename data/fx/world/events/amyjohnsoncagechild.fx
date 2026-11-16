#########################################################
##	onfire
##
FxInfo  

#LifeSpan	185

##########################################################################################
##Healing Glow
#######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	RingdownHookUp
		Type	Posit
		At	origin
		Geom	RootToHeadAdjustment
	
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	ring
		Type	Posit
		At	origin
		#bhvr	behaviors/lifespan2.bhvr
		Part1	:HealingFlame01.part
		Part2	:WilloWispSpark.part	
		Part3	:HealingFlame02.part	
		Part3	:HealingFlame03.part
		LifeSpan	25
	End

End

##########################################################################################
##Healing Flames Ring
#######################################################

Condition
	On 	time
	Time 	83
	
	Event
		EName 	ring1
		Type	Posit
		At	origin
		#bhvr	behaviors/lifespan2.bhvr
		Part1	:HealingFlame01.part
		Part2	:WilloWispSpark.part	
		Part3	:HealingFlame02.part
		LifeSpan	25
	End

End


################################################################
##Round1
##############################################


Condition
	On 	Time
	Time 	20

	Event
		EName 	Fire1
		Type	local
		At	origin
		Anim	FX_WilloWisps
		Sound healingflame 100 100 .88
		LifeSpan	70
	End

	Event
		EName 	FireChest
		Type	local
		At	Fire1
		Animpiv	chest
		Part1	:HealingFlame.part
		##Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireFace
		Type	local
		At	Fire1
		Animpiv	Face
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireNeck
		Type	local
		At	Fire1
		Animpiv	Neck
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireWaist
		Type	local
		At	Fire1
		Animpiv	Waist
		Part1	:HealingFlame.part
		LifeSpan	65
	End

End

################################################################
##Round2
##############################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Fire2
		Type	local
		At	origin
		Anim	FX_WilloWisps
		Sound healingflame 100 100 .75
		LifeSpan	65
	End

	Event
		EName 	FireChest2
		Type	local
		At	Fire2
		Animpiv	chest
		Part1	:HealingFlame.part
		##Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireFace2
		Type	local
		At	Fire2
		Animpiv	Face
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireNeck2
		Type	local
		At	Fire2
		Animpiv	Neck
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireWaist2
		Type	local
		At	Fire2
		Animpiv	Waist
		Part1	:HealingFlame.part
		LifeSpan	65
	End

End

################################################################
##Round3
##############################################

Condition
	On 	Time
	Time 	40

	Event
		EName 	Fire3
		Type	local
		At	origin
		Anim	FX_WilloWisps
		#Sound 	healingflame 100 30 .75
		LifeSpan	65
	End

	Event
		EName 	FireChest3
		Type	local
		At	Fire3
		Animpiv	chest
		Part1	:HealingFlame.part
		##Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireFace3
		Type	local
		At	Fire3
		Animpiv	Face
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireNeck3
		Type	local
		At	Fire3
		Animpiv	Neck
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	65
	End

	Event
		EName 	FireWaist3
		Type	local
		At	Fire3
		Animpiv	Waist
		Part1	:HealingFlame.part
		LifeSpan	65
	End

End

################################################################
##Round4
##############################################

Condition
	On 	Time
	Time 	50

	Event
		EName 	Fire4
		Type	local
		At	origin
		Anim	FX_WilloWisps
		#Sound 	healingflame 100 30 .75
		LifeSpan	66
	End

	Event
		EName 	FireChest4
		Type	local
		At	Fire4
		Animpiv	chest
		Part1	:HealingFlame.part
		##Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	66
	End

	Event
		EName 	FireFace4
		Type	local
		At	Fire4
		Animpiv	Face
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	66
	End

	Event
		EName 	FireNeck4
		Type	local
		At	Fire4
		Animpiv	Neck
		Part1	:HealingFlame.part
		#Bhvr	Behaviors/Firespin.bhvr
		LifeSpan	66
	End

	Event
		EName 	FireWaist4
		Type	local
		At	Fire4
		Animpiv	Waist
		Part1	:HealingFlame.part
		LifeSpan	66
	End

End




Condition
	On 	Time
	Time 	160

	Event
		EName 	Ring
		Type	Destroy
		
	End

End

End
