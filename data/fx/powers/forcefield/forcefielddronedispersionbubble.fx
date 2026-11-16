#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HIPS
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	Inpname	Root
End

Input  
	Inpname	Origin
End


#####################
##Hands
############################################


Condition
	On	Time	
	Time 	7

	Event
		Type 	Local
		At	Origin
		Sound forcehands3c 80 80 .8
	End

End


Condition
	On	Time	
	Time 	17

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	PositOnly
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/DispersionBubbleLarge2.bhvr
		
		Part1	powers/forcefield/bubblesphereLarge.part
		
	End

	Event
		EName 	Prime2
		Type	PositOnly 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/DispersionBubbleLarge.bhvr
		
		Geom	FX_ForceBubble
		
		#LifeSpan	50
				
	End

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:RepulsionBubbleRing.part
				
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName 	Ring
		Type	PositOnly 
		At	Root
		Part1	:RepulsionBubbleRingBurst.part
				
	End

End

End
