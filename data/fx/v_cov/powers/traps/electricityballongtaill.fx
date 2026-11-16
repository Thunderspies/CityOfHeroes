#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	archPoints
		Type	Local 
		At	root
		geom	electricityoffset

	End

	Event
		EName 	Prime
		Type	start
		At	archPoints
		altpiv	3
		Part1	:ElectricityControl01a.part
		Part2	:ElectricityControl02a.part
		Part3	:ElectricityInnerGlowa.part
		Part5	:ElectricityBallTail.part
		Bhvr	Behaviors/MorterLobbprojectile.bhvr
		Sound elecball2 80 80 .8
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime2
		Type	start
		At	archPoints
		altpiv	3

		Part1	:ElectricityOuterGlowa.part
		Part2	:ElectricityArcha.part
		Part3	:ElectricityRinga.part
		Part4	:ElectricityArchInverta.part
		Part5	:ElectricityBallTail2.part
		Bhvr	Behaviors/MorterLobbprojectile.bhvr
		
		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
		
	End

	Event
		
		Type	Local
		At	Target
		
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		Sound elecballhit 60 60 .6
		lifespan	30
	End

	Event
		ENAME	H
		Type	start
		At	T_Root
		
		
		geom	roottochestadjustment
		lifespan	30
	End
	
	Event
		EName 	Mallet
		Type	start 
		At	H
		altpiv	1

		Geom	ElectricityHit
		bhvr	behaviors/ScaleBubble2.bhvr
		lifespan	7.5
	End

	Event
		
		Type	start
		At	H
		altpiv	1
		Part1	Powers/ElectricityControl/ElectricitySparkBurst.part
		Part2	Powers/ElectricityControl/ElectricitySparkBurst.part
		Part4	Powers/ElectricityControl/ShockFieldRingA.part
		lifespan	30
	End

	Event
		
		Type	Local
		At	Target
		Part2	Powers/ElectricityControl/ShockFieldGlow.part
		
		lifespan	5
	End
End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End

End





#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	35
	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

End

End