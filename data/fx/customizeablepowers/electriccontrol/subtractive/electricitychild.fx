#############################################################
## HEADER
#############################################################

FxInfo

Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound electransform2 70 70 .88
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Origin
		Sound elecarmor5_loop 70 70 .4
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################
## FAINT EYES AND MOUTH
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		.15 .2 0
		End
		Part	:BluePupil.part
		Part	:WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		-.15 .2 0
		End
		Part	:BluePupil.part
		Part	:WhitePupil.part
	End

	Event
		Type	Local
		At	hair
		BhvrOverride
			PositionOffset		 -.11 -.1 0
		End
		Part	:CreepyMouth.part
	End

	#Event
		#Type	Local
		#At	hair
		#BhvrOverride
			#PositionOffset		 -.11 .13 0
		#End
		#Part1	:Red_ElectricityControl02.part
		#Part2	:Red_ElectricityControl01white.part
	#End
End

#############################################################
## HAZY SMOKE
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	spadR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	spadL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
	End

	Event
		Type	Local
		At	ULegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	ULegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrilsSubtle.part
	End

	Event
		Type	Local
		At	LLegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	LLegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrils.part
	End

	Event
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityIonHaze.part
		Part	:Blue_ElectricityTendrils.part
	End
End

#############################################################
## RANDOM ELECTRICAL FLASHES
#############################################################

Condition
	On 	cycle
	Time 	3
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Hair
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	4
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	Hips
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End


Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LarmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LarmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	ULegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	UlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LlegL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	LlegR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End

Condition
	On 	cycle
	Time 	6
	Chance	0.7

	Event
		EName	3
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ElectricityControl01white.part
		Part	:Blue_ElectricityControl02.part
		Part	:Blue_ElectricitySparkBigGlow.part
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		lifespan 3
	End
End


#############################################################
## RANDOM ARCS OF LIGHTNING
#############################################################

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet UArmL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet UArmR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	UArmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet Hips
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	UArmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet Hips
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet Hips
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	10
		Type	Local
		At	UArmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LArmL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	11
		Type	Local
		At	LArmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet WepL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	13
		Type	Local
		At	UArmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LArmR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	14
		Type	Local
		At	LArmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet WepR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	3
		Type	Local
		At	ULEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet Chest
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	4
		Type	Local
		At	ULEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LLEGL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	4
		Type	Local
		At	LLEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet ULEGL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	5
		Type	Local
		At	LLEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet FootL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	3
		Type	Local
		At	ULEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet Chest
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	4
		Type	Local
		At	LLEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Red_ElectricitySparkBurst2.part
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet ULEGR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	4
		Type	Local
		At	ULEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LLEGR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	5
		Type	Local
		At	LLEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet FootR
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	5
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LLEGL
		lifespan 2
	End
End

Condition
	On	Cycle
	Time	3
	Chance	0.4

	Event
		EName	5
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:Blue_ChargedBolt01.Part
		Part	:Blue_ChargedBolt02.Part
		Part	:Blue_ChargedBolt05.part
		Part	:Blue_ChargedBolt06.part
		PMagnet LLEGR
		lifespan 2
	End
End


#############################################################
## FAINT RANDOM ARCS
#############################################################

Condition
	On	Cycle
	Time	14
	Chance	0.6

	Event
		Type	Local
		At	Hair
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet Chest
		POther  Chest
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	11
	Chance	0.6

	Event
		Type	Local
		At	UarmR
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	16
	Chance	0.6

	Event
		Type	Local
		At	WepR
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LarmR
		POther  LarmR
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	20
	Chance	0.6

	Event
		Type	Local
		At	UarmL
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	9
	Chance	0.6

	Event
		Type	Local
		At	WepL
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LarmL
		POther  LarmL
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	17
	Chance	0.6

	Event
		Type	Local
		At	UlegL
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	8
	Chance	0.6

	Event
		Type	Local
		At	FootL
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet Llegl
		POther  Llegl
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	12
	Chance	0.6

	Event
		Type	Local
		At	UlegR
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		lifespan 10
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.6

	Event
		Type	Local
		At	FootR
		#Bhvr	Behaviors\TelportationFade.bhvr
		part	:Blue_BodyArcsSmall01.part
		part	:Blue_BodyArcsSmall02.part
		Pmagnet LlegR
		POther  LlegR
		lifespan 10
	End
End

#############################################################

End