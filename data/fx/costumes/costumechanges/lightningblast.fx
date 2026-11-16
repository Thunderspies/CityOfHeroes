#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound eleccage_loop 70 70 .25
	End

End

#########################################################
#################### LIGHTNING ##########################
#########################################################


Condition
	On	Time
	Time	20

	Event
		ENAME	HeadGlow
		Type	Local
		At	Origin
		Part	:LargeBolts.part
		Part	:LightningGlow.part

		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10

	End


End

Condition
	On	Time
	Time 	0
	#Chance	.25


	Event
		Type	Local
		At	Hips
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	Hair
		Lifespan 40
	End

	Event
		Type	Local
		At	Chest
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	Hair
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmR
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	WepR
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmL
		Lifespan 40
	End

	Event
		Type	Local
		At	UArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LArmL
		Lifespan 40
	End

	Event
		Type	Local
		At	LArmL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	WepL
		Lifespan 40
	End

	Event
		Type	Local
		At	FootR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegR
		Lifespan 40
	End

	Event
		Type	Local
		At	FootR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegR
		Lifespan 40
	End

	Event
		Type	Local
		At	LLegR
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	ULegR
		Lifespan 40
	End


	Event
		Type	Local
		At	FootL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	LLegL
		Lifespan 40
	End

	Event
		Type	Local
		At	LLegL
		Part 	:BodyGlowsLarge01.part
		Part	:BodyGlowsLarge02.part
		Part	:BodyArcsSmall02.part
		Pmagnet	ULegL
		Lifespan 40
	End
End

Condition
	On	Time
	Time 	30

	Event
		Type	Local
		At	Hips
		Part	Costumes\CostumeChanges\SmallGlow.part	#middle glow
		Part	Costumes\CostumeChanges\LowerBlast.part
		Part	Costumes\CostumeChanges\LargeGlow.part 	#big glow
		Part	Costumes\CostumeChanges\SmokePillar3.part
		Part	Costumes\CostumeChanges\LargeBlast.part
	End

End

Condition
	On	Time
	Time 	34

	Event
		Type	Local
		At	Hips
		Part	Costumes\CostumeChanges\MainExplosion.part
	#	Part	Costumes\CostumeChanges\SmokePillarAdditive.part
	#	Part	Costumes\CostumeChanges\LargeBlast.part
	End

End
End