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
		
		Part1	:BlackVomitSpit.part
		Part2	:BlackVomitSpitBubbles.part
		Part3	:BlackVomitSpecksSpit.part
		Part3	:BlackVomitSpecks.part
		Sound BlackVomit2 100.0 100.0 .75
		
	End
	
	Event
		EName	GasSpecks
		Type	Local
		At	spike
		Altpiv   1
		Part1	:BlackVomitBallz.part		
		Part2	:BlackVomitSpecks.part
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