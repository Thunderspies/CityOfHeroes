#########################################################
##	
##

FxInfo

LifeSpan	30

##################################
Condition
	On 	Time
	Time 	0

	Event
		
		Type	start
		At	chest
		
		Part	:HitBlackVomitSpit.part
		Part	:BLKHitBlackVomitSpit.part

		LifeSpan	10

	End

	Event
		
		Type	start
		At	chest
		
		Part	:HitBlackVomitBallz.part
		Part	:HitBlackVomitDrips.part
		Part	:HitBlackVomitDripsLITE.part			
		Part	:HitBlackVomitChunks.part

		LifeSpan	20

	End

	Event
		EName	GasStream
		Type	start
		At	Root
		
	End

End

Condition
	On 	Time
	Time 	23

	Event
		Type	start
		At	GasStream
		Part	:BaseSplatter.part
		Part	:HitBaseBubblesSplatterBLK.part
		Part	:HitBaseBubblesSplatter.part
		Part	:VomitBaseFlat.part
		Part	:PoisonSpecksFlat.part
		Part	:VomitBaseFlatBLK.part
		LifeSpan	7
	End

	Event
		Type	start
		At	GasStream
		
		Part	:BaseBubbles.part
		Part	:BaseBubblesDRK.part
		Part	:BaseBubblesDRK2.part
		LifeSpan	20
	End

End


End				