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
	Inpname	C_HandL
End

Input  
	Inpname	T_Root
End

LifeSpan	200

########################################################

Condition
	On	Time
	Time	36

	Event
		Type	Local
		At	waist
		Sound TeslaBolt1 120 120 .99
	End
End


Condition
	On 	time
	Time 	20
	
	Event
		ename	Charger
		Type	local
		At	waist
		lifespan 40
	end
End

Condition
	On 	cycle
	Time 	2
	
	Event
		Type	local
		At	Charger
		childfx :Bot_Dis_Arcs.fx
		lifespan 20
	end
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime
		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityControl01a.part
		Part2	POWERS\ElectricityControl\ElectricityControl02a.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlowa.part
		Part5	POWERS\ElectricityControl\ElectricityBallTail.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	Prime2
		Type	start
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityOuterGlowa.part
		Part2	POWERS\ElectricityControl\ElectricityArcha.part
		Part3	POWERS\ElectricityControl\ElectricityRinga.part
		Part4	POWERS\ElectricityControl\ElectricityArchInverta.part
		Part5	POWERS\ElectricityControl\ElectricityBallTail2.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	RPrime
		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityControl01a.part
		Part2	POWERS\ElectricityControl\ElectricityControl02a.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlowa.part
		Part5	POWERS\ElectricityControl\ElectricityBallTail.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
		
		Magnet	Target
		Lookat	Target

	End

	Event
		EName 	RPrime2
		Type	start
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityOuterGlowa.part
		Part2	POWERS\ElectricityControl\ElectricityArcha.part
		Part3	POWERS\ElectricityControl\ElectricityRinga.part
		Part4	POWERS\ElectricityControl\ElectricityArchInverta.part
		Part5	POWERS\ElectricityControl\ElectricityBallTail2.part
		Bhvr	Behaviors/Medianprojectile1.bhvr
		
		Magnet	Target
		Lookat	Target
end
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
		
		Part1	POWERS\ElectricityControl\ShockFieldArchsa.part
		Part2	POWERS\ElectricityControl\ShockFieldArchs2a.part
		Part3	POWERS\ElectricityControl\ShockFieldArchs1a.part
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
		lifespan	3.5
	End

	Event
		
		Type	start
		At	H
		altpiv	1
		Part1	POWERS\ElectricityControl\ElectricitySparkBurst.part
		Part2	POWERS\ElectricityControl\ElectricitySparkBurst.part
		Part4	POWERS\ElectricityControl\ShockFieldRingA.part
		lifespan	30
	End

	Event
		
		Type	Local
		At	Target
		Part2	POWERS\ElectricityControl\ShockFieldGlow.part
		
		lifespan	5
	End

		Event
		EName 	RPrime
		Type	Destroy
		
	End

	Event
		
		Type	Local
		At	Target
		
		Part1	POWERS\ElectricityControl\ShockFieldArchsa.part
		Part2	POWERS\ElectricityControl\ShockFieldArchs2a.part
		Part3	POWERS\ElectricityControl\ShockFieldArchs1a.part
		
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
		EName 	RMallet
		Type	start 
		At	H
		altpiv	1

		Geom	ElectricityHit
		bhvr	behaviors/ScaleBubble2.bhvr
		lifespan	3.5
	End

	Event
		
		Type	start
		At	H
		altpiv	1
		Part1	POWERS\ElectricityControl\ElectricitySparkBurst.part
		Part2	POWERS\ElectricityControl\ElectricitySparkBurst.part
		Part4	POWERS\ElectricityControl\ShockFieldRingA.part
		lifespan	30
	End

	Event
		
		Type	Local
		At	Target
		Part2	POWERS\ElectricityControl\ShockFieldGlow.part
		
		lifespan	5
	End
End

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy
		
	End
		Event
		EName 	RPrime2
		Type	Destroy
		
	End

End





#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Thingy1
		Type	local
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		
	End


	Event
		EName 	ThingyB
		Type	local
		At	C_HandL
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		
	End

		Event
		EName 	RThingy1
		Type	local
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityControl01.part
		Part2	POWERS\ElectricityControl\ElectricityControl02.part
		Part3	POWERS\ElectricityControl\ElectricityInnerGlow.part
		
	End


	Event
		EName 	RThingyB
		Type	local
		At	C_HandR
		Part1	POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\ElectricityArch.part
		Part3	POWERS\ElectricityControl\ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	45
	
	Event
		EName 	BurstL
		Type	local
		At	C_HandL
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		LifeSpan	20

	End

		Event
		EName 	RBurstL
		Type	local
		At	C_HandR
		Part4	POWERS\ElectricityControl\ElectricitySparkBurst.part
		LifeSpan	20

	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	Event
		
		EName	RThingy1
		Type	destroy
		
		
	End

	Event
		
		EName	RThingyB
		Type	destroy
		
		
	End


End

End