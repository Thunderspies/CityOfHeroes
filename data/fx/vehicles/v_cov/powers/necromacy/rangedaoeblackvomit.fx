#########################################################
##	
##

FxInfo

LifeSpan	300

##################################
Condition
	On 	Time
	Time 	34



	Event
		EName	Prime
		Type	Local
		At	head
		
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
	End

	Event
		EName	Prime2
		Type	start
		At	Prime
		Altpiv   1
		
		BHVR	Behaviors/VomitProjectile.bhvr
		Part	:CoreBubbles.part
		Part	:Core.part
		Part1	:PoisonSpit.part
		Part2	:PoisonSpitBubbles.part
		Part3	:PoisonSpecksSpit.part
		Part3	:PoisonSpecks.part
		Part1	:PoisonBallz.part

	#		Sound 	Poison2	100.0 30.0 .75
		magnet	t_Root
		
	End
	
End


Condition
	On 	PrimeHit


	Event
		EName	Prime
		Type	Destroy
		
	End

End

Condition
	On 	Prime2Hit

	
	Event	
		Type	start
		At	T_Root
		BHVR	:GreenRingSplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	15
	End

	Event
		Type	start
		At	T_Root
		Part	:BaseSplatter.part
		Part	:BaseBubblesSplatterBLK.part
		Part	:BaseBubblesSplatter.part
		Part	:VomitBaseFlat.part
		Part	:PoisonSpecksFlat.part
		Part	:VomitBaseFlatBLK.part
		#Part	:BlackVomitSpitBallz.part
		LifeSpan	15
	End

	Event
		Type	start
		At	T_Root
		
		Part	:BaseBubbles.part
		Part	:BaseBubblesDRK.part
		Part	:BaseBubblesDRK2.part
		LifeSpan	28
	End

	Event
		EName	Prime2
		Type	Destroy
		
	End

End

End				