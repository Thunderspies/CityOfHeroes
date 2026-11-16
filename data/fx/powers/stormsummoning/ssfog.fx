  #########################################################
##	Inferno FX
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Root
End

Input  
	Inpname	hips
End

Input  
	InpName	WepL
End

Input  
	Inpname	WepR
End


Condition
	On 	Time
	Time 	10


	Event
		EName   RingScale
		Type	local
		At	Root
		Sound fog2 100 100 .8

	End


	Event
		EName 	FogRing1
		Type	Local 
		At	RingScale
		bhvr    behaviors/powers/StormSummoning/FogRing2.bhvr
		Part	:SSFogClouds01.part
		LifeSpan 120
		
	End		


End

Condition
	On 	Time
	Time 	30

	Event
		EName 	FogRing2
		Type	Local 
		At	RingScale
		bhvr    behaviors/powers/StormSummoning/FogRing2.bhvr
		Part	:SSFogClouds01.part
		LifeSpan 120
		
	End		


End

Condition
	On 	Time
	Time 	85

	Event
		EName 	OriginNode
		Type	Posit 
		At	Root
		Geom	Roottochestadjustment
		
	End	

	Event
		EName 	FogRing3
		Type	Local 
		At	OriginNode
		AltPiv	1
		bhvr    behaviors/powers/StormSummoning/FogRing1.bhvr
		Part	:SSFogClouds02.part
		
	End		


End

Condition
	On 	Time
	Time 	65

	Event
		EName 	FogRing4
		Type	Posit 
		At	Root
		#Part	:SSFogClouds03.part
		
	End		

End

Condition
	On 	Time
	Time 	65

	Event
		EName 	FogRing4
		Type	Posit 
		At	Root
		Part	:SSFogGroundRing.part
		
	End		

End




End
