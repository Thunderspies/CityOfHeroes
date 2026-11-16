#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	Root
End

Input  
	Inpname	T_Root
End
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hand1
		Type	Local 
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .4
		LifeSpan	100
	End

	Event
		EName	Hand2
		Type	Local 
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		LifeSpan	100
	End


End

Condition
	On 	Time
	Time 	30

	Event
		
		EName 	LocknarDummyHookup
		Type	Start 
		At	Root
		Geom	FX_InfectOthers
		Sound radblast 80 80 .85
		
	End

	Event
		EName 	LocknarFadeIn
		Type	Start 
		At	LocknarDummyHookup
		 AltPiv 1
		Bhvr	behaviors/Powers/RadiationControl/InfectOthersFadeIn.bhvr
		Geom	FX_Locknar
		Part1	:InfectOthersLocknar.part
		Part2	:RadiationHandElectrons.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		Part5	:RadiationInfectOthersHands.part
	End


End


Condition
	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		Bhvr	behaviors/MorterLobbprojectile.bhvr
		Geom	FX_Locknar
		Magnet	Target
		LookAt	Target
		Part1	:InfectOthersStreak.part
		Part2	:RadiationHandElectrons.part
		Part3	:MorterStreakGlows.part
		Part4	:MorterStreakRings.part
		
	End


End


Condition
	On	time
	time	63
		
	Event
		Ename	LocknarDummyHookup
		Type	Destroy
	End

	Event
		Ename	LocknarFadeIn
		Type	Destroy
	End
End

Condition
	On	PrimeHit
	
	Event
		Ename	Mushroom
		Type	Local
		At	T_Root
		ChildFx POWERS/RadiationControl/InfectOthersHit.fx
	End
	
	Event
		Ename	Prime
		Type	Destroy
	End

	
End


End

			##		ChildFx  fire_ball.fx
			##A new fx will be created with three inputs: First input:  this fx node. 
			##Next input: this fx node's magnet, if any.   
			##Next input, this fx node's other point, if any.  
			##Note that you do put .fx at the end of the ChildFx's name.

				

	


			