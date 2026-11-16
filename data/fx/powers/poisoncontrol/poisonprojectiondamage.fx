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
	Time 	34



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
		
		Part1	:PoisonSpit.part
		Part2	:PoisonSpitBubbles.part
		Part3	:PoisonSpecksSpit.part
		Part3	:PoisonSpecks.part
		Sound Poison2 100.0 100.0 .75
		
	End
	
	Event
		EName	GasSpecks
		Type	Local
		At	spike
		Altpiv   1
		Part1	:PoisonBallz.part		
		Part2	:PoisonSpecks.part
	End

End


Condition
	On 	Time
	Time 	78

	Event
		EName	GasSpecks
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	80

	Event
		EName	all
		Type	Destroy
		
	End

End

End				