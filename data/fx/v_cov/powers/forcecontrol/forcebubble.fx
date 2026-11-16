#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	HIPS
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End



	
#####################
##Hands
############################################


Condition
	On	Time	
	Time 	4

	Event
		Type 	Local
		At	Origin
		Sound forcehands5 80 80 .22
	End

End

Condition
	On	Time	
	Time 	10

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
	Time 	10

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Part1	:HandGlows.part
		
				
	End
	
	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	:HandGlows.part

	End	

End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime3
		Type	start
		At	WepR
	
		Geom	EarthGraspHit  #CosmicPeaceBringerShield
		
		BhvrOverride
			StartColor	255 205 100
		End
		Bhvr 	behaviors/ForceBubbleHands.bhvr

		LifeSpan	9

	End
	
	Event
		EName 	Prime4
		Type	start
		At	WepL
		
		Geom	EarthGraspHit  #CosmicPeaceBringerShield

		BhvrOverride
			StartColor	255 205 100
		End
		Bhvr 	behaviors/ForceBubbleHands.bhvr

		LifeSpan	9

	End

End

Condition
	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble2.bhvr
		
		Part1	:bubblesphere.part
		POther	Next
		PMagnet	Next
		
	End

	Event
		EName 	Prime2
		Type	Posit
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/ForceBubble.bhvr
		BhvrOverride
			StartColor	255 205 100
			SpinJitter	.025 .025 .025
		End
		
		Geom	EarthGraspHit  #CosmicPeaceBringerShield
				
	End

	Event
		EName 	Distort
		Type	Local
		At	Prime2
		BhvrOverride
			Scale	6.65 6.65 6.65
			SpinJitter	.15 .15 .15
			Alpha	64
		End
		
		Geom	FX_BlurBall
				
	End
	
	Event
		EName 	Distort2
		Type	Local
		At	Prime2
		BhvrOverride
			Scale	6.7 6.7 6.7
			SpinJitter	.1 .1 .1
			Alpha	80
		End
		
		Geom	FX_BlurBall
				
	End

End

End
