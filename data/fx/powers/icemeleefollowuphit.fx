#########################################################
##	template

FxInfo

#########################################################
Condition
	On Time 
	Time 3

	Event
		Type	Local 
		At	CHEST
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_CHEST
		part5	:bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	10
	End

	Event
		Type	Local 
		At	Hips
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_HIPS
		part1	:bodychillbig.part
		#part2   :HeadLegschillbig.part
		part1   :SnowBurst.part
		LifeSpan	10
	End


	Event
		Type	Local 
		At	UARML
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_UARML
		part1	:bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	10

	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_UARMR
		part1	:bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	10

	End

	Event
		Type	Local 
		At	LARMR
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_LARMR
		part1   :bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	10

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_LARML
		part1   :bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	10

	End

	Event
		Type	Local 
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		LifeSpan	10
	End

	Event
		Type	Local 
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		LifeSpan	10

	End

###########################################################################

	Event
		Type	Local 
		At	UlegL
		
		BhvrOverride
			PositionOffset	-.2 0 0
		End
		
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_ULEGR
		part1	:bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	7

	End

	Event
		Type	Local 
		At	UlegR
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_ULEGL
		part1	:bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	7

	End

	Event
		Type	Local 
		At	LlegR
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_LLEGR
		part1   :bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	5

	End

		Event
		Type	Local 
		At	LlegL
		Bhvr 	:icearmor.bhvr
		geom	FX_IceArmor_LLEGL
		part1   :bodychillbig.part
		part2	:IceArmorBits.part
		part3	:IceArmorBitsSmall.part
		part1   :SnowBurst.part
		LifeSpan	5

	End
################################################################################

	Event
		Type	Local 
		At	CHEST
		Bhvr 	:icearmor.bhvr
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
		LifeSpan	20

	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

	Event
		Type	Local 
		At	LARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbig.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

		Event
		Type	Local 
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		LifeSpan	40
	End

		Event
		Type	Local 
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1   :Snowfall.part
		LifeSpan	40

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
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
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
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
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

	