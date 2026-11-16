#########################################################
##	
##

FxInfo

Input  
	InpName	head
End


LifeSpan	100

##################################
Condition
	On 	Time
	Time 	0



	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
	End

	Event
		EName	GasStream
		Type	Local
		At	spike
		Altpiv   1
		
		Part1	POWERS\PoisonControl\PoisonSpit.part
		Part2	POWERS\PoisonControl\PoisonSpitBubbles.part
		Part3	POWERS\PoisonControl\PoisonSpecksSpit.part
		Part3	POWERS\PoisonControl\PoisonSpecks.part
		Sound femvomit1 60 60 .6
		Sound femvomit2 60 60 .6
		
	End
	
	Event
		EName	GasSpecks
		Type	Local
		At	spike
		Altpiv   1
		Part1	POWERS\PoisonControl\PoisonBallz.part		
		Part2	POWERS\PoisonControl\PoisonSpecks.part
	End

End


Condition
	On 	Time
	Time 	30

	Event
		EName	GasSpecks
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName	all
		Type	Destroy
		
	End

End

End				