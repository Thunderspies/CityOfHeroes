#########################################################
##	
##

FxInfo


Input  
	InpName	head
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Lifespan	150

Condition
	On 	Time
	Time 	35

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	60
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Sound Ignite2 100.0 100.0 .8
	
		LifeSpan	60
	End


End

Condition
	On 	Time
	Time 	30



	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivot01
		bhvr	behaviors/scale1a.bhvr	
		Lifespan	80
	End

	Event
		EName	GasStream
		Type	Local
		At	spike
		Altpiv   4
		
		Part1	:FireSpit.part
		Part2	:FireSpecksSpit.part
		Part3	:FireSpecksFalling.part
		Part4	:FireSpit2.part
		Part5	:FireSpitStart.part
		Sound arcoflame 100 100 .88
		Lifespan	80
		
	End
	
End

End				