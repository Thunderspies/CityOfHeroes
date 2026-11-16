#########################################################
##	Thunder Kick
FxInfo

Lifespan 100

##################################

Condition
	On	Time
	Time	0
		
	Event	
		ENAME	KickFlash
		Type	Local
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr

		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive.part

		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Part1	:DarkHand.part
		Part3	:DarkHands.part
		Sound darkwindup3 60 60 .44
		LifeSpan 50
	
	End

	Event	
		ENAME	KickFlash
		Type	Local
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive.part

		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Part1	:DarkHand.part
		Part3	:DarkHands.part
		#Sound	whoosh13 60 30 .88
		LifeSpan 50
	
	End
End


						