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
	InpName	Root
End

Input  
	InpName	Head
End


LifeSpan	100

##################################
Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	Origin
		Sound Sandpour2 60 60 .64
	End
End

Condition
	On 	Time
	Time 	5
	Event
		Type	Local
		At	Origin
		Sound Splat 60 60 .94
	End
End

Condition
	On 	Time
	Time 	10


	Event
		EName	Bones
		Type	Start
		At	Root
		part	:DryHeaveBeetleRingSmall01.part
		Sound cockroaches 50 50 .33
		bhvr	:Adamastor.bhvr
		Lifespan 30
	End
End
Condition
	On 	Time
	Time 	0
	Event
		EName	Bones
		Type	Posit
		At	Head
		part	:DryHeaveHitDustSmall01.part
		
		Lifespan 30
	End
End


End				