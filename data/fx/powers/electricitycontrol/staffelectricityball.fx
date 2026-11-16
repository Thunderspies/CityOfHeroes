#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	wepR
End

Input  
	Inpname	T_Root
End

LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	node
		Type	local
		At	wepR
		Geom	Staff_02

	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	Prime
		Type	start
		At	node
		altpiv	1
		Part1	:ElectricityControl01a.part
		Part2	:ElectricityControl02a.part
		Part3	:ElectricityInnerGlowa.part
		Part5	:ElectricityBallTail.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		Sound elecball2 80 80 .8
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime2
		Type	start
		At	node
		altpiv	1
		Part1	:ElectricityOuterGlowa.part
		Part2	:ElectricityArcha.part
		Part3	:ElectricityRinga.part
		Part4	:ElectricityArchInverta.part
		Part5	:ElectricityBallTail2.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
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

End

#	Event
#		
#		Type	Local
#		At	Target
#		
#		Part1	:ShockFieldArchsa.part
#		Part2	:ShockFieldArchs2a.part
#		Part3	:ShockFieldArchs1a.part
#		
#		lifespan	30
#	End
#
#	Event
#		ENAME	H
#		Type	start
#		At	T_Root
#		
#		
#		geom	roottochestadjustment
#		lifespan	30
#	End
#	
#	Event
#		EName 	Mallet
#		Type	start 
#		At	H
#		altpiv	1
#
#		Geom	ElectricityHit
#		bhvr	behaviors/ScaleBubble2.bhvr
#		lifespan	3.5
#	End
#
#	Event
#		
#		Type	start
#		At	H
#		altpiv	1
#		Part1	:ElectricitySparkBurst.part
#		Part2	:ElectricitySparkBurst.part
#		Part4	:ShockFieldRingA.part
#		lifespan	30
#	End
#
#	Event
#		
#		Type	Local
#		At	Target
#		Part2	:ShockFieldGlow.part
#		
#		lifespan	5
#	End
#End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End

End


End