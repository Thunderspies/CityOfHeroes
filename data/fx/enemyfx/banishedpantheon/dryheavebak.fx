#########################################################
##	
##

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Head
End




LifeSpan	100

##################################

Condition
	On 	Time
	Time 	38

	Event
		EName	DustStream
		Type	Start
		At	Head
		ChildFX EnemyFX/BanishedPantheon/DryHeaveBoneChild.fx
		#Magnet	Target
		LookAt	Target
	End
End


Condition
	On 	Time
	Time 	36
	Event
		EName	DustStream
		Type	Start
		At	Head
		Part	:DryHeaveStreamDust01.part
		LookAt	Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	Spike
		Type	Local
		At	Head
		bhvr	:DryHeave.bhvr
	End

	Event
		EName	GasStream
		Type	Start
		At	spike
		Part	:DryHeaveBeetles01.part
		Part	:DryHeaveMainStream01.part
		Sound DryHeave2 100.0 100.0 .75
		
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